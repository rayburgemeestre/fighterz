#include "common.h"

static void m_version(struct data *client, unsigned int ver)
{
	if (client->enabled != STATE_NEW)
	{
		kick_player(client, "Invalid protocol sequence (version)");
		return;
	}
	if (ver < MIN_CLIENT_VERSION)
	{
		kick_player(client, "Client too old, please download the latest version");
		return;
	}
	/* Good */
	SetVersion(client);
}

int isvalid(char c)
{
	if (isalpha(c) || isdigit(c) ||
	    (c == '-') || (c == '_') || (c == '[') ||
	    (c == ']') || (c == '|') || (c == '^'))
		return 1;
	else
		return 0;
}

static void m_nick(struct data *client, char *newnick)
{
unsigned int i, nicklen;
struct data *c;

	if (client->enabled < STATE_GOTVERSION)
	{
		kick_player(client, "Invalid protocol sequence (nick)");
		return;
	}
	nicklen = (unsigned int)strlen(newnick);
	if (nicklen == 0)
	{
		send_nickreply(client, 1, "Empty nick");
		return;
	}
	if (nicklen > NICKLEN)
	{
		send_nickreply(client, 1, "Nick too long");
		return;
	}

	for (i=0; i < nicklen; i++)
	{
		if (!isvalid(newnick[i]))
		{
			send_nickreply(client, 1, "Bad character(s) in nick");
			return;
		}
	}
	c = getplayer_bynick(newnick);
	if (c)
	{
		send_nickreply(client, 1, "Nick already in use");
		return;
	}
	
	/* Valid nick */
	strcpy(client->nick, newnick);
	send_nickreply(client, 0, "Ok");
	if (!IsRegistered(client))
	{
		/* New user just got registered */
		send_newuser(EVERYONE, client,
			client->id, client->x, client->y, client->deg,
			(signed char)client->velocity, client->alive, client->frags, client->team,
			(signed char)client->turn, (unsigned char)client->type, client->speed, client->shiptype,
			client->nick);

		/* Start tha lag calc */
		SetLag(client);
		head->init_lag = LAG_CHECKS; /* 5 checks */
		send_servertime(client);
		/* Send user list */
		sendulist(client);
	} else
	{
		/* Normal user changing nick */
		send_nick(EVERYONE, NULL, client->id, client->nick);
	}
}

static void m_lag(struct data *client, unsigned int t)
{
int diff, i;
	verbose("CMSG_PONG %d", t);

	diff = (servertime - t);
	diff = (int)( ( (float)diff / 2.0) + 0.5);
	if (diff < 0)
	{
		kick_player(client, "Negative lag detected");
		return;
	}
	send_lag(client, diff);
	for (i=0; i < 4; i++)
		client->lag[i] = client->lag[i+1];
	client->lag[4] = (double)diff;
}

static void m_spawn(struct data *client)
{
	if (client->dead != 2)
		return; /* Not allowed to respawn (yet), ignore */

	client->dead = 0;
	client->freeze = 0;

	client->invincible = 1;
	client->invincibility_t = 5000;
	client->invincibility_t2 = servertime;
	send_invincible(EVERYONE, NULL, client->id, (unsigned char)client->invincible, client->invincibility_t);
	
	if (client->team == 1)
	{
		client->x = (double)(RED.x + (BLOCKSIZE / 2));
		client->y = (double)(RED.y + (BLOCKSIZE / 2));
	} else {
		client->x = (double)(BLUE.x + (BLOCKSIZE / 2));
		client->y = (double)(BLUE.y + (BLOCKSIZE / 2));
	}

	send_spawn(EVERYONE, NULL,
		client->id, client->x, client->y, client->deg,
		(signed char)client->velocity, client->alive, client->frags,
		(signed char)client->turn, (unsigned char)client->type, client->speed);
}

// *lag*
static void m_accel(struct data *client, double x, double y, signed char accel, double speed)
{
unsigned int clag;
	verbose("CMSG_ACCEL %2.2f %2.2f %d %2.2f", x, y, (int)accel, speed);

#if IGNORE_LAG != 1
	clag = (unsigned int)current_lag(client->id);
#else
	clag = 0
#endif

	/* ajust our db */
	client->x = x;
	client->y = y;
	client->velocity = accel;
	client->vel_time = (servertime - clag);
	client->speed = speed;
	client->t = (servertime - clag);
	client->freeze = 0;
	
	/* move ship to the position we think it SHOULD be */
#if IGNORE_LAG != 1
	moveship(client->id, servertime);
#endif
	send_accel(EVERYONE, client, client->id, client->x, client->y, (signed char)client->velocity, client->speed);
}

// *lag*
static void m_turn(struct data *client, double x, double y, signed char turn, double deg)
{
unsigned int clag;
printf("x: %d, y: %d, deg: %2.2f\n", x, y, deg);

#if IGNORE_LAG != 1
	clag = (unsigned int)current_lag(client->id);
#else
	clag = 0
#endif

	/* ajust our db */
	client->x = x;
	client->y = y;
	client->turn = turn;
	client->deg = deg;
	client->t = (servertime - clag);
	client->turn_t = (servertime - clag);
	client->freeze = 0;

	/* move ship to the position we think it SHOULD be */
#if IGNORE_LAG != 1
	moveship(client->id, servertime);
#endif
	send_turn(EVERYONE, client, client->id, client->x, client->y, (signed char)client->turn, client->deg);
}

// *lag*
void m_newbullet(struct data *client, double x, double y, double deg)
{
unsigned int clag;
struct data *bullet;

#if IGNORE_LAG != 1
	clag = (unsigned int)current_lag(client->id);
#else
	clag = 0
#endif
	
	if (client->bulletcnt == (unsigned)BULLET_MAX)
	{
		LINK oldestbullet = NULL;
		LINK current2;
		/* remove oldest bullet */
		for (current2 = head; current2; current2 = current2->next)
			if (IsBullet(current2) && (current2->owner == client))
				oldestbullet = current2;
		if (!oldestbullet)
		{
			kick_player(client, "Impossible #001");
			return;
		}
			
		oldestbullet->owner->bulletcnt--;
#if DEBUG == 1
		fprintf(outp, "deletingasdf bullet(%d) at addr: %d\n", oldestbullet->id, oldestbullet);fflush(stdout);
#endif
		del_bullet(oldestbullet);
	}

	bullet = add_bullet(client, x, y, deg, servertime - clag);

#if IGNORE_LAG != 1
	if (!movebullet(bullet)) /* if movebullet returned 1 then bullet is already gone */
#endif
		send_newbullet(EVERYONE, client, bullet->id, client->id, x, y, deg);
}

static void m_newship(struct data *client, int shiptype)
{
	client->shiptype = shiptype;
	send_newship(client);
}

static void m_cmd(struct data *client, char *cmd)
{
size_t len;
	cleanstr(cmd);
	len = strlen(cmd);
	if (len == 0)
		return; /* Ignore empty msgs */
	if (len > 512)
		cmd[511] = '\0';

	printf("[%s@fighterz] %s\n", client->nick, cmd);

	if (!strcmp(cmd, "quit"))
	{
		quit_player(client, "Connection terminated, Client left.");
		del_tha_player(client);
	}
	else if (!strcmp(cmd, "die"))
	{
		LINK node = head;
		while (node != NULL)
		{
			quit_player(node, "Server shutting down");
			node = node->next;
		}
		exit(0);
	}
	else if (!strncmp(cmd, "kick ", 5))
	{
		if (strlen(cmd) > 5)
		{
		int temp;
		char *ptr;
		char buf[512];
		LINK node;

			
			ptr = cmd;
			ptr += 5;
			temp = atoi(ptr);
			node = getplayer_byid(temp);
			if (!node)
				return;

			sprintf(buf, "Kicked by: %s", client->nick);
			node->dead = 3; // Dead @ 3 is disabled
			softkick_player(node, buf);

			//flagstuff
			if (id_has_redflag == node->id)
			{
				id_has_redflag = -1;
				REDFLAG.x = node->x;
				REDFLAG.y = node->y;
			}
			else if (id_has_blueflag == node->id)
			{
				id_has_blueflag = -1;
				REDFLAG.x = node->x;
				REDFLAG.y = node->y;
			}
		}
	}
	else if (!strncmp(cmd, "skick ", 6))
	{
		if (strlen(cmd) > 6)
		{
		int temp;
		char *ptr;
		char buf[512];
		LINK node;

			
			ptr = cmd;
			ptr += 5;
			temp = atoi(ptr);
			node = getplayer_byid(temp);

			sprintf(buf, "Kicked from server by: %s", client->nick);
			quit_player(node, buf);
			del_tha_player(node);
			
			//flagstuff
			if (id_has_redflag == node->id)
			{
				id_has_redflag = -1;
				REDFLAG.x = node->x;
				REDFLAG.y = node->y;
			}
			else if (id_has_blueflag == node->id)
			{
				id_has_blueflag = -1;
				BLUEFLAG.x = node->x;
				BLUEFLAG.y = node->y;
			}
		}
	}
	else if (!strcmp(cmd, "spec"))
	{
		if (1 == 2)
		{//deprecated
		// del_tha_player(client);
		client->velocity = 0;
		client->speed = 0;
		client->dead = 2;
		send_spawnready(client);
		// softkick_player(client, "Spectator mode");
		} else {
			client->velocity = 0;
			client->speed = 0;
			client->dead = 2;
			client->dead = 2;
			softkick_player(client, "Thou shall spectate");
			send_spawnready(client);
		}

		//flagstuff
		if (id_has_redflag == client->id)
		{
			id_has_redflag = -1;
			REDFLAG.x = client->x;
			REDFLAG.y = client->y;
		}
		else if (id_has_blueflag == client->id)
		{
			id_has_blueflag = -1;
			BLUEFLAG.x = client->x;
			BLUEFLAG.y = client->y;
		}
	}
	else if (!strcmp(cmd, "respawn"))
	{
		client->dead = 2;
		send_spawnready(client);		

		//flagstuff
		if (id_has_redflag == client->id)
		{
			id_has_redflag = -1;
			REDFLAG.x = client->x;
			REDFLAG.y = client->y;
		}
		else if (id_has_blueflag == client->id)
		{
			id_has_blueflag = -1;
			BLUEFLAG.x = client->x;
			BLUEFLAG.y = client->y;
		}
	}
	else if (!strcmp(cmd, "godme")) // todo: auth needed :]
	{
		int diff;
		diff = servertime - client->invincibility_t2;
		
		client->invincible = 2;
		send_invincible(NULL, NULL, client->id, client->invincible, servertime - diff);
	}
	else if (!strncmp(cmd, "nick ", 5))
	{
		if (strlen(cmd) > 5)
		{
			char *p = (cmd + 5);
			if (strlen(p) > 64)
				*(p + 63) = '\0';

			strcpy(client->nick, p);

			send_nick(EVERYONE, NULL, client->id, client->nick);
		}
	}
	else if (!strcmp(cmd, "whoami"))
	{
		// TODO: send nick back ;))
	}
	else if (!strcmp(cmd, "addbot"))
	{
		struct data *bot;
		// findpath(head);
		bot = add_bot();
		notify_of_newuser(EVERYONE, NULL, bot);
		
		notify_of_respawn(EVERYONE, NULL, bot);
		//flyto((bot_id - 1), REDFLAG.x, REDFLAG.y);

		// flyto random place
										{
											int rand_x, rand_y, tx, ty;
											int continue_loop = 0;
											do
											{
												rand_x = 1+(int) (field_width*rand()/(RAND_MAX+1.0));
												rand_y = 1+(int) (field_height*rand()/(RAND_MAX+1.0));

												tx = (int) ((rand_x - (BLOCKSIZE / 4)) / BLOCKSIZE);
												ty = (int) ((rand_y - (BLOCKSIZE / 4)) / BLOCKSIZE);
											} while (field[ty][tx] == '1');

											flyto(bot_id - 1, rand_x, rand_y);
										}
	}
	else if (!strcmp(cmd, "delall"))
	{
		LINK x;
again:
		x = head;
		while (x != NULL)
		{
			if (x->bot == 1)
			{
				x->errmsg = strdup("Removed from botnet");
				quit_player(x, x->errmsg);
				del_tha_player(x);
				goto again;
			}

			x = x->next;
		}
		return;
	}
	else if (!strcmp(cmd, "ship1")) { client->shiptype = 1; send_newship(client); }
	else if (!strcmp(cmd, "ship2")) { client->shiptype = 2; send_newship(client); }
	else if (!strcmp(cmd, "ship3")) { client->shiptype = 3; send_newship(client); }
	else if (!strcmp(cmd, "ship4")) { client->shiptype = 4; send_newship(client); }
	else if (!strcmp(cmd, "ship5")) { client->shiptype = 5; send_newship(client); }
	else if (!strcmp(cmd, "ship6")) { client->shiptype = 6; send_newship(client); }
}

static void m_say(struct data *client, char *txt)
{
size_t len;
	cleanstr(txt);
	len = strlen(txt);
	if (len == 0)
		return; /* Ignore empty msgs */
	if (len > 512)
		txt[511] = '\0';

	send_say(EVERYONE, NULL, client->id, txt);
}

/** Packet parse routine.
 * This routine parses every packet and hands it over to the m_* functions.
 */
void dopacket(struct data *client, unsigned short xtype, unsigned short len, char *dta)
{
	// printf("dopacket: type=%u (0x%x), len=%hu\n", xtype, xtype, len);

	/* Msgs which are allowed if unregistered */
	switch(xtype)
	{
		case CMSG_VERSION:
		{
			unsigned int ver;
			printf("got version\n");
			if (!get_u32(&ver, &dta, &len))
				goto fatal;
			
			m_version(client, ver);
			return;
		}
		case CMSG_NICK:
		{
			char newnick[256];
			printf("got nick.. \n");
			if (!get_str(newnick, &dta, &len, sizeof(newnick)))
				goto fatal;
			printf("got nick %s\n", newnick);
			m_nick(client, newnick);
			return;
		}
		case CMSG_PONG:
		{
			unsigned int datime;
			if (!get_u32(&datime, &dta, &len))
				goto fatal;
			m_lag(client, datime);
			return;
		}
		default:
			break;
	}
	if (client->enabled < STATE_OK)
	{
		kick_player(client, "Packet error/not registered");
		return;
	}

	/* Other msgs here */
	switch(xtype)
	{
		case CMSG_SPAWN:
		{
			m_spawn(client);
			return;
		}
		case CMSG_ACCEL:
		{
			double x, y, speed;
			signed char accel;
			if (!get_dbl(&x, &dta, &len))
				goto fatal;
			if (!get_dbl(&y, &dta, &len))
				goto fatal;
			if (!get_s8(&accel, &dta, &len))
				goto fatal;
			if (!get_dbl(&speed, &dta, &len))
				goto fatal;
			
			m_accel(client, x, y, accel, speed);
			return;
		}
		case CMSG_TURN:
		{
			double x, y, deg;
			signed char turn;
			if (!get_dbl(&x, &dta, &len))
				goto fatal;
			if (!get_dbl(&y, &dta, &len))
				goto fatal;
			if (!get_s8(&turn, &dta, &len))
				goto fatal;
			if (!get_dbl(&deg, &dta, &len))
				goto fatal;
			
			m_turn(client, x, y, turn, deg);
			return;
		}
		case CMSG_NEWBULLET:
		{
			double x, y, deg;
			if (!get_dbl(&x, &dta, &len))
				goto fatal;
			if (!get_dbl(&y, &dta, &len))
				goto fatal;
			if (!get_dbl(&deg, &dta, &len))
				goto fatal;

			m_newbullet(client, x, y, deg);
			return;
		}
		case CMSG_NEWSHIP:
		{
			int shiptype;

			if (!get_s32(&shiptype, &dta, &len))
				goto fatal;

			m_newship(client, shiptype);
			return;
		}
		case CMSG_CMD:
		{
			char cmd[512];
			if (!get_str(cmd, &dta, &len, sizeof(cmd)))
				goto fatal;
			
			m_cmd(client, cmd);
			return;
		}
		case CMSG_SAY:
		{
			char text[512];
			if (!get_str(text, &dta, &len, sizeof(text)))
				goto fatal;
			
			m_say(client, text);
			return;
		}
		case CMSG_TEAMSAY:
		{
			/* Not implemented yet */
			return; 
		}
	}


	/* This point should never be reached */
	return;
fatal:
	
	kick_player(client, "Packet error");
	return;
}
