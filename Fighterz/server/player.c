#include "common.h"

/********************************************
*  PLAYER FUNCTIONS
****************************************************/

struct data *add_player(ID sock_fd)
{
	LINK new_node;
	
//	addtext("ADD BOT TRIGGERED");
	printf_("Creating player: %d\n", sock_fd);

	player_nr++;

	if (!(new_node = (LINK)malloc(sizeof(user))))
	{   printf_("Error allocating memory");
		exit(-1);
	}

	/* Zerofill it ^_^ */
	memset(new_node, 0, sizeof(user));

	new_node->next = head;
	head = new_node;
	
	head->id = sock_fd;
	head->dead = 1;
	head->died = servertime;
	head->bullet = 0;
	head->bot = 0;
//	addtext("we made a human");
	head->bulletcnt = 0;
	head->enabled = STATE_NEW;
	head->x = (double)(RED.x + BLOCKSIZE / 2);
	head->y = (double)(RED.y + BLOCKSIZE / 2); /* -> will eventually become dynamic */
	/* -100,-100 means 'not visibile yet/dead' */
	/* head->x = -100.0;
	head->y = -100.0; */
	head->deg = 180;
	head->power = MAX_HITS;
	head->t = servertime;
	head->move = 0;
	head->turn = 0;
	head->turn_t = servertime;
	head->velocity = 0;
	head->speed = 0.0;
	head->max_speed = 0.2;
	head->vel_time = servertime;
	head->alive = 0; /* seconds in the game*/
	//head->score_update = time(NULL);
	head->frags = 0;
	head->invincible = 1;
	head->invincibility_t = 5000; /* millisecs */
	head->invincibility_t2 = servertime;
//	head->tmpstr[0] = '\0';
	/* TODO: only alloc for players :P */
	head->buffer = malloc(8192);
	memset(head->buffer, 'X', 8192);
	head->inlen = 0;
	head->freeze = 0;

	{
		int rnd = 1+(int) (4.0*rand()/(RAND_MAX+1.0));
		head->shiptype = rnd;
	}

	head->type = T_PLAYER;
	
	head->init_lag = 0;
	head->last_lag_check = servertime;
	head->path[PATH_MAX_-1][0] = 0;
	head->path[PATH_MAX_-1][1] = 0;

	snprintf(head->nick, NICKLEN, "Unit%d", player_nr);
	return head;
}

/* to delete a bot you can still use del_player() ;) 
	this is just for creating 
*/
struct data *add_bot()
{
	LINK new_node;
	
//addtext("ADD BOT TRIGGERED");

	// printf_("Creating bot: %d\n", bot_id);

	player_nr++;

	if (!(new_node = (LINK)malloc(sizeof(user))))
	{   printf_("Error allocating memory");
		exit(-1);
	}
	new_node->next = head;
	head = new_node;

	head->id = bot_id++;
	head->dead = 0;
	head->died = 0;
	head->bullet = 0;
	head->bot = 1;
	
	head->bullet_time = servertime;
	head->bulletcnt = 0;
	head->enabled = 0;
#if 0
	head->x = RED.x + BLOCKSIZE / 2;
	head->y = RED.y + BLOCKSIZE / 2; /* -> will eventually become dynamic */
#endif
	head->x = (double)(BLUE.x + BLOCKSIZE / 2);
	head->y = (double)(BLUE.y + BLOCKSIZE / 2); /* -> will eventually become dynamic */
	
	head->deg = 180;
	head->power = MAX_HITS;
	head->t = servertime;
	head->move = 0;
	head->turn = 0;
	head->turn_t = servertime;
	head->speed = 0.00;
	head->max_speed = 0.10;
	head->velocity = 0;
	head->vel_time = servertime;
	head->alive = 0; /* seconds in the game*/
	//head->score_update = time(NULL);
	head->frags = 0;
	head->invincible = 1;
	head->invincibility_t = 5000; /* millisecs */
	head->invincibility_t2 = servertime;

	head->buffer = NULL;

	head->init_lag = 0; /* 5 checks */
	head->last_lag_check = servertime;
	head->lag[0] = 0;
	head->lag[1] = 0;
	head->lag[2] = 0;
	head->lag[3] = 0;
	head->lag[4] = 0;
	head->path[PATH_MAX_-1][0] = 0;
	head->path[PATH_MAX_-1][1] = 0;

	head->type = T_BOT;
	head->freeze = 0;
	
	//head->target_x = BLUEFLAG.x + (BLOCKSIZE / 2);
	//head->target_y = BLUEFLAG.y + (BLOCKSIZE / 2);

	snprintf(head->nick, NICKLEN, "Bot%d", player_nr);
	return head;
}

void del_player(ID sock_fd)
{
	LINK old = NULL;
	LINK future = NULL;
	LINK current = head;
	
	while (current != NULL)
	{
		future = current->next;
		if (current->id == sock_fd)
		{
			if (current == head)
			{
				/* first node */
				future = current->next;
#if DEBUG == 1
				fprintf(outp, "FREEINGd current(%s) at addr: %d\n", current->nick, current);fflush(stdout);
#endif
				free(current);
				head = future;
				return;
			} else {
				if (current->next != NULL)
				{
					/* mid node */
#if DEBUG == 1
					fprintf(outp, "FREEINGe current(%s) at addr: %d\n", current->nick, current);fflush(stdout);
#endif
					free(current);
					old->next = future;
					return;
				} else {
					/* last node */
#if DEBUG == 1
					fprintf(outp, "FREEINGf current(%s) at addr: %d\n", current->nick, current);fflush(stdout);
#endif
					free(current);
					old->next = NULL;
					return;
				}
			}
		}
		old = current;
		current = current->next;
	}
}

void del_tha_player(struct data *client)
{
	CLOSE_SOCK((int)client->id);
	del_bullets(client->id);
	del_player(client->id);
}

struct data *getplayer_byid(ID id)
{
	LINK current;
	for (current = head; current; current = current->next)
		if (current->id == id)
			return current;
	return NULL;
}

struct data *getplayer_bynick(char *nick)
{
	LINK current;
	for (current = head; current; current = current->next)
		if (IsRegistered(current) && !strcmp(current->nick, nick)) /* CHK: was strcasecmp */
			return current;
	return NULL;
}


void printplayerlist()
{
	LINK current = head;
	if (!head)
		return;
	printf_("------------- player list -------------\n");
	while (current != NULL)
	{
		if (current->bullet != 1)
			printf_("- %s (lag:%2.2fms id:%u ei:%d)\n", current->nick, current_lag(current->id), current->id, current->enabled);
		current = current->next;
	}
	printf_("---------------------------------------\n");
}

// sendulist(id); will send already connected players
void sendulist(struct data *client)
{
	int diff;
	LINK current;
	
	for (current = head; current; current = current->next)
	{
		send_newuser(client, NULL,
			current->id, current->x, current->y, current->deg,
			(signed char)current->velocity, current->alive, current->frags, current->move,
			(signed char)current->turn, (unsigned char)current->type, current->speed, current->shiptype,
			current->nick);
		// send_newuser(
		if (current->invincible)
		{
			diff = servertime - current->invincibility_t2;
			send_invincible(NULL, NULL,
				current->id, (unsigned char)current->invincible, current->invincibility_t - diff);
			// TODO: i think this line ^ is incorrect........
		}
	}
}

#if DEBUG2 == 1
/* Prints userlist */
void printulist() 
{
LINK current;
	TXTPTR2 = TOP;
	textprintf(tmpscreen, font, LEFT_2, TXTPTR2 , 15, "Player List:");
	TXTPTR2 = TOP + 20;

	current = head;
	while (current != NULL)
	{
		if (current->bullet != 1)
			printul((char *)(current->nick), (long int)(current->frags) );

		current = current->next;
	}
}

/* Used in printulist(); */
void printul(char *nick, long int one) 
{
	char buf[128];
	/* again no snprintf().. arghl*/
	if (strlen(nick) > 30) nick[30] = '\0'; /* frag >30 */
	sprintf(buf, "%8s %4d", nick, one);

	TXTPTR2 = TXTPTR2 + 10;

	if (TXTPTR2 == 0) { 
		textprintf(tmpscreen, font, LEFT_2, 1, 42, buf); /* 42 = makecol(255, 128, 0) (orange) */
	} else { 
		textprintf(tmpscreen, font, LEFT_2, TXTPTR2, 42, buf);
	}
}
#endif

/* notify connected users of new bot or player */
void notify_of_newuser(struct data *client, struct data *except, struct data *current)
{
int diff = servertime - current->invincibility_t2;

	send_newuser(client, except,
		current->id, current->x, current->y, current->deg,
		(signed char)current->velocity, current->alive, current->frags, current->move,
		(signed char)current->turn, (unsigned char)current->type, current->speed, current->shiptype,
		current->nick);
	//send_newuser(

	diff = servertime - current->invincibility_t2;
	send_invincible(client, except, current->id, (unsigned char)current->invincible, current->invincibility_t - diff);
	// send_invincible(
}

/* notify connected users of a respawn */
void notify_of_respawn(struct data *client, struct data *except, struct data *current)
{
int diff = servertime - current->invincibility_t2;

	send_spawn(client, except,
		current->id, current->x, current->y, current->deg,
		(signed char)current->velocity, current->alive, current->frags,
		(signed char)current->turn, (unsigned char)current->type, current->speed);

	diff = servertime - current->invincibility_t2;
	send_invincible(client, except, current->id, (unsigned char)current->invincible, current->invincibility_t - diff);
}

void kick_player(struct data *client, char *reason)
{
	if (!IsRegistered(client))
		send_kick(client, NULL, client->id, reason);
	else
		send_kick(EVERYONE, NULL, client->id, reason);
	SetDead(client);
}

void softkick_player(struct data *client, char *reason)
{
	if (!IsRegistered(client))
		send_kick(client, NULL, client->id, reason);
	else
		send_kick(EVERYONE, NULL, client->id, reason);
}

void quit_player(struct data *client, char *reason)
{
	if (!IsRegistered(client))
		send_quit(client, NULL, client->id, reason);
	else
		send_quit(EVERYONE, NULL, client->id, reason);

	SetDead(client);
}

