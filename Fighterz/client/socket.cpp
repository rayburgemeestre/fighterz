#include "common.h"

/********************************************
*  SOCKET FUNCTIONS
****************************************************/

void connect_socket()
{
	WORD version = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;

	const int just_say_no = 1;

	/* Start up Winsock as before */
	WSAStartup(version, &wsaData);

	/* Store information about the server */
	LPHOSTENT lpHostEntry;
	lpHostEntry = gethostbyname(taddr);  /* Specifying server by its name */

	if (lpHostEntry == NULL) {
		printff_direct("Error at gethostbyname()");
		terminate();
		return;
	}

	/* Create the socket */
	// socket declared in main.h

	theSocket = socket(AF_INET,	/* Go over TCP/IP */
				SOCK_STREAM,	/* Socket type */
				IPPROTO_TCP); /* Protocol */
	
	if (theSocket == INVALID_SOCKET) {
		printff_direct("Error at socket()");
		terminate();
		return;
	}

	
	/* Use SOCKADDR_IN to fill in address information */
	SOCKADDR_IN saServer;
	
	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
			  /* ^ Address of the server being inserted into the address field */
	saServer.sin_port = htons(8099);

	if (setsockopt(theSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&just_say_no,
														sizeof just_say_no) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	/* Connect to the server */
	nRet = connect(theSocket,
				  (LPSOCKADDR)&saServer, /* Server address */
				  sizeof(struct sockaddr));	/* Length of address structure */

	if (nRet == SOCKET_ERROR) {
		printff_direct("Error at connect()");
		terminate();
		return;
	}

	/* Connected */
	printff_direct("Socket succesfully connected..");
	printff_direct("");

	/* Calculating lag and requesting to join the server */
	unsigned long one = 1;
	int pRet = 0;
	  /* make socket non-blocking */
	ioctlsocket(theSocket, FIONBIO, &one);

return; /* Fuck this shit i will only need one sockread anyway */
}

void sockread()
{
	char buf[8192], tmpbuf[4096], *p = buf;
	int r, len;
	unsigned short plen;
	unsigned long one = 1;

	ioctlsocket(theSocket, FIONREAD, &one);
	if (one > 0)
	{
		r = recv(theSocket, tmpbuf, sizeof(tmpbuf),	0);
		if (r == SOCKET_ERROR)
			die("Error on recv()");

		memcpy(buf, buffer, inlen);
		memcpy(buf + inlen, tmpbuf, r);
		len = r + inlen;

		memcpy(buf, buffer, inlen);
		memcpy(buf + inlen, tmpbuf, r);
		len = r + inlen;
		{
			int i;
			char pbuf[512];
			strcpy(pbuf, "dbg: ");
			for (i=0; i < len; i++)
			{
				sprintf(pbuf+strlen(pbuf), "%.2X ", (unsigned char)buf[i]);
				if (strlen(pbuf) > 300)
						break;
			}
			// verbose("%s", pbuf);
		}
		while(len > 0)
		{
			char *p2 = p;
			unsigned short xlen = (unsigned short)len;
			unsigned short xtype;
			
			if (!get_u16(&plen, &p2, &xlen))
				break; /* Insufficient data (short header) */
			if (xlen < plen)
				break; /* Insufficient data (short type/data) */
			if (!get_u16(&xtype, &p2, &xlen))
				break; /* Impossible */

			dopacket(xtype, plen-2, p2);

			p += (2 + plen);
			len -= (2 + plen);
		}
		inlen = len;
		if (len > 0)
			memcpy(buffer, p, len);
		return;
	}

}
void dopacket(int xtype, unsigned short len, char *dta)
{

	/* Msgs which are allowed if unregistered */
	switch(xtype)
	{
		case SMSG_HI:
		{
			unsigned int sver, oid;
			if (!get_u32(&sver, &dta, &len))
				goto fatal;
			if (!get_u32(&oid, &dta, &len))
				goto fatal;

			m_hi(sver, oid);
			return;
		}
		case SMSG_MOTD:
		{
			verbose("SMSG_MOTD");
			break;
		}
		case SMSG_PING:
		{
			unsigned int servertime;

			if (!get_u32(&servertime, &dta, &len))
				goto fatal;	
			
			m_ping(servertime);
			return;
		}
		case SMSG_LAG:
		{
			signed int diff;
			if (!get_s32(&diff, &dta, &len))
				goto fatal;
							
			m_lag((double)diff);
			return;
		}
		case SMSG_SPAWNREADY:
		{
			m_spawnready();
			return;
		}
		case SMSG_NICKREPLY:
		{
			signed char acceptance;
			char str[256];

			if (!get_s8(&acceptance, &dta, &len))
				goto fatal;
			if (!get_str(str, &dta, &len, sizeof(str)))
				goto fatal;

			m_nickreply(acceptance, str);
			return;
		}
		default:
			break;
	}

	//if (client->enabled < STATE_OK)
	//	return kick_player(client, "Packet error/not registered");

	/* Other msgs here */
	switch(xtype)
	{
		case SMSG_ACCEL:
		{
			unsigned int id;
			double x, y, speed;
			signed char accel;
			
			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_dbl(&x, &dta, &len))
				goto fatal;
			if (!get_dbl(&y, &dta, &len))
				goto fatal;
			if (!get_s8(&accel, &dta, &len))
				goto fatal;
			if (!get_dbl(&speed, &dta, &len))
				goto fatal;

			m_accel(id, x, y, accel, speed);
			return;
		}
		case SMSG_TURN:
		{
			unsigned int id;
			double x, y, deg;
			signed char turn;
			
			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_dbl(&x, &dta, &len))
				goto fatal;
			if (!get_dbl(&y, &dta, &len))
				goto fatal;
			if (!get_s8(&turn, &dta, &len))
				goto fatal;
			if (!get_dbl(&deg, &dta, &len))
				goto fatal;
			// verbose("SMSG_TURN");
			m_turn(id, x, y, turn, deg);
			return;
		}
		case SMSG_INVINCIBLE:
		{
			verbose("SMSG_INVINCIBLE");
			return;
		}
		case SMSG_HIT:
		{
			// verbose("SMSG_HIT");

			m_hit();			
			return;
		}
		case SMSG_SAY:
		{
			// verbose("SMSG_SAY");
			unsigned int id;
			char msg[512];

			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_str(msg, &dta, &len, sizeof(msg)))
				goto fatal;

			m_say(id, msg);
			return;
		}
		case SMSG_TEAMSAY:
		{
			verbose("SMSG_TEAMSAY");
			return;
		}
		case SMSG_NEWUSER:
		{
			// verbose("SMSG_NEWUSER");
			unsigned int id;
			unsigned int alive;
			unsigned int pending_moves;
			double x, y, deg, speed;
			signed char accel, turn;
			unsigned char type;
			signed short frags;
			char nick[64];

			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_dbl(&x, &dta, &len))
				goto fatal;
			if (!get_dbl(&y, &dta, &len))
				goto fatal;
			if (!get_dbl(&deg, &dta, &len))
				goto fatal;
			if (!get_s8(&accel, &dta, &len))
				goto fatal;
			if (!get_u32(&alive, &dta, &len))
				goto fatal;
			if (!get_s16(&frags, &dta, &len))
				goto fatal;
			if (!get_u32(&pending_moves, &dta, &len))
				goto fatal;
			if (!get_s8(&turn, &dta, &len))
				goto fatal;
			if (!get_u8(&type, &dta, &len))
				goto fatal;
			if (!get_dbl(&speed, &dta, &len))
				goto fatal;
			if (!get_str(nick, &dta, &len, sizeof(nick)))
				goto fatal;
			
			// verbose("got nick: %s", nick);

			// strcpy(nick, "TEMPPPP");
			
			m_newuser(id, x, y, deg, accel, alive, frags, pending_moves, turn, type, speed, nick);
			// alert("newua", "", "", "", "", 0, 0);
			return;
		}
		case SMSG_DELOBJ:
		{
			verbose("SMSG_DELOBJ");
			return;
		}
		case SMSG_KILL:
		{
			unsigned int victimid, evilid;
			char *killstr = strdup("");

			// verbose("SMSG_KILL <victimid (u32)> <evilid (u32)> <killtxt (str)>");

			if (!get_u32(&victimid, &dta, &len))
				goto fatal;
			if (!get_u32(&evilid, &dta, &len))
				goto fatal;
			if (!get_str(killstr, &dta, &len, sizeof(killstr)))
				goto fatal;
			
			m_kill(victimid, evilid, killstr);
			return;
		}
		case SMSG_NEWBULLET:
		{
			unsigned int id, ownerid;
			double x, y, deg;
			// verbose("SMSG_NEWBULLET");

			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_u32(&ownerid, &dta, &len))
				goto fatal;
			if (!get_dbl(&x, &dta, &len))
				goto fatal;
			if (!get_dbl(&y, &dta, &len))
				goto fatal;
			if (!get_dbl(&deg, &dta, &len))
				goto fatal;

			m_newbullet(id, ownerid, x, y, deg);
			return;
		}
		case SMSG_NICK:
		{
			unsigned int id;
			char nick[64];
			// verbose("SMSG_NICK");
			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_str(nick, &dta, &len, sizeof(nick)))
				goto fatal;

			m_nick(id, nick);
			return;
		}
		case SMSG_SPAWN:
		{
			unsigned int oid;
			verbose("SMSG_SPAWN");
			if (!get_u32(&oid, &dta, &len))
				goto fatal;
			m_spawn(oid);
			return;
		}
		case SMSG_RESPAWN:
		{
			unsigned int oid;
			verbose("SMSG_RESPAWN");
			if (!get_u32(&oid, &dta, &len))
				goto fatal;
			m_respawn(oid);
			return;
		}
		case SMSG_QUIT:
		{
			// verbose("SMSG_QUIT");
			unsigned int id;
			char quit_msg[64];

			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_str(quit_msg, &dta, &len, sizeof(quit_msg)))
				goto fatal;

			m_quit(id, quit_msg);
			return;
		}
		case SMSG_KICK:
		{
			verbose("SMSG_KICK");
			return;
		}
	}

	printff_direct("Ignored type: 0x%x", xtype);

	/* This point should never be reached */
	return;
fatal:
	die("Packet error");
}



int sockwrite(char *pattern, ...)
{
	char buf[512];
	int total = 0;
	int bytesleft;
	int n;

	va_list ap;
	va_start (ap, pattern);
	vsprintf(buf, pattern, ap);
	va_end(ap);

	addtext("sockwrite(): %s -- ignored", strtok(buf, "\n"));	
	return 0;

	printff_direct("  Sending: %s", buf);

	bytesleft = strlen(buf);

	// printf_("%lu SEND[%d]: %s", servertime, sockfd, buf);
	// addtext("-> %s", buf);

	//if (send(sockfd, buf, strlen(buf), 0) == -1)
	//	perror("send");

	while((unsigned)total < strlen(buf)) {
		n = send(theSocket, &buf[total], bytesleft, 0);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
	}

	if (n == SOCKET_ERROR)
	{
		printff_direct("Error on send()");
		terminate();
	}

	return 0;
}

/* this function used to have another purpose 
so the structure is a bit fragged */
void lag_proc(char *lag_char, char *servertime_char)
{  // TODO: i am not sure if i'll need the servertime ?!
	
	double _lag = strtod(lag_char, '\0');
	unsigned long servertime = atol(servertime_char);
	int i = 0;

	//addtext("lag: %2.2f servertime: %lu", servertime);

	while (i < 4)
	{
		lag[i] = lag[i+1];
		i++;
	}

	lag[i] = _lag;
	// addtext("lag 1:%2.2f 2:%2.2f 3:%2.2f 4:%2.2f 5:%2.2f", lag[0] , lag[1] , lag[2] , lag[3] , lag[4]);

}

void m_hi(unsigned int sver, unsigned int oid)
{
	// verbose("SMSG_HI: %d %d", sver, oid);
	our_id = oid; /* and our id :) */

	/* We'll now send */
	/* - version */
	send_version();
	/* - nickname */
	send_nickname();
}

void m_ping(unsigned int servertime)
{
	// verbose("SMSG_PING: %d", servertime);
	send_pong(servertime);
}

void m_lag(double diff)
{
	// verbose("SMSG_LAG: %f (avg lag now: %2.2f)", diff, current_lag());
	int i = 0;
	while (i < 4)
	{
		lag[i] = lag[i+1];
		i++;
	}
	lag[i] = diff;
}

void m_nickreply(unsigned char acceptance, char *msg)
{
	verbose("SMSG_NICKREPLY: %d %s", (int)acceptance, msg);
}

void m_quit(unsigned int id, char *quit_msg)
{
	LINK current = getplayer_byid(id);
	if (current == NULL)
		die("m_quit(): current cannot be NULL");

	explosion(current->x, current->y, 500, 10, makecol(255, 128, 255));
	
	if (current != our_node)
	{
		verbose("*** Quit: %s [%s]", current->nick, quit_msg);
		del_player(id);
	}
	else
	{
		our_node->dead = 1;
		verbose("*** Disconnected [%s]", quit_msg);
		; // we dead ;)
	}
}

void m_spawnready()
{
	verbose("SMSG_SPAWNREADY");
	can_spawn = 1;
	requested_spawn = 0;
	
	set_window_title("Fighterz -- You can press enter to spawn now.");

}

void m_newuser(int id, double x, double y, double deg,
	signed char accel, unsigned int alive, signed short frags, unsigned int pending_moves,
	signed char turn, unsigned char type, double speed,
	char *nick)
{
	verbose("---------------------- adding new user ----------------------");
	verbose("id: %d x: %2.2f y: %2.2f deg: %2.2f accel: %d, alive: %d", 
	         id, x, y, deg, (int)accel, alive);
	verbose("frags: %d turn: %d type: %d speed: %2.2f nick: %s", (int)frags,
	         (int)turn, (int)type, speed, nick);
	verbose("-------------------------------------------------------------");

	/* create first node in linked list */
	if (!(new_node = (LINK)malloc(sizeof(user))))
	{
		printff_direct("Error allocating memory");
		exit(-1);
	}
	/* insert the created node */
	new_node->next = head;
	head = new_node;
	/* intiate some values */
	head->id = id;
	head->dead = 0; /* CHK */
	
	if (type == T_BOT) /* TODO: change all these seperate */
		head->bot = 1; /* fucking variables to one named type */
	else
		head->bot = 0;

	if (type == T_BULLET)
		head->bullet = 1;
	else
		head->bullet = 0;

	head->power = MAX_HITS;
	head->TTL = 0;
	head->vel_time = ourtime;
	head->x = x;
	head->y = y;
    setsec(head); /* secondary coordinates for this struct */
	head->deg = deg;
	head->t = ourtime;
	head->move = 0;
	head->freeze = 0; /* CHK */
	head->turn = turn;
	head->turn_t = ourtime; /* CHK */
	head->velocity = accel;
	head->speed = speed;
	// not yet used: head->alive = alive; /* seconds in the game*/
	head->kills = frags;

	head->invincible = 0;
	head->invincibility_t = 0;
	head->invincibility_t2 = ourtime;
	head->hit_t = ourtime;
	head->impact = 0;
	strcpy(head->nick, nick);

// dbg 
return;
	alert("ADDED", nick, "", "", "", 1, 1);
	for (current=head; current; current=current->next)
		alert(current->nick, "", "", "", "", 1, 1);
	delay(2);
}

void m_kill(unsigned int victimid, unsigned int evilid, char *killstr)
{
	LINK evil_node = getplayer_byid(evilid);
	LINK victim_node = getplayer_byid(victimid);

	if (evil_node != NULL && victim_node != NULL)
	{
		victim_node->velocity = 0;
		victim_node->speed = 0.0;
		victim_node->dead = 1;
		victim_node->power = MAX_HITS;
		
		if (victim_node == our_node)
			guessed_power = MAX_HITS;

		evil_node->kills++;
		addtext("*** %s(%d) fragged %s(%d)", evil_node->nick, evil_node->id,
			victim_node->nick, victim_node->id);
		
		explosion(victim_node->x, victim_node->y, 250, 10, makecol(255,0,0));
	} else {
		die("evil_node or victim_node is NULL");
	}
}

void m_newbullet(unsigned int id, unsigned int ownerid, double x, double y, double deg)
{
	// *the entire `unsigned int id` var isn't used
	
	//verbose("New bullet id: [!], ownerid: %d, x: %2.2f, y: %2.2f, deg: %2.2f\n",
	//	ownerid, x, y, deg);

	add_bullet( getplayer_byid(ownerid), (int)x, (int)y, (double)deg, ourtime);

	return;
}

void m_nick(unsigned int id, char *nick)
{
	LINK node = getplayer_byid(id);
	if (strlen(nick) > 64)
		*(nick + 63) = '\0';
	verbose("*** Nickchange: %s -> %s", node->nick, nick);
	strcpy(node->nick, nick);
}

void m_say(unsigned int id, char *msg)
{
	// verbose("m_say");
	LINK node = getplayer_byid(id);
	addtext("<[%d]%s> %s", id, node->nick, msg);
}

void m_spawn(unsigned int oid)
{
	// verbose("spawned with id: %d", oid);
	if (oid == our_id)
	{
		/* sync our_node */
		for (our_node=head; our_node->id!=our_id; our_node=our_node->next);

		if (!our_node)
			die("Failed synching our_node with our_id");
		
		set_window_title("Fighterz"); /* or something... */

		STARTED = 1;
		our_node->dead = 0;
	} 
}

void m_respawn(unsigned int id)
{
	LINK node = getplayer_byid(id);
	node->dead = 0;
}

void m_hit()
{
	if (our_node->power > 0)
		our_node->power--;
}

void m_accel(unsigned int id, double x, double y, signed char accel, double speed)
{
	LINK current;
	// verbose("SMSG_ACCEL %d %2.2f %2.2f %d %2.2f", id, x, y, (int)accel, speed);
	current = getplayer_byid(id);
	if (!current)
		die("Error in SMSG_ACCEL.."); /* TODO: replace with some error function */

	current->freeze = 0;
	current->id = id;
	current->x = x;
	current->y = y;
	current->velocity = (int)accel;
	current->speed = speed;
	return;
}

void m_turn(unsigned int id, double x, double y, signed char turn, double deg)
{
LINK current;
unsigned int clag;
	current = getplayer_byid(id);
	// verbose("Got player id: %d, got deg: %2.2f", current->id, current->deg);
	if (!current)
		return;

	/* TODO: zit hier iets fout? of niet.. ;P */
	
#if IGNORE_LAG == 1
	clag = 0;
#else
	clag = (unsigned long)current_lag();
#endif
	current->freeze = 0;
	current->t = ourtime - clag;
	current->turn = turn;
	current->turn_t = ourtime - clag;
	current->deg = deg;
	current->x = x;
	current->y = y;
	setsec(current);
}
