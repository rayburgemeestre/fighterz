#include "common.h"

/********************************************
*  PROCEDURES
****************************************************/

int terminate_request = 0;

void closereq()
{
	terminate_request = 1;
}

void init()
{
	char *datafile;
	
	allegro_init();

	set_color_depth(16);

	set_display_switch_mode(SWITCH_BACKGROUND);
	
	//set_window_close_button(FALSE);
	set_window_close_hook(closereq);

	init_screen();	
	center_window();

	datafile = strdup(BASE_DATAFILE);
	dataf = load_datafile(datafile);
	//dataf = load_datafile("#");

	destroy_bitmap(tmpscreen);
	tmpscreen = create_bitmap(SCREEN_X, SCREEN_Y);
	
	/* Initialize Allegro. */
	printff_direct("Initialized allegro");

	
#if DEBUG == 1
	/* debug*/
	printff_direct("Preparing debug file");
	boom = fopen( "coords.log", "w+" );
#endif 

	printff_direct("  Keyboard");
	install_keyboard();
	printff_direct(" Mouse");
	install_mouse();
	printff_direct(" Timer");
	install_timer();
	printff_direct("Loading Datafile");
	
#if NOSOUND == 1
	printff_direct("Skipping Sound");
#else
	printff_direct("Loading Sound");

	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
	{
		printff_direct("ERROR initializing sound!");
		USE_SOUND = 0;
	} else {
		USE_SOUND = 1;
	}

	if (USE_SOUND == 1)
		// play_sample((SAMPLE *)dataf[intro].dat, 255, 128, 800, 0);
#endif
}

void start()
{
	int i;

	//set_color_depth(16);
	//init_screen();		
	// center_window();
	
	clear_to_color(tmpscreen, makecol(0,0,0));
	blit (tmpscreen, screen, 0, 0, 0, 0, SCREEN_X, SCREEN_Y);

	//destroy_bitmap(tmpscreen);
	//tmpscreen = create_bitmap(SCREEN_X, SCREEN_Y);
	
	time_proc();
	
	printff_direct("Loading map: %s", map2);

	strncpy(map2, "maps\\lvl-x.txt", 64);
	map2[63] = '\0';

	/* Read map from file and set some important variables */
	// loadmap();
	BLOCKSIZE = 20; // @!

	/* 
		printff_direct("Building map in buffer");
		buildmap();
	*/

	printff_direct("Some variables %s", map2);

	LEFT_2 = (SCREEN_X - 150) - LEFT;
	/*					^ lists width */
	
	SPEED_2 = SPEED;
	B_SPEED_2 = B_SPEED;
	
// snipped: asdfasdfasdf

	showscr = 0;
	
	msg[0] = '\0';
	/* dataf = load_datafile("fighterz.dat"); */

/*	if (!dataf)
	{
		printff_direct("ha");
		terminate();
	}*/

	printff_direct("Initializing linked list for users");

	// add_player(0, 50, 50, 90, 0, 2, 0, 0, 0, "TriGen");

	/* ///////DEBUG::::: */

	/* initiate some values */
	fps_oldtime = time(NULL);
	
	printff_direct("Preparing console");
	for (i=0; i<MAX_C_LINES; i++)
		console[i][0] = '\0';
	printff_direct("Preparing lag array");
	for (i=0; i<5; i++)
		lag[i] = 0.0;
	
	printff_direct("Connecting socket to %s:%d", taddr, tport);
	connect_socket();

/* tests */
	guessed_power = MAX_HITS;

}

void init_screen()
{
	/* Set the resolution with SAFE autodetection. */

	if (FULLSCREEN == 1)
	{
		set_gfx_mode(GFX_SAFE, SCREEN_X, SCREEN_Y, 0, 0);
	} else {
		set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_X, SCREEN_Y, 0, 0);
	}
}

void add_player(int _id, int _x, int _y, int _deg, int _move, int _turn,  
				int _velocity, int _alive, int _kills, int _killavg, 
				int _bot, double _speed, int _dead, char *_nick)
{
	/* create first node in linked list */
	if (!(new_node = (LINK)malloc(sizeof(user))))
	{   printff_direct("Error allocating memory");
		exit(-1);
	}

	/* insert the created node */
	new_node->next = head;
	head = new_node;
	/* intiate some values */
	head->id = _id;
	head->dead = _dead;
	head->bot = _bot;
	head->bullet = 0;
	head->power = MAX_HITS;
	head->TTL = 0;
	head->vel_time = ourtime;
	head->x = _x;
	head->y = _y;

    setsec(head);

	head->deg = _deg;
	head->t = ourtime;
	head->move = _move;
	head->freeze = 0;
	head->turn = _turn;
	head->turn_t = ourtime;
	head->velocity = _velocity;
	head->speed = _speed;
	head->alive = _alive; /* seconds in the game*/
	head->kills = _kills;
	head->kills_avg = _killavg;
	
	head->invincible = 0;
	head->invincibility_t = 0;
	head->invincibility_t2 = ourtime;

	head->hit_t = ourtime;
	head->impact = 0;

	strcpy(head->nick, _nick);
}

/* SETS x2/y2 etc */
void setsec(struct data *ptr)
{
	if (ZOOM > 0.0)
	{
		head->x2 = head->x * (1.0 + ZOOM);
		head->y2 = head->y * (1.0 + ZOOM);
	}
	else if (ZOOM < 0.0)
	{
		head->x2 = head->x / (1.0 - ZOOM);
		head->y2 = head->y / (1.0 - ZOOM);
	}
	else 
	{
		head->x2 = head->x;
		head->y2 = head->y;
	}	
}

void del_player(int sock_fd)
{
	LINK old = NULL;
	LINK future = NULL;

	current = head;
	
	while (current != NULL)
	{
		future = current->next;
		if (current->id == (unsigned)sock_fd)
		{
			if (current == head)
			{
				/* first node */
				future = current->next;
				free(current);
				head = future;
				return;
			} else {
				if (current->next != NULL)
				{
					/* mid node */
					free(current);
					old->next = future;
					return;
				} else {
					/* last node */
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

/********************************************
*  FUNCTIONS
****************************************************/

double current_lag()
{
	int i;
	double current_lag = 0.0;
	for (i=0; i<5; i++)
		current_lag += lag[i];
	return ( current_lag / 5.0 );
}

// getangle() -> I ported this java code i got from www.scottandrew.com 
// Gives the angle of the radian described by the two specified points
int getangle(double x1, double y1, double x2, double y2)
{
	double diffH = (x2 - x1);
	double diffV = (y2 - y1);

	double slope;
	double angle;
	double dgrs;

	if (diffH)
	{
		slope = diffV / diffH ;
		angle = atan(slope);
		dgrs = ((angle * 180) / PI);

		if (diffH < 0)
			dgrs += 180;
	}
	else if (diffV < 0)
		dgrs = 270;
	else if (diffV > 0)
		dgrs = 90;
	else 
		dgrs = 0;
	/* .. het zal allemaal wel :) */
	if (dgrs < 0) 
		dgrs = 360 + dgrs;

	/* leuk maar moet 90 graden naar links ofzo, added:*/
	dgrs -= 90;
	if (dgrs < 0)
		dgrs = -90;
	else if (dgrs > 360)
		dgrs = 90;

	return (int)dgrs;
}

// the window initially starts at 32,32. 
// This will center the window on the desktop
void center_window() 
{ 
#ifdef ALLEGRO_WINDOWS 
    HWND wnd = win_get_window(); 
    HWND desktop = GetDesktopWindow(); 
    RECT wndRect, desktopRect; 
    int  w , h , dw, dh; 
 
    GetWindowRect( wnd , &wndRect ); 
    GetWindowRect( desktop , &desktopRect ); 
    w  = wndRect.right      - wndRect.left; 
    h  = wndRect.bottom     - wndRect.top; 
    dw = desktopRect.right  - desktopRect.left; 
    dh = desktopRect.bottom - desktopRect.top; 
 
    MoveWindow( wnd , ( dw - w ) / 2 , ( dh - h ) / 2 , w , h , TRUE ); 
#endif 
} 

void draw_talk_box()
{
	clear_to_color(talkbuff, makecol(0, 0, 0));
	rect(
		talkbuff, 
		0, 
		0, 
		MAP_W - 1, 
		CSCREEN_H - 1, 
		(
			( talk == 1 )
		? 
			makecol(0, 128, 255)
		:
			(
				( talk == 2 )
			?
				makecol(255, 128, 128)
			:
				makecol(192, 192, 192)
			)
		)
	);
	textprintf(talkbuff, font, (CSCREEN_H / 2) - 10, (CSCREEN_H / 2) - 10, makecol(255, 255, 255), msg);
}

void read_config()
{

}

double dabs(double x)
{
	if (x >= 0.0)
	{
		return (x);
	} else {
		return (-1 * x);
	}
}

void initialize_vars()
{
	/* Will eventually become dynamic etc :) */
	SCREEN_Y = 600;
	SCREEN_X = 800;
	FULLSCREEN = 0;
	HIGH_GRAPHICS = 1;
	BOUNCING_BULLETS = 0;
	MOVE_STEPW = 1; 
	MOVE_BSTEPW = 1;
	SPEED = 0.20; 
	B_SPEED = 0.40;
	TXTPTR = 0; 
	TXTPTR2 = 0; 
	Y_BLOCKS = 0;
	X_BLOCKS = 0;
	BLOCKSIZE = 0;
	LEFT = 20;
	TOP = 20;
	fps = 0; 
	fps_count = 0;
	show_fps = 1;
	show_names = 1;
	show_ulist = 1;
	ourtime = 0;
	our_id = -1;
	STARTED = 0;
	bullet_time = 0;
	BULLET_COUNT = 0;
	BULLET_MAX = 10;
	BULLET_TTL = 3000;
	BULLET_RE = 150;	
	MAP_W = 600;
	MAP_H = 400;
	RADAR_SIZE = 100;
	RADAR_SHOW = 1;
	INDICATOR_WIDTH = 10;
	INDICATOR_DISTANCE_BETWEEN = 5;
	CSCREEN_H = 30;
	ZOOM = 0;
	zoom_time = 0;
	MAX_HITS = 10;
	talk = 0;
	MAX_CHARS_IN_SENTENCE = 80;
	active_o_item = 0;
	field_mode = 1;	
	strcpy(taddr, "localhost");
	tport = 8099;
	direction_is_up = 1;
	ftime(&started); /* ret starting time */

	buffer = (char *)malloc(8192);
}


void mainloop()
{
/********** mainloop init **********/
	drawmap();

/********** mainloop **********/
	while(!key[KEY_F12] && !terminate_request) 
	{
/********** initialization **********/
		time_proc();
		sockread();

		// if (!STARTED && requested_spawn == 0 && key[KEY_ENTER] && can_spawn == 1)
		if (requested_spawn == 0 /* && key[KEY_ENTER] */ && can_spawn == 1)
			send_ispawn();

		if (!STARTED) 
			continue; /* Don't do graphics */
		
/********** graphics **********/
		blit(fieldbuff, tmpscreen, MAP_X, MAP_Y, FIELD_X, FIELD_Y, MAP_W, MAP_H);
		clear_to_color(shipbuff, makecol(255, 0, 255)); /* transparent */
		clear_to_color(RADAR, 0);
		clear_to_color(CONSOLE, 0);
		fps_proc();
		moveships();
		movebullets();
		setrcoords();
		debug();
		drawfps();
		drawconsole();
		drawexplosions();
		drawships();
		drawbullets();
		drawradar();
		draw_talk_box();
		parse_input();
		printulist();		
		set_map_coords();
		show_graphics();
	}
}

void die(char *s)
{
	printff_direct("%s", s);
	delay(1);
	exit(-1);
}

struct data *getplayer_byid(unsigned int id)
{
LINK current;
	for (current=head; current; current=current->next)
		if (current->id == id)
			return current;
	return NULL;
}
