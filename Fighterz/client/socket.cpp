#include "common.h"

//Sockets, Communication
SOCKET theSocket; /**< socket descriptor for our connection with server */
double lag[5]; /**< previous 5 lag calculations */
char serv_addr[512]; /**< host/ip of the host server */
int serv_port; /**< port of the host server */
char *buffer; /**< allocated to 8196bytes, serves as a buffer for data rcvd from serv */
int inlen; /**< input buffer length */

/********************************************
*  SOCKET FUNCTIONS
****************************************************/

int connect_socket()
{
	WORD version = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;

	const int just_say_no = 1;

	/* Start up Winsock as before */
	WSAStartup(version, &wsaData);

	/* Store information about the server */
	LPHOSTENT lpHostEntry;
	lpHostEntry = gethostbyname(serv_addr);  /* Specifying server by its name */

	if (lpHostEntry == NULL) {
		printff_direct("Error at gethostbyname()");
		terminate();
		return -1;
	}

	/* Create the socket */
	// socket declared in main.h

	theSocket = socket(AF_INET,	/* Go over TCP/IP */
				SOCK_STREAM,	/* Socket type */
				IPPROTO_TCP); /* Protocol */
	
	if (theSocket == INVALID_SOCKET) {
		printff_direct("Error at socket()");
		//terminate();
		return -1;
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
		// terminate();
		return -1;
	}

	/* Connected */
	printff_direct("Socket succesfully connected..");
	printff_direct("");

	/* Calculating lag and requesting to join the server */
	unsigned long one = 1;
	int pRet = 0;
	  /* make socket non-blocking */
	ioctlsocket(theSocket, FIONBIO, &one);

return 0;
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
		case SMSG_FLAGCARR:
		{
		unsigned int carrier_id;
		unsigned int code;

			if (!get_u32(&carrier_id, &dta, &len))
				goto fatal;
			if (!get_u32(&code, &dta, &len))
				goto fatal;
			
			addtext("flag_carrier id=%lu,code=%lu", carrier_id, code);			
			m_flagcarrier(carrier_id, code);

			return;
		}
		case SMSG_FLAGREST:
		{
		unsigned int restorer_id;
		unsigned int code;

			if (!get_u32(&restorer_id, &dta, &len))
				goto fatal;
			if (!get_u32(&code, &dta, &len))
				goto fatal;
			
			addtext("restorer_id id=%lu,code=%lu", restorer_id, code);			
			m_flagrestorer(restorer_id, code);

			return;
		}
		case SMSG_FLAGCAPT:
		{
		unsigned int capturer_id;
		unsigned int code;
		unsigned int red_team_score;
		unsigned int blue_team_score;

			if (!get_u32(&capturer_id, &dta, &len))
				goto fatal;
			if (!get_u32(&code, &dta, &len))
				goto fatal;
			if (!get_u32(&red_team_score, &dta, &len))
				goto fatal;
			if (!get_u32(&blue_team_score, &dta, &len))
				goto fatal;
			
			addtext("flag_capturer id=%lu,code=%lu,points red=%lu,blue=%lu", capturer_id, code,
				red_team_score, blue_team_score);			
			m_flagcapturer(capturer_id, code, red_team_score, blue_team_score);

			return;
		}
		case SMSG_CLEARFIELD:
		{
			m_clearfield();
			return;
		}
		case SMSG_FIELDLINE:
		{
		unsigned int index;
		char fieldline[512];

			if (!get_u32(&index, &dta, &len))
				goto fatal;
			if (!get_str(fieldline, &dta, &len, sizeof(fieldline)))
				goto fatal;

			m_fieldline(index, fieldline);
			return;
		}
		case SMSG_BLOCKINFO:
		{
		int w, h, size;

			if (!get_s32(&w, &dta, &len))
				goto fatal;
			if (!get_s32(&h, &dta, &len))
				goto fatal;
			if (!get_s32(&size, &dta, &len))
				goto fatal;

			m_blockinfo(w, h, size);
			return;
		}
		case SMSG_BACKGROUND:
		{
		int df_id, pos_x, pos_y;
		char datfile[128];
		
			if (!get_s32(&df_id, &dta, &len))
				goto fatal;
			if (!get_s32(&pos_x, &dta, &len))
				goto fatal;
			if (!get_s32(&pos_y, &dta, &len))
				goto fatal;
			if (!get_str(datfile, &dta, &len, sizeof(datfile)))
				goto fatal;

			m_background(df_id, pos_x, pos_y, datfile);
			return;
		}
		case SMSG_FIELDEND:
		{
			m_fieldend();
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
		case SMSG_NEWSHIP:
		{
			unsigned int id;
			int newshiptype;
			
			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_s32(&newshiptype, &dta, &len))
				goto fatal;

			m_shiptype(id, newshiptype);
			return;
		}
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
			// verbose("SMSG_INVINCIBLE");

			unsigned int id, t;
			unsigned char yesno;

			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_u8(&yesno, &dta, &len))
				goto fatal;
			if (!get_u32(&t, &dta, &len))
				goto fatal;
			
			m_invincible(id, yesno, t);
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
			double x, y, deg, speed;
			signed char accel, turn, team;
			unsigned char type;
			signed short frags;
			int shiptype;
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
			if (!get_s8(&team, &dta, &len))
				goto fatal;
			if (!get_s8(&turn, &dta, &len))
				goto fatal;
			if (!get_u8(&type, &dta, &len))
				goto fatal;
			if (!get_dbl(&speed, &dta, &len))
				goto fatal;
			if (!get_s32(&shiptype, &dta, &len))
				goto fatal;
			if (!get_str(nick, &dta, &len, sizeof(nick)))
				goto fatal;
			
			// verbose("got nick: %s", nick);

			// strcpy(nick, "TEMPPPP");
			addtext("user_id=%d team=%d", id, (int)team);
			
			m_newuser(id, x, y, deg, accel, alive, frags, team, turn, type, speed, shiptype, nick);
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
			unsigned int id, alive;
			double x, y, deg, speed;
			signed char accel, turn;
			unsigned char type;
			short frags;

			// verbose("SMSG_SPAWN");			

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
			if (!get_s8(&turn, &dta, &len))
				goto fatal;
			if (!get_u8(&type, &dta, &len))
				goto fatal;
			if (!get_dbl(&speed, &dta, &len))
				goto fatal;

			m_spawn(id, x, y, deg, accel, alive, frags, turn, type, speed);
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
			unsigned int id;
			char reason[128];

			// verbose("SMSG_KICK");
			if (!get_u32(&id, &dta, &len))
				goto fatal;
			if (!get_str(reason, &dta, &len, sizeof(reason)))
				goto fatal;

			m_kick( id, reason );
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

void m_shiptype(unsigned int id, int newshiptype)
{
	PLAYER node = getplayer_byid( id );
	node->shiptype = newshiptype;
	//addtext("id new type: %d %d", node->id, newshiptype);
}

void m_ping(unsigned int servertime)
{
	// verbose("SMSG_PING: %d", servertime);
	send_pong(servertime);
}

void m_lag(double diff)
{
//	if (!game_started && our_spawnstatus == 0)
		//play_sample((SAMPLE *)dat_sound[TUSCH].dat, 255, 128, 1000, 0);
	int i = 0;
	while (i < 4)
	{
		lag[i] = lag[i+1];
		i++;
	}
	lag[i] = diff;

	printff_direct("smsg_lag diff=%f avg_lag=%2.2f OK", diff, current_lag());
}

void m_clearfield()
{
	verbose("Map download: Started");
	bg_imgs_data_index = 0;
}

void m_nickreply(unsigned char acceptance, char *msg)
{
	verbose("SMSG_NICKREPLY: %d %s", (int)acceptance, msg);
}

void m_flagcarrier(unsigned int carrier_id, unsigned int code)
{
PLAYER lnk = getplayer_byid( carrier_id );

	if (NULL == lnk)
	{
		addtext("carrier_id=%d THATS NOT A VALID ID!", carrier_id);
		return;
	}

	if (code == 1)
		red_flag_carrier = carrier_id;
	else
		blue_flag_carrier = carrier_id;

	large_text("%s has the %s flag!", lnk->nick, code == 1 ? "red" : "blue");

}

void m_flagrestorer(unsigned int restorer_id, unsigned int code)
{
PLAYER lnk = getplayer_byid( restorer_id );

	if (NULL == lnk)
	{
		addtext("restorer_id=%d THATS NOT A VALID ID!", restorer_id);
		return;
	}

	if (code == 1)
	{
		red_flag_x = red_flag_def_x;
		red_flag_y = red_flag_def_y;
	} else {
		blue_flag_x = blue_flag_def_x;
		blue_flag_y = blue_flag_def_y;
	}

	large_text("%s restored the %s flag!", lnk->nick, code == 1 ? "red" : "blue");
}

void m_flagcapturer(unsigned int capturer_id, unsigned int code, 
					unsigned int red_team_score, unsigned int blue_team_score)
{
PLAYER lnk = getplayer_byid( capturer_id );

	if (NULL == lnk)
	{
		addtext("capturer_id=%d THATS NOT A VALID ID!", capturer_id);
		return;
	}

	if (code == 1)
	{
		red_flag_carrier = -1;
		red_flag_x = red_flag_def_x;
		red_flag_y = red_flag_def_y;
	} else {
		blue_flag_carrier = -1;
		blue_flag_x = blue_flag_def_x;
		blue_flag_y = blue_flag_def_y;
	}
	
	large_text("%s captured the %s flag!", lnk->nick, code == 1 ? "red" : "blue");
}

void m_blockinfo(int w, int h, int size)
{
	map_blocks_y = h;
	map_blocks_x = w;
	blocksize = size; // 20 anyways *sigh*

//
	field_width = map_blocks_x * blocksize;
	field_height = map_blocks_y * blocksize;

//

	bmp_console = create_sub_bitmap(tmpscreen, console_x, console_y, console_w, console_h);
	clear_to_color(bmp_console, makecol(0,0,0));
	

	bmp_radar = create_sub_bitmap(tmpscreen, radar_x, radar_y, radar_w, radar_h);
	
	bmp_radar_display = create_sub_bitmap(bmp_radar, radar_field_x, radar_field_y, radar_field_w, radar_field_h);

	draw_sprite(bmp_radar, (BITMAP *)dat_base[GUI_RADAR_BG].dat, 0, 0);

	destroy_bitmap(bmp_mapfield);
	bmp_shipfield = create_sub_bitmap(tmpscreen, field_x, field_y, field_w, field_h);
	
	destroy_bitmap(bmp_command);
	bmp_command = create_sub_bitmap(tmpscreen, command_x, command_y, command_w, command_h);
	
	destroy_bitmap(bmp_scoreboard);
	bmp_scoreboard = create_sub_bitmap(tmpscreen, scoreboard_x, scoreboard_y, scoreboard_w, scoreboard_h);//

	printff_direct("  Vertical blockcount: %d", map_blocks_y);
	printff_direct("  Horizontal blockcount: %d", map_blocks_x);
	printff_direct("  Blocksize in pixels: %d", blocksize);
}
void m_fieldline(unsigned int index, char *fieldline)
{
	// verbose("map: %s", fieldline);
	strcpy(field[index], fieldline);
}

void m_background(int df_id, int pos_x, int pos_y, char *datfile)
{
	bg_imgs_data[bg_imgs_data_index].df_id = df_id;
	bg_imgs_data[bg_imgs_data_index].pos_x = pos_x;
	bg_imgs_data[bg_imgs_data_index].pos_y = pos_y;
	if (strlen(datfile) > 128)
		datfile[127] = '\0';

	strcpy(bg_imgs_data[bg_imgs_data_index].datfile, datfile);

	bg_imgs_data_index++;
}
void m_fieldend()
{
	verbose("Map download: Completed");
	draw_map();
	game_started = 1;
	large_text("Map download complete.");

	// I'd like to view our spawn location please instead of 0,0
	x_on_map = red_spawn_x - field_w / 2;
	y_on_map = red_spawn_y - field_h / 2;
}
void m_kick( unsigned int id, char *reason )
{
	PLAYER node = getplayer_byid( id );

	node->velocity = 0;
	node->speed = 0;
	node->dead = 1;

	if (node == our_node)
	{
		// alert("You were kicked for:", reason, "Press [OK] to exit", "", "", 1, 1);
		// alert("We were kicked:", "", reason, "OK", NULL, KEY_ENTER, 1);
		large_text("KICKED!: %s", reason);
	} else {
		addtext("*** %s was kicked [%s]", node->nick, reason);
	}

	add_explosion(node->x, node->y, 250, 10, makecol(255,0,0));
	
	//flagstuff
	if (red_flag_carrier == node->id)
	{
		red_flag_carrier = -1;
		red_flag_x = node->x;
		red_flag_y = node->y;
	}
	if (blue_flag_carrier == node->id)
	{
		blue_flag_carrier = -1;
		blue_flag_x = node->x;
		blue_flag_y = node->y;
	}
}

void m_quit(unsigned int id, char *quit_msg)
{
	PLAYER current = getplayer_byid(id);
	if (current == NULL)
		die("m_quit(): current cannot be NULL");

	add_explosion(current->x, current->y, 500, 10, makecol(255, 128, 255));
	
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
	
	//flagstuff
	if (red_flag_carrier == current->id)
	{
		red_flag_carrier = -1;
		red_flag_x = current->x;
		red_flag_y = current->y;
	}
	if (blue_flag_carrier == current->id)
	{
		blue_flag_carrier = -1;
		blue_flag_x = current->x;
		blue_flag_y = current->y;
	}
}

void m_spawnready()
{
	verbose("SMSG_SPAWNREADY");
	our_node = getplayer_byid(our_id);
	our_spawnstatus = 1;
	our_spawnrequested = 0;
	our_node->dead = 2;

//flagstuff *snip*

	set_window_title("Fighterz -- Hit fire to respawn");
	set_map_coords();
	large_text("Received ship - Fire to spawn!");
}

// *lag*
void m_newuser(int id, double x, double y, double deg,
	signed char accel, unsigned int alive, signed short frags, signed char team,
	signed char turn, unsigned char type, double speed, int shiptype,
	char *nick)
{
	unsigned long clag;
/*	verbose("---------------------- adding new user ----------------------");
	verbose("id: %d x: %2.2f y: %2.2f deg: %2.2f accel: %d, alive: %d", 
	         id, x, y, deg, (int)accel, alive);
	verbose("frags: %d turn: %d type: %d speed: %2.2f nick: %s", (int)frags,
	         (int)turn, (int)type, speed, nick);
	verbose("-------------------------------------------------------------");
*/
	/* create first node in linked list */
	if (!(new_node = (PLAYER)malloc(sizeof(user))))
	{
		printff_direct("Error allocating memory");
		exit(-1);
	}
	/* insert the created node */
	new_node->next = head;
	head = new_node;
	/* intiate some values */
	head->id = id;
	head->dead = 2; /* spawnready ofzo? */
	
	if (type == T_BOT) /* TODO: change all these seperate */
		head->bot = 1; /* fucking variables to one named type */
	else
		head->bot = 0;

	if (type == T_BULLET)
		head->bullet = 1;
	else
		head->bullet = 0;

	head->power = ship_maxpower;
	head->TTL = 0;
	head->vel_time = ourtime;
	head->x = x;
	head->y = y;

	head->deg = deg;
#if IGNORE_LAG == 1
	clag = 0;
#else
	clag = (unsigned long)current_lag();
#endif
	head->t = ourtime - clag;
	head->move = 0;
	head->freeze = 0;
	head->turn = turn;
	head->turn_t = ourtime; /* CHK */
	head->velocity = accel;
	head->speed = speed;
	head->max_speed = (head->bot == 1 ? 0.10 : 0.20);
	// not yet used: head->alive = alive; /* seconds in the game*/
	head->kills = frags;

	head->invincible = 0;
	head->invincibility_t = 0;
	head->invincibility_t2 = ourtime;
	head->hit_t = ourtime;
	head->impact = 0;
	head->shiptype = shiptype;
	head->team = team;
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
	PLAYER evil_node = getplayer_byid(evilid);
	PLAYER victim_node = getplayer_byid(victimid);

	if (evil_node != NULL && victim_node != NULL)
	{
		victim_node->velocity = 0;
		victim_node->speed = 0.0;
		victim_node->dead = 1;
		victim_node->power = ship_maxpower;
		
	

		evil_node->kills++;
		addtext("*** %s(%d) fragged %s(%d)", evil_node->nick, evil_node->id,
			victim_node->nick, victim_node->id);
		
		add_explosion(victim_node->x, victim_node->y, 250, 10, makecol(255,0,0));
	} else {
		die("evil_node or victim_node is NULL");
	}

	//flagstuff
	if (red_flag_carrier == victim_node->id)
	{
		red_flag_carrier = -1;
		red_flag_x = victim_node->x;
		red_flag_y = victim_node->y;
	}
	if (blue_flag_carrier == victim_node->id)
	{
		blue_flag_carrier = -1;
		blue_flag_x = victim_node->x;
		blue_flag_y = victim_node->y;
	}
}

// *lag*
void m_newbullet(unsigned int id, unsigned int ownerid, double x, double y, double deg)
{
	unsigned long clag;

#if IGNORE_LAG == 1
	clag = 0;
#else
	clag = (unsigned long)current_lag();
#endif
	// *the entire `unsigned int id` var isn't used
	
	//verbose("New bullet id: [!], ownerid: %d, x: %2.2f, y: %2.2f, deg: %2.2f\n",
	//	ownerid, x, y, deg);

	if (ownerid != our_id) 
		play_sample((SAMPLE *)dat_sound[SHOOT].dat, 50, 128, 1000, 0);

	add_bullet( getplayer_byid(ownerid), (int)x, (int)y, (double)deg, ourtime - clag);

	return;
}

void m_nick(unsigned int id, char *nick)
{
	PLAYER node = getplayer_byid(id);
	if (strlen(nick) > 64)
		*(nick + 63) = '\0';
	verbose("*** Nickchange: %s -> %s", node->nick, nick);
	strcpy(node->nick, nick);
}

void m_say(unsigned int id, char *msg)
{
	// verbose("m_say");
	PLAYER node = getplayer_byid(id);
	addtext("<[%d]%s> %s", id, node->nick, msg);
}

// void m_spawn(unsigned int oid)
void m_spawn(unsigned int id, double x, double y, double deg, signed char accel, 
	unsigned int alive, short frags, signed char turn, unsigned char type, double speed)
{
/*	addtext("************************************");
	addtext("spwn: id: %u, x: %2.2f, y: %2.2f", id, x, y);
	addtext("spwn: deg: %2.2f, accel: %d, alive: %u", deg, accel, alive);
	addtext("spwn: frags: %d, turn: %d, type: %d", (int)frags, (int)accel, (int)alive);
	addtext("spwn: speed: %2.2f", speed);
	addtext("************************************");
*/
	if (id == our_id)
	{
		/* sync our_node */
		for (our_node=head; our_node->id!=our_id; our_node=our_node->next);

		if (!our_node)
			die("Failed synching our_node with our_id");
		
		set_window_title("Fighterz"); /* or something... */

		game_started = 1;
	} 

	PLAYER node = getplayer_byid( id );
	
	if (!node)
	{
		return;
	}

	node->x = x;
	node->y = y;
	node->deg = deg;
	node->velocity = accel;
	node->alive = alive;
	node->kills = frags;
	node->turn = turn;
	// node->type ?
	node->speed = speed;
	node->dead = 0;

	play_sample((SAMPLE *)dat_sound[SPAWN].dat, 100, 128, 1000, 0);
}

void m_respawn(unsigned int id)
{
	PLAYER node = getplayer_byid(id);
	node->dead = 0;
}

void m_hit()
{
	if (our_node->power > 0)
		our_node->power--;
}

// *lag*
void m_accel(unsigned int id, double x, double y, signed char accel, double speed)
{
	PLAYER current;
	unsigned long clag;
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
#if IGNORE_LAG == 1
	clag = 0;
#else
	clag = (unsigned long)current_lag();
#endif
	current->t = ourtime - clag;

	return;
}

void m_invincible(unsigned int id, unsigned char yesno, unsigned int t)
{
	PLAYER node = getplayer_byid(id);
	int clag;

	#if IGNORE_LAG == 1
		clag = 0;
	#else
		clag = current_lag();
	#endif
			
	node->invincible = yesno;
	node->invincibility_t = t;
	node->invincibility_t2 = ourtime + clag;
}

// *lag*
void m_turn(unsigned int id, double x, double y, signed char turn, double deg)
{
PLAYER current;
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
}
