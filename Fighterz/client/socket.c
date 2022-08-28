#include "common.h"
//refactored: 12 dec 2004


//functions:
//--
// int socket_connect();
// void socket_read();;
// void dopacket(int xtype, unsigned short len, char *dta);
// void m_hi(unsigned int sver, unsigned int oid);
// void m_shiptype(unsigned int id, int newshiptype);
// void m_ping(unsigned int servertime);
// void m_lag(double diff);
// void m_clearfield();
// void m_nickreply(unsigned char acceptance, char *msg);
// void m_flagcarrier(unsigned int carrier_id, unsigned int code);
// void m_flagrestorer(unsigned int restorer_id, unsigned int code);
// void m_flagcapturer(unsigned int capturer_id, unsigned int code,
//					 unsigned int red_team_score, unsigned int blue_team_score);
// void m_blockinfo(int w, int h, int size);
// void m_fieldline(unsigned int index, char *fieldline);
// void m_background(int df_id, int pos_x, int pos_y, char *datfile);
// void m_fieldend();
// void m_kick( unsigned int id, char *reason );
// void m_quit(unsigned int id, char *quit_msg);
// void m_spawnready();
// void m_newuser(int id, double x, double y, double deg,
// 	signed char accel, unsigned int alive, signed short frags, signed char team,
// 	signed char turn, unsigned char type, double speed, int shiptype,
// 	char *nick);
// void m_kill(unsigned int victimid, unsigned int evilid, char *killstr);
// void m_newbullet(unsigned int id, unsigned int ownerid, double x, double y, double deg);
// void m_nick(unsigned int id, char *nick);
// void m_say(unsigned int id, char *msg);
// void m_spawn(unsigned int id, double x, double y, double deg, signed char accel, 
// 	unsigned int alive, short frags, signed char turn, unsigned char type, double speed);
// void m_respawn(unsigned int id);
// void m_hit();
// void m_accel(unsigned int id, double x, double y, signed char accel, double speed);
// void m_invincible(unsigned int id, unsigned char yesno, unsigned int t);
// void m_turn(unsigned int id, double x, double y, signed char turn, double deg);

//variables:
//--

//Sockets, Communication
SOCKET sock_to_serv; /**< socket descriptor for our connection with server */
double lag[5];		 /**< previous 5 lag calculations */
char serv_addr[512]; /**< [host|ip] of the server */
int serv_port;		 /**< port of the host server */
char *buffer;		 /**< allocated to 8196bytes, serves as a buffer for data rcvd from serv */
int inlen;			 /**< input buffer length */


#define RETURN_ERROR(str) \
	do { \
		printff_direct(str); \
		return -1; \
	} while (0)

#ifdef _WIN32
int socket_connect()
{
WORD version = MAKEWORD(1,1);
WSADATA wsaData;
int nRet;
const int just_say_no = 1;

	//Initialize
	WSAStartup(version, &wsaData);

	//Store information about the server
	LPHOSTENT lpHostEntry;
	//Specifying server by its name
	lpHostEntry = gethostbyname(serv_addr); 

	if (lpHostEntry == NULL)
		RETURN_ERROR("Error at gethostbyname()");

	//Create the socket
	sock_to_serv = socket(AF_INET,		/* Go over TCP/IP	*/
						  SOCK_STREAM,	/* Socket type		*/
						  IPPROTO_TCP); /* Protocol			*/

	if (sock_to_serv == INVALID_SOCKET)
		RETURN_ERROR("Error at socket()");

	
	//Use SOCKADDR_IN to fill in address information
	SOCKADDR_IN saServer;	
	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
			  // ^ Address of the server being inserted into the address field
	saServer.sin_port = htons(8099);

	if (setsockopt(sock_to_serv, IPPROTO_TCP, TCP_NODELAY, (char*)&just_say_no,
														sizeof just_say_no) == -1)
	{	
		printff_direct("setsockopt");
		terminate();
	}

	//Connect to the server
	nRet = connect(sock_to_serv,
				   (LPSOCKADDR)&saServer,	 /* Server address */
				   sizeof(struct sockaddr)); /* Length of address structure */

	if (nRet == SOCKET_ERROR)
		RETURN_ERROR("Error at connect()");

	//Connected!
	printff_direct("Socket succesfully connected..");
	printff_direct("");

	//make socket non-blocking
	//--
	unsigned long one = 1;
	int pRet = 0;
	ioctlsocket(sock_to_serv, FIONBIO, &one);
	//--

	return 0;
}

void socket_read()
{
char buf[8192], tmpbuf[4096], *p = buf;
int r, len;
unsigned short plen;
unsigned long one = 1;

	ioctlsocket(sock_to_serv, FIONREAD, &one);

	if (one > 0)
	{
		r = recv(sock_to_serv, tmpbuf, sizeof(tmpbuf),	0);

		if (r == SOCKET_ERROR)
			die("Error on recv()");

		memcpy(buf, buffer, inlen);
		memcpy(buf + inlen, tmpbuf, r);
		len = r + inlen;

		memcpy(buf, buffer, inlen);
		memcpy(buf + inlen, tmpbuf, r);
		len = r + inlen;

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
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int socket_connect()
{
	int nRet;
	const int just_say_no = 1;

	//Initialize

	//Store information about the server
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	// TODO: check return
	const char *serv_port_str = "8099"; // temp hack to see if stuff works
	getaddrinfo(serv_addr, serv_port_str, &hints, &res);

//	if (lpHostEntry == NULL)
//		RETURN_ERROR("Error at gethostbyname()");


	//Create the socket
	sock_to_serv = socket(AF_INET,		/* Go over TCP/IP	*/
						  SOCK_STREAM,	/* Socket type		*/
						  IPPROTO_TCP); /* Protocol			*/

						  // TODO: check socke
	if (sock_to_serv == -1)
		RETURN_ERROR("Error at socket()");


	//Use SOCKADDR_IN to fill in address information
//	SOCKADDR_IN saServer;
//	saServer.sin_family = AF_INET;
//	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
//	// ^ Address of the server being inserted into the address field
//	saServer.sin_port = htons(8099);

// TODO: TCP_NODELAY set doesn't work?
//	if (setsockopt(sock_to_serv, IPPROTO_TCP, NULL, (char*)&just_say_no,
//				   sizeof just_say_no) == -1)
//	{
//		printff_direct("setsockopt");
//		terminate();
//	}

	nRet = connect(sock_to_serv, res->ai_addr, res->ai_addrlen);

	if (nRet == -1)
		RETURN_ERROR("Error at connect()");

	//Connected!
	printff_direct("Socket succesfully connected..");
	printff_direct("");

	//make socket non-blocking
	//--
	unsigned long one = 1;
	int pRet = 0;
	fcntl(sock_to_serv, F_SETFL, O_NONBLOCK);

	//--

	return 0;
}

void socket_read()
{
	char buf[8192], tmpbuf[4096], *p = buf;
	int r, len;
	unsigned short plen;
	unsigned long one = 1;

	// ioctlsocket(sock_to_serv, FIONREAD, &one);

	if (one > 0)
	{
		r = recv(sock_to_serv, tmpbuf, sizeof(tmpbuf),	0);

		if (r == -1) {
			return;//
//			int e = errno;
//
//			die("Error on recv()");
		}

		memcpy(buf, buffer, inlen);
		memcpy(buf + inlen, tmpbuf, r);
		len = r + inlen;

		memcpy(buf, buffer, inlen);
		memcpy(buf + inlen, tmpbuf, r);
		len = r + inlen;

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
#endif

void dopacket(int xtype, unsigned short len, char *dta)
{
	/* Msgs which are allowed if unregistered */
	switch(xtype)
	{
		case SMSG_HI:
		{
		unsigned int sver, oid;
			if (!get_u32(&sver, &dta, &len)) goto fatal;
			if (!get_u32(&oid, &dta, &len)) goto fatal;
			m_hi(sver, oid);
			return;
		}
		case SMSG_MOTD: { break; }
		case SMSG_PING:
		{
		unsigned int servertime;
			if (!get_u32(&servertime, &dta, &len)) goto fatal;	
			m_ping(servertime);
			return;
		}
		case SMSG_LAG:
		{
		signed int diff;
			if (!get_s32(&diff, &dta, &len)) goto fatal;
			m_lag((double)diff);
			return;
		}
		case SMSG_SPAWNREADY: { m_spawnready(); return; }
		case SMSG_NICKREPLY:
		{
		signed char acceptance;
		char str[256];
			if (!get_s8(&acceptance, &dta, &len)) goto fatal;
			if (!get_str(str, &dta, &len, sizeof(str))) goto fatal;
			m_nickreply(acceptance, str);
			return;
		}
		case SMSG_FLAGCARR:
		{
		unsigned int carrier_id;
		unsigned int code;
			if (!get_u32(&carrier_id, &dta, &len)) goto fatal;
			if (!get_u32(&code, &dta, &len)) goto fatal;
			m_flagcarrier(carrier_id, code);
			return;
		}
		case SMSG_FLAGREST:
		{
		unsigned int restorer_id;
		unsigned int code;
			if (!get_u32(&restorer_id, &dta, &len)) goto fatal;
			if (!get_u32(&code, &dta, &len)) goto fatal;
			m_flagrestorer(restorer_id, code);
			return;
		}
		case SMSG_FLAGCAPT:
		{
		unsigned int capturer_id;
		unsigned int code;
		unsigned int red_team_score;
		unsigned int blue_team_score;
			if (!get_u32(&capturer_id, &dta, &len)) goto fatal;
			if (!get_u32(&code, &dta, &len)) goto fatal;
			if (!get_u32(&red_team_score, &dta, &len)) goto fatal;
			if (!get_u32(&blue_team_score, &dta, &len)) goto fatal;
			addtext("flag_capturer id=%lu,code=%lu,points red=%lu,blue=%lu", 
					capturer_id, code, red_team_score, blue_team_score);
			m_flagcapturer(capturer_id, code, red_team_score, blue_team_score);
			return;
		}
		case SMSG_CLEARFIELD: { m_clearfield(); return; }
		case SMSG_FIELDLINE:
		{
		unsigned int index;
		char fieldline[512];
			if (!get_u32(&index, &dta, &len)) goto fatal;
			if (!get_str(fieldline, &dta, &len, sizeof(fieldline))) goto fatal;
			m_fieldline(index, fieldline);
			return;
		}
		case SMSG_BLOCKINFO:
		{
		int w, h, size;
			if (!get_s32(&w, &dta, &len)) goto fatal;
			if (!get_s32(&h, &dta, &len)) goto fatal;
			if (!get_s32(&size, &dta, &len)) goto fatal;
			m_blockinfo(w, h, size);
			return;
		}
		case SMSG_BACKGROUND:
		{
		int df_id, pos_x, pos_y;
		char datfile[128];		
			if (!get_s32(&df_id, &dta, &len)) goto fatal;
			if (!get_s32(&pos_x, &dta, &len)) goto fatal;
			if (!get_s32(&pos_y, &dta, &len)) goto fatal;
			if (!get_str(datfile, &dta, &len, sizeof(datfile))) goto fatal;
			m_background(df_id, pos_x, pos_y, datfile);
			return;
		}
		case SMSG_FIELDEND: { m_fieldend(); return; }
		default:
			break;
	}

	//Other msgs here
	switch(xtype)
	{
		case SMSG_NEWSHIP:
		{
		unsigned int id;
		int newshiptype;			
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_s32(&newshiptype, &dta, &len)) goto fatal;
			m_shiptype(id, newshiptype);
			return;
		}
		case SMSG_ACCEL:
		{
		unsigned int id;
		double x, y, speed;
		signed char accel;			
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_dbl(&x, &dta, &len)) goto fatal;
			if (!get_dbl(&y, &dta, &len)) goto fatal;
			if (!get_s8(&accel, &dta, &len)) goto fatal;
			if (!get_dbl(&speed, &dta, &len)) goto fatal;
			m_accel(id, x, y, accel, speed);
			return;
		}
		case SMSG_TURN:
		{
		unsigned int id;
		double x, y, deg;
		signed char turn;
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_dbl(&x, &dta, &len)) goto fatal;
			if (!get_dbl(&y, &dta, &len)) goto fatal;
			if (!get_s8(&turn, &dta, &len)) goto fatal;
			if (!get_dbl(&deg, &dta, &len)) goto fatal;
			m_turn(id, x, y, turn, deg);
			return;
		}
		case SMSG_INVINCIBLE:
		{
		unsigned int id, t;
		unsigned char yesno;
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_u8(&yesno, &dta, &len)) goto fatal;
			if (!get_u32(&t, &dta, &len)) goto fatal;
			m_invincible(id, yesno, t);
			return;
		}
		case SMSG_HIT: { m_hit(); return; }
		case SMSG_SAY:
		{
		unsigned int id;
		char msg[512];
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_str(msg, &dta, &len, sizeof(msg))) goto fatal;
			m_say(id, msg);
			return;
		}
		case SMSG_TEAMSAY: { return; }
		case SMSG_NEWUSER:
		{
		unsigned int id;
		unsigned int alive;
		double x, y, deg, speed;
		signed char accel, turn, team;
		unsigned char type;
		signed short frags;
		int shiptype;
		char nick[64];
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_dbl(&x, &dta, &len)) goto fatal;
			if (!get_dbl(&y, &dta, &len)) goto fatal;
			if (!get_dbl(&deg, &dta, &len)) goto fatal;
			if (!get_s8(&accel, &dta, &len)) goto fatal;
			if (!get_u32(&alive, &dta, &len)) goto fatal;
			if (!get_s16(&frags, &dta, &len)) goto fatal;
			if (!get_s8(&team, &dta, &len)) goto fatal;
			if (!get_s8(&turn, &dta, &len)) goto fatal;
			if (!get_u8(&type, &dta, &len)) goto fatal;
			if (!get_dbl(&speed, &dta, &len)) goto fatal;
			if (!get_s32(&shiptype, &dta, &len)) goto fatal;
			if (!get_str(nick, &dta, &len, sizeof(nick))) goto fatal;
			addtext("user_id=%d team=%d", id, (int)team);			
			m_newuser(id, x, y, deg, accel, alive, frags, team, 
					  turn, type, speed, shiptype, nick);
			return;
		}
		case SMSG_DELOBJ: { return; }
		case SMSG_KILL:
		{
		unsigned int victimid, evilid;
		char *killstr = strdup("");
			if (!get_u32(&victimid, &dta, &len)) goto fatal;
			if (!get_u32(&evilid, &dta, &len)) goto fatal;
			if (!get_str(killstr, &dta, &len, sizeof(killstr))) goto fatal;
			m_kill(victimid, evilid, killstr);
			return;
		}
		case SMSG_NEWBULLET:
		{
		unsigned int id, ownerid;
		double x, y, deg;
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_u32(&ownerid, &dta, &len)) goto fatal;
			if (!get_dbl(&x, &dta, &len)) goto fatal;
			if (!get_dbl(&y, &dta, &len)) goto fatal;
			if (!get_dbl(&deg, &dta, &len)) goto fatal;
			m_newbullet(id, ownerid, x, y, deg);
			return;
		}
		case SMSG_NICK:
		{
		unsigned int id;
		char nick[64];
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_str(nick, &dta, &len, sizeof(nick))) goto fatal;
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
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_dbl(&x, &dta, &len)) goto fatal;
			if (!get_dbl(&y, &dta, &len)) goto fatal;
			if (!get_dbl(&deg, &dta, &len)) goto fatal;
			if (!get_s8(&accel, &dta, &len)) goto fatal;
			if (!get_u32(&alive, &dta, &len)) goto fatal;
			if (!get_s16(&frags, &dta, &len)) goto fatal;
			if (!get_s8(&turn, &dta, &len)) goto fatal;
			if (!get_u8(&type, &dta, &len)) goto fatal;
			if (!get_dbl(&speed, &dta, &len)) goto fatal;
			m_spawn(id, x, y, deg, accel, alive, frags, turn, type, speed);
			return;
		}
		case SMSG_RESPAWN:
		{
		unsigned int oid;
			if (!get_u32(&oid, &dta, &len)) goto fatal;
			m_respawn(oid);
			return;
		}
		case SMSG_QUIT:
		{
		unsigned int id;
		char quit_msg[64];
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_str(quit_msg, &dta, &len, sizeof(quit_msg))) goto fatal;
			m_quit(id, quit_msg);
			return;
		}
		case SMSG_KICK:
		{
		unsigned int id;
		char reason[128];
			if (!get_u32(&id, &dta, &len)) goto fatal;
			if (!get_str(reason, &dta, &len, sizeof(reason))) goto fatal;
			m_kick( id, reason );
			return;
		}
	}

	printff_direct("Ignored type: 0x%x", xtype);
	return;

fatal:
	die("Packet error");
}

void m_hi(unsigned int sver, unsigned int oid)
{
	our_id = oid;
	send_version();
	send_nickname();
}

void m_shiptype(unsigned int id, int newshiptype)
{
PLAYER node = getplayer_byid(id);
	node->shiptype = newshiptype;
}

void m_ping(unsigned int servertime)
{
	send_pong(servertime);
}

void m_lag(double diff)
{
int i = 0;
	while (i < 4) {
		lag[i] = lag[i + 1];
		i++;
	}

	lag[i] = diff;
	printff_direct("smsg_lag diff=%f avg_lag=%2.2f OK", 
		diff, current_lag());
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
	//Preconditions
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
	//Preconditions
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
	//Preconditions
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
	destroy_bitmap(bmp_mapfield);
	destroy_bitmap(bmp_command);
	destroy_bitmap(bmp_scoreboard);

	map_blocks_y		= h;
	map_blocks_x		= w;
	blocksize			= size; // [TODO] remove this ""variable""
	field_width			= map_blocks_x * blocksize;
	field_height		= map_blocks_y * blocksize;
	
	bmp_console			= create_sub_bitmap(tmpscreen, console_x, console_y, console_w, console_h);
	bmp_radar			= create_sub_bitmap(tmpscreen, radar_x, radar_y, radar_w, radar_h);
	bmp_radar_display	= create_sub_bitmap(bmp_radar, radar_field_x, radar_field_y, radar_field_w, radar_field_h);
	bmp_shipfield		= create_sub_bitmap(tmpscreen, field_x, field_y, field_w, field_h);
	bmp_command			= create_sub_bitmap(tmpscreen, command_x, command_y, command_w, command_h);	
	bmp_scoreboard		= create_sub_bitmap(tmpscreen, scoreboard_x, scoreboard_y, scoreboard_w, scoreboard_h);

	clear_to_color(bmp_console, makecol(0,0,0));
	draw_sprite(bmp_radar, (BITMAP *)dat_base[GUI_RADAR_BG].dat, 0, 0);

	printff_direct("  Vertical blockcount: %d", map_blocks_y);
	printff_direct("  Horizontal blockcount: %d", map_blocks_x);
	printff_direct("  Blocksize in pixels: %d", blocksize);
}

void m_fieldline(unsigned int index, char *fieldline)
{
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
}

void m_kick( unsigned int id, char *reason )
{
PLAYER node = getplayer_byid( id );

	node->velocity  = 0;
	node->speed		= 0;
	node->dead		= 1;

	if (node == our_node)
		large_text("KICKED!: %s", reason);
	else
		addtext("*** %s was kicked [%s]", node->nick, reason);

	add_explosion(node->x, node->y, 250, 10, makecol(255,0,0));
	
	//Flagstuff [todo: make macro]
	//--
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
	//--
}

void m_quit(unsigned int id, char *quit_msg)
{
PLAYER current = getplayer_byid(id);
	//Preconditions
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
	}
	
	//Flagstuff [todo: make macro]
	//--
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
	//--
}

void m_spawnready()
{
	our_node			= getplayer_byid(our_id);
	our_spawnstatus		= 1;
	our_spawnrequested  = 0;
	our_node->dead		= 2;

	//View our spawn location
	if (our_node->team == 1) // red
	{
		x_on_map = red_spawn_x - field_w / 2;
		y_on_map = red_spawn_y - field_h / 2;
	} else { // blue
		x_on_map = blue_spawn_x - field_w / 2;
		y_on_map = blue_spawn_y - field_h / 2;
	}
	set_map_coords();
	
	set_window_title("Fighterz -- Hit fire to respawn");
	large_text("Received ship - Fire to spawn!");
}

void m_newuser(int id, double x, double y, double deg,
	signed char accel, unsigned int alive, signed short frags, signed char team,
	signed char turn, unsigned char type, double speed, int shiptype,
	char *nick)
{
unsigned long clag;

	//	verbose("---------------------- adding new user ----------------------");
	//	verbose("id: %d x: %2.2f y: %2.2f deg: %2.2f accel: %d, alive: %d", 
	//	         id, x, y, deg, (int)accel, alive);
	//	verbose("frags: %d turn: %d type: %d speed: %2.2f nick: %s", (int)frags,
	//	         (int)turn, (int)type, speed, nick);
	//	verbose("-------------------------------------------------------------");

	//Create new linked list node
	if (!(new_node = (PLAYER)malloc(sizeof(user))))
		die("Error allocating memory");

	//Link created node
	new_node->next = head;
	head = new_node;
	
	//Initialize
	clag = (unsigned long)current_lag();

	head->id				= id;
	head->dead				= 2; //spectator	
	//[TODO] we need one phucing type 
	//instead of booleans for each specialization
	head->bot				= (type == T_BOT) ? 1 : 0;
	head->bullet			= (type == T_BULLET) ? 1 : 0;
	head->power				= ship_maxpower;
	head->TTL				= 0;
	head->vel_time			= ourtime;
	head->x					= x;
	head->y					= y;
	head->deg				= deg;	
	head->t					= ourtime - clag;
	head->move				= 0;
	head->freeze			= 0;
	head->turn				= turn;
	head->turn_t			= ourtime;
	head->velocity			= accel;
	head->speed				= speed;
	head->max_speed			= (head->bot == 1 ? 0.10 : 0.20);
	head->kills				= frags;
	head->invincible		= 0;
	head->invincibility_t	= 0;
	head->invincibility_t2	= ourtime;
	head->hit_t				= ourtime;
	head->impact			= 0;
	head->shiptype			= shiptype;
	head->team				= team;

	strcpy(head->nick, nick);
}

void m_kill(unsigned int victimid, unsigned int evilid, char *killstr)
{
PLAYER evil_node = getplayer_byid(evilid);
PLAYER victim_node = getplayer_byid(victimid);

	if (evil_node != NULL && victim_node != NULL)
	{
		victim_node->velocity = 0;
		victim_node->speed	  = 0.0;
		victim_node->dead     = 1;
		victim_node->power    = ship_maxpower;

		evil_node->kills++;
		addtext("*** %s(%d) fragged %s(%d)", evil_node->nick, evil_node->id,
			victim_node->nick, victim_node->id);
		add_explosion(victim_node->x, victim_node->y, 250, 10, 
			makecol(255,0,0));

	} else {
		die("evil_node or victim_node is NULL");
	}

	//Flaggstuff
	//--
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
	//--
}

void m_newbullet(unsigned int id, unsigned int ownerid, double x, double y, double deg)
{
unsigned long clag = (unsigned long)current_lag();
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
PLAYER node = getplayer_byid(id);
	addtext("<[%d]%s> %s", id, node->nick, msg);
}

void m_spawn(unsigned int id, double x, double y, double deg, signed char accel, 
	unsigned int alive, short frags, signed char turn, unsigned char type, double speed)
{
PLAYER node = getplayer_byid( id );
	//	addtext("************************************");
	//	addtext("spwn: id: %u, x: %2.2f, y: %2.2f", id, x, y);
	//	addtext("spwn: deg: %2.2f, accel: %d, alive: %u", deg, accel, alive);
	//	addtext("spwn: frags: %d, turn: %d, type: %d", (int)frags, (int)accel, (int)alive);
	//	addtext("spwn: speed: %2.2f", speed);
	//	addtext("************************************");
	
	//Preconditions
	if (!node)
		die("Invalid id for node in m_spawn");

	if (id == our_id) 
	{
		our_node = node;
		set_window_title("Fighterz");
		game_started = 1;
	}

	node->x			= x;
	node->y			= y;
	node->deg		= deg;
	node->velocity	= accel;
	node->alive		= alive;
	node->kills		= frags;
	node->turn		= turn;
	node->speed		= speed;
	node->dead		= 0;

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
PLAYER current = getplayer_byid(id);
unsigned long clag;

	//Preconditions	
	if (!current)
		die("current is null in m_accel");

	current->freeze		= 0;
	current->id			= id;
	current->x			= x;
	current->y			= y;
	current->velocity	= (int)accel;
	current->speed		= speed;
	clag				= (unsigned long)current_lag();
	current->t			= ourtime - clag;

	return;
}

void m_invincible(unsigned int id, unsigned char yesno, unsigned int t)
{
PLAYER node = getplayer_byid(id);
int clag = current_lag();
	node->invincible	   = yesno;
	node->invincibility_t  = t;
	node->invincibility_t2 = ourtime + clag;
}

void m_turn(unsigned int id, double x, double y, signed char turn, double deg)
{
PLAYER current = getplayer_byid(id);
unsigned int clag = (unsigned long)current_lag();
	//Preconditions
	if (!current) die("current is null in m_turn");

	current->freeze	= 0;
	current->t		= ourtime - clag;
	current->turn	= turn;
	current->turn_t = ourtime - clag;
	current->deg	= deg;
	current->x		= x;
	current->y		= y;
}
