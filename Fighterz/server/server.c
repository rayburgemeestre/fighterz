#include "common.h"

int id_has_redflag  = -1;
int id_has_blueflag = -1;

/* 
	TODO:
	
	- generaliseren fuctureX|Y alike functies in 1 functie
	- vars gooi ik nog wel in gunstige volgoorde/groepen 
	  als ik daar spontaan zin in krijg
	
*/

/***************** VARS ****************/

struct flagpositions flagpos; /**< flagpos will be used to store 
                                 path calculation results (length of all 4 directions) */
// DEPRECATED double SPEED; /**< pixels to move every step :) 
				   //should be in sync with clients, example: 0.20 (pixels) */
struct timeb t; /**< used to calculate our time in millisecs (our_time) */
struct timeb started; /**< used to keep track of the time the game started for us as a client */

#if WINDOWS != 1
/* used for unix socket stuff, TODO: see if all structs are needed
   and where they are used for, select?.. */
struct timeval t2; 
#else
struct timeb t2; 
#endif

struct timeval timevl;
TIME servertime;
unsigned long int lps_time; /**< used for loops/sec counter */
unsigned long int lps_time2; /**< used for loops/sec counter */
unsigned int lps; /**< contains the current amount of loops/sec */
unsigned int lps_counter; /**< used for loops/sec counter */
int player_nr; /**< will start @ zero, and increase with every
                    new bot creation (used for naming them, bot1, bot2 etc) */
unsigned int LAG_CHECKS; /**< Amount of lag checks to perform when new client connects */
TIME LAG_DELAY; /**< Amount of millisecs between these lag checks */
int LAG_DELAY_SECS; /**< Seconds between each normal lag check (while in game) */
unsigned int RESPAWN_DELAY; /**< Delay between death/respawn in _seconds_ */
int MOVE_STEPW; /**< Amount of times to move will be multiplied by this. So
                     i recommend you to let this stay @ 1. But if you make it 2
                     all the ships  shall go twice as fast, because instead of
                     moving the ships 1 time, it will be done 2 times. */
int BLOCKSIZE; /**< Size of blocks in pixels (also the ships are made using this size) */
unsigned long field_width; /**< Contains field width in pixels */
unsigned long field_height; /**< Contains field height in pixels */
int Y_BLOCKS; /**< Amount of blocks vertically on the map */
int X_BLOCKS; /**< Amount of blocks horizontally on the map */
/* Think these max. values are fine */
char field[500][500]; /**< Used in path calculation routines */
char map2[1000][1000]; /**< Used in path calculation routines */
char map3[64]; /**< Contains active map's filename */
int map[1000][1000]; /**< Used in path calculation routines
					      (called map, because the lib that i implemented used this name) */
#if DEBUG == 1
FILE *outp; /**< debug outputfile filedescriptor */
#endif
FILE *outp2; /**< another debug outputfile filedescriptor */
unsigned int BULLET_MAX; /**< User can only have BULLET_MAX bullets at a time _in_ the field (should be in sync with clients) */
TIME BULLET_TTL; /**< Millisecs that a bullet can exist in the field (should be in sync with clients) */
int MOVE_BSTEPW; /**< Amount of times to move will be multiplied by this. So
                      i recommend you to let this stay @ 1. But if you make it 2
                      all the ships  shall go twice as fast, because instead of 
					  moving the ships 1 time, it will be done 2 times. */
double B_SPEED;  /**< pixels to move every step :) 
				      should be in sync with clients, example: 0.40 (pixels) */

#if DEBUG2 == 1
BITMAP *tmpscreen; /* primary bitmap screen buffer used to draw everything on */
BITMAP *fieldbuff; /**< Screen bitmap buffer used to draw the field to once (after that this 
                        bitmap will be constantly blitted to tmpscreen )*/
BITMAP *shipbuff; /**< Screen bitmap buffer where the ships/bullets are drawn to */

#define MAX_C_LINES 12 /**< Maximum amount of lines visible in the console window */
#define MAX_C_LLENGTH 80 /**< Maximum amount of characters possible in each line */
char console[MAX_C_LINES][MAX_C_LLENGTH]; /**< Array used to store the entire console with messages */
int TXTPTR; /**< Text 'pointer', will contain positions of the text on the screen
               used in textprintf(); */
int TXTPTR2; /**< Text 'pointer', will contain positions of the text on the screen
               used in textprintf(); */
int LEFT_2; /**< Distance from the left of the screen, where the player list should be situated */
int FULLSCREEN; /**< indicates whether to use fullscreen or not, 1 = yes */
int SCREEN_Y; /**< Screen height in pixels */
int SCREEN_X; /**< Screen width in pixels */
int LEFT; /**< Used margin from the left of the screen */
int TOP; /**< Used margin from the top of the screen */
int show_fps; /**< Indicates whether to show frames/sec on the screen, 1 = yes */
int show_ulist; /**< Indicates whether to show playerslist on the screen, 1 = yes */
int fps; /**< Contains the current amount of frames/sec */
int fps_count; /**< Used to calculate the frames/sec */
long int fps_oldtime; /**< Used to calculate the frames/sec */
long int fps_newtime; /**< Used to calculate the frames/sec */
int MAP_X; /**< X Coordinate on fieldbuffer wherefrom should be started to blit on tmpscreen */
int MAP_Y; /**< Y Coordinate on fieldbuffer wherefrom should be started to blit on tmpscreen */
int MAP_W; /**< Specified Width of the field to draw */
int MAP_H; /**< Specified Height of the field to draw */
int VIEW_X; /**< Current x coordinate from where we are viewing at the field */
int VIEW_Y; /**< Current x coordinate from where we are viewing at the field */
unsigned int FIELD_X; /**< X Coordinate of where the field should be drawn to on the screen */
unsigned int FIELD_Y; /**< Y Coordinate of where the field should be drawn to on the screen */
unsigned int map_scroll; /**< Used to constrain the speed of scrolling on the map to time */
#endif /* DEBUG2 == 1 */

unsigned int MAX_HITS; /**< Contains the amount of bullet impacts a ship can have before it dies */
/* *** TODO: MAX_CHARS_IN_SENTENCE is eigenlijk dubbel, want er is ook al:
             MAX_C_LLENGTH dus 1 moet weg..  *** */
unsigned int MAX_CHARS_IN_SENTENCE; /**< Maximum amount of characters possible in one sentance. */
struct location RED; /**< struct which will contain the position of the red flag 
                          on the map (not in x/y coordinates, but in blocks)*/
struct location BLUE; /**< struct which will contain the position of the red flag 
                           on the map (not in x/y coordinates, but in blocks)*/
struct location REDFLAG; /**< struct which will contain the position of the red flag 
                              on the map (not in x/y coordinates, but in blocks)*/
struct location BLUEFLAG; /**< struct which will contain the position of the red flag 
                               on the map (not in x/y coordinates, but in blocks)*/
int extra_x; /**< Used within the path calculations (x-offset) */
int extra_y; /**< Used within the path calculations (x-offset) */
ID bot_id; /**< Used to give bots id's like 1000, 1001, 1002 etc */
int path_calc_dbg_on; /**< Indicates whether a debug file should be written about
                           how the path was calculated */
FILE *path_calc_dbg; /**< pathcreation debugfile filedescriptor */
LINK head = NULL; /**< Pointer to the first node in the linked list */
int dist[1000][1000]; /**< Used within the findpath routines */
int parent[1000][1000][2]; /**< Used within the findpath routines */
int startr; /**< Start coordinate from the left, used within the findpath routines 
                 (not x,y coord, but a position on the map in blocks) */
int startc; /**< Start coordinate from the left, used within the findpath routines 
                 (not x,y coord, but a position on the map in blocks) */
int endr; /**< End coordinate from the left, used within the findpath routines 
               (not x,y coord, but a position on the map in blocks) */
int endc; /**< End coordinate from the left, used within the findpath routines 
               (not x,y coord, but a position on the map in blocks) */
int rows_read, cols_read; /**< Help vars used within findpath routines */
int pops, pushes; /**< Help vars used within findpath routines */
int element; /**< Help vars used within findpath routines */
int debug_level; /**< Debug level for within findpath routines (in my 
                      implimentation this must always be zero) */

int temprrr = 0; /**< debug var, can be deleted */

/* End of vars section */

//temp
void flagcheck()
{
int blue_x = BLUEFLAG.x + (BLOCKSIZE / 2);
int blue_y = BLUEFLAG.y + (BLOCKSIZE / 2);
int red_x  = REDFLAG.x  + (BLOCKSIZE / 2);
int red_y  = REDFLAG.y  + (BLOCKSIZE / 2);

	LINK lnk;
	for (lnk = head; lnk; lnk = lnk->next)
	{
		if (lnk->bot == 1 || lnk->bullet == 1)
			continue;

		if ( (id_has_redflag == -1 && lnk->id != id_has_blueflag) &&
			((sqrt((lnk->x - red_x) * (lnk->x - red_x)+
			 (lnk->y - red_y) * (lnk->y - red_y)) <= (BLOCKSIZE / 2))) )
		{
			id_has_redflag = lnk->id;
			send_flagcarrier(lnk->id, 1);
		}
		else if ( (id_has_blueflag == -1 && lnk->id != id_has_redflag) &&
			  (sqrt((lnk->x - blue_x) * (lnk->x - blue_x)+
			 (lnk->y - blue_y) * (lnk->y - blue_y)) <= (BLOCKSIZE / 2)) )
		{
			id_has_blueflag = lnk->id;
			send_flagcarrier(lnk->id, 2);
		}
	}
}

int main(void)
{
	/* ******************** */
	/* *** UNIX SOCKETS *** */
	/* ******************** */

	fd_set master;   // master file descriptor list
	fd_set read_fds; // temp file descriptor list for select()
	struct sockaddr_in myaddr;	 // server address
	struct sockaddr_in remoteaddr; // client address
	int fdmax;		// maximum file descriptor number
	int listener;	 // listening socket descriptor
	int newfd;		// newly accept()ed socket descriptor
//	char buf[256];	-- buffer for client data
//	int nbytes;
	int yes=1;		// for setsockopt() SO_REUSEADDR, below
	int addrlen;
	int i;
	const int just_say_no = 1;
	struct data *client;
#if WINDOWS == 1
	
	/* ******************* */
	/* *** WIN SOCKETS *** */
	/* ******************* */

	#define	SERV_TCP_PORT	PORT
	#define	SERV_HOST_ADDR	"127.0.0.1"  /* Change this to be your host addr */

    // char                request;
    struct WSAData      data;

    WORD			x;
    int			ret_value;
	int		select_timeout = 10; /* millisecs ofzo? */
	timevl.tv_sec = 0;
	timevl.tv_usec = 10000;
	//timevl.tv_usec = 100;
#endif

	settime(); 

	/* ********************************* */
	/* *** TEMPORARY INITIALIZATIONS *** */
	/* ********************************* */
	// in the future all settings will be gathered from 
	// server... (together with the map etc)
	initialize_vars();

	initialize_debug();

#if DEBUG2 == 1
	init(); /* also contains all graphics routines */
#endif


#if WINDOWS == 1
	/* ******************************** */
	/* *** WINDOWS SOCKETS SPECIFIC *** */
	/* ******************************** */

    x=257; /* Using WINSOCK Version 1.1 (Windows 95) */
    ret_value = WSAStartup(x,&data);

	FD_ZERO(&master);	// clear the master and temp sets
	FD_ZERO(&read_fds);

    /* Create communication endpoint */
    if ( (listener = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("server: can't open stream socket");
        exit(1);
    }

    /* Bind socket to local address */
    memset((char *) &myaddr, 0, sizeof(myaddr));
    myaddr.sin_family      = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port        = htons(SERV_TCP_PORT);

    if (bind(listener, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
        printf("server: can't bind local address");
        exit(1);
    }

	// listen
	if (listen(listener, 10) == -1) {
		perror("listen");
		exit(1);
	}
#else
	/* **************************** */
	/* *** BSD SOCKETS SPECIFIC *** */
	/* **************************** */

	(void)signal(SIGPIPE, SIG_IGN);

	FD_ZERO(&master);	// clear the master and temp sets
	FD_ZERO(&read_fds);

	// get the listener
	if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	// lose the pesky "address already in use" error message

    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,
                                                       sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }


	// bind
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(PORT);
	memset(&(myaddr.sin_zero), '\0', 8);
	if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	// listen
	if (listen(listener, 10) == -1) {
		perror("listen");
		exit(1);
	}
#endif

	/* make socket nonbuffering */
	if (setsockopt(listener, IPPROTO_TCP, TCP_NODELAY, (char*)&just_say_no,
														sizeof just_say_no) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	// add the listener to the master set
	FD_SET(listener, &master);

	// keep track of the biggest file descriptor
	fdmax = listener; // so far, it's this one
	
	loadmap();
	
#if DEBUG2 == 1
	drawmap();
#else
	drawmap_features();
#endif

/* ************************* */
/* *** MAIN PROGRAM LOOP *** */
/* ************************* */

#if DEBUG2 == 0
	for(;;) {
#else
	while(!key[KEY_ESC]) {
		
		set_map_coords();
#endif
		settime();
		
		lps_proc();
		
		lag_check_proc();

		moveships();
		
		movebullets();

		flagcheck();

		/* ***** ai_bots() ****** */
		{
		LINK humanoid, bot;
		for (bot = head; bot; bot = bot->next)
			if (bot->bot == 1 && bot->invincible != 1 && bot->dead == 0)
				for (humanoid = head; humanoid; humanoid = humanoid->next)
					if (humanoid->bot == 0 && humanoid->dead == 0 && humanoid->invincible == 0)
						if (sqrt((bot->x - humanoid->x)*(bot->x - humanoid->x)+
								 (bot->y - humanoid->y)*(bot->y - humanoid->y)) <= 300)
						{
							if ( (bot->bullet_time == 0) || ((servertime - bot->bullet_time) >= (unsigned)BULLET_RE) )
							{	bot->bullet_time = servertime;
								if (bot->bulletcnt < BULLET_MAX)
								{	
									LINK b;
									double angle  = getangle(bot->x, bot->y, humanoid->x, humanoid->y) + 180;

									//bot->velocity = 0;
									//bot->speed    = 0.0;
									//send_accel(EVERYONE, bot, bot->id, bot->x, bot->y, (signed char)bot->velocity, bot->speed);

									b = add_bullet(bot, bot->x, bot->y, angle, servertime);									
									//send_newbullet(EVERYONE, bot, b->id, bot->id, bot->x, bot->y, angle)
									m_newbullet(bot, bot->x, bot->y, angle);
									break;							
								}
							}
						}
		}
//test
#if DEBUG2 == 1
		if (key[KEY_H])
		{
			struct data *bot;
			// findpath(head);
			bot = add_bot();
			notify_of_newuser(EVERYONE, NULL, bot);
			
			notify_of_respawn(EVERYONE, NULL, bot);
			flyto((bot_id - 1), REDFLAG.x, REDFLAG.y);
/*
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
			} */
			while (key[KEY_H]);
		}
		if (key[KEY_G])
		{
			struct data *bot;
			// findpath(head);
			bot = add_bot();
			notify_of_newuser(EVERYONE, NULL, bot);
			
			notify_of_respawn(EVERYONE, NULL, bot);
			
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

				flyto(bot->id, rand_x, rand_y);
			}
			while (key[KEY_G]);
		}
#endif
//test
		
		/* ********************** */

#if DEBUG2 == 1
		parse_input();

		drawgraphics();
#endif

		/* show loops/sec every 10 secs */
		if ( (time(NULL) - lps_time2) >= 10 )
		{
			printf_("Loops/sec: %d hmz: %u\n", lps, servertime);
			lps_time2 = time(NULL);

			printplayerlist();
		}

		read_fds = master; // copy it

#if WINDOWS == 1
		if (select(fdmax+1, &read_fds, NULL, NULL, &timevl) == -1) {
			perror("select");
			exit(1);
		}
#else
		if (select(fdmax+1, &read_fds, NULL, NULL, &t2) == -1) {
			perror("select");
			exit(1);
		}
#endif 

		// run through the existing connections looking for data to read
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // we got one!!
				if (i == listener) {
					// handle new connections
					addrlen = (int)sizeof(remoteaddr);
#if WINDOWS == 1
					if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen)) == -1) { 
#else
					if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr, (socklen_t *)&addrlen)) == -1) { 
#endif
						perror("accept");
					} else {
#if 1 == 2
						if (set_sockopts(newfd) < 0)
						{
							printf("Ouch! Lost client when setting sockopts (possible at Linux 2.4.x/BSD/etc)\n");
							continue; 
						}
#endif
						/* make socket nonbuffering */
						if (setsockopt(newfd, IPPROTO_TCP, TCP_NODELAY, (char*)&just_say_no,
																	sizeof just_say_no) == -1)
						{
							perror("setsockopt");
							exit(1);
						}

						FD_SET(newfd, &master); // add to master set
						if (newfd > fdmax) {	// keep track of the maximum
							fdmax = newfd;
						}
						/* MARK */
						printf_("selectserver: new connection from %s on "
							"socket %d\n", inet_ntoa(remoteaddr.sin_addr), newfd);
						
						client = add_player((ID)newfd);
						send_hi(client);
						/* TODO: send_motd(client); */
					}
				} else {
					// handle data from a client
					client = getplayer_byid((ID)i);
					if (!client)
					{
						/* Uh??? */
						printf("ARGH! socket %d has no client entry -- closing!\n", i);
						CLOSE_SOCK(i);
					} else {
						/* Normal client */
						readpacket(client, i);
						if (IsDead(client))
						{
							if (client->enabled == STATE_DEADSEND)
								quit_player(client, client->errmsg);
							del_tha_player(client);
							FD_CLR((unsigned)i, &master);
						}
					}
				}
			}
		} /* run through the existing connections */
		
		/* Now check for any dead clients and remove them */
		for (client = head; client; client = client->next)
		{
			if (IsDead(client))
			{
				if (client->enabled == STATE_DEADSEND)
					quit_player(client, client->errmsg);
				del_tha_player(client);
				FD_CLR((unsigned)i, &master);
			}
		}
	}

#if DEBUG2 == 1
	allegro_exit(); 
#endif
		return 0;
}
#if DEBUG2 == 1
END_OF_MAIN();
#endif

