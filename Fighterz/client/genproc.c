#include "common.h"

int radar_padding_x = 0;
int radar_padding_y = 0;

/********************************************
*  PROCEDURES
****************************************************/

int terminate_request = 0;
int midi_track = 3;

void closereq()
{
	terminate_request = 1;
}

void init()
{
	char *datafile;
	char *datafile_sound;

	allegro_init();

	set_color_depth(16);

	set_display_switch_mode(SWITCH_BACKGROUND);
	
	//set_window_close_button(FALSE);
	set_window_close_hook(closereq);

	init_screen();	
	clear_to_color(screen, makecol(0, 0, 0));
	center_window();

	datafile = strdup(BASE_DATAFILE);
	dat_base = load_datafile(datafile);
	
	datafile_sound = strdup(BASE_SOUND_DATAFILE);
	dat_sound = load_datafile(datafile_sound);
	if (!dat_sound)
		alert("Could not load sound", "", "", "", NULL, 0, 0);

	//dataf = load_datafile("#");

	destroy_bitmap(tmpscreen);
	tmpscreen = create_bitmap(screensize_x, screensize_y);
	
	/* Initialize Allegro. */
	printff_direct("Initialized allegro");

	/* init AllegroFont */
	printff_direct("Loading AllegroFont...");
	if (alfont_init() != ALFONT_OK) 
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error while initializing alfont");
		allegro_exit();
		return;
	}
	/* try to load the font given by the parameter*/
	printff_direct("   - lcdn.ttf");
	lcdfont = alfont_load_font("system/lcdn.ttf");
	if (lcdfont == NULL) 
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Cannot load the font lcdn.ttf");
		alfont_exit();
		allegro_exit();
		return;
	}
	alfont_set_font_size(lcdfont, 19);
	
	/* try to load the font given by the parameter*/
	printff_direct("   - tccm.ttf");
	tccmfont = alfont_load_font("system/tccm.ttf");
	if (tccmfont == NULL) 
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Cannot load the font tccmfont.ttf");
		alfont_exit();
		allegro_exit();
		return;
	}
	alfont_set_font_size(tccmfont, 28);
	

	printff_direct("  Keyboard");
	install_keyboard();
	printff_direct(" Mouse");
	install_mouse();
	printff_direct(" Timer");
	install_timer();
	printff_direct("Loading Datafile");
	

	printff_direct("Loading Sound");

	load_midi_patches();

	int a = install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	if (a < 0)
	{
		printff_direct("ERROR initializing sound!");
		terminate();
	}

		//play_rand_bg_music();
		intro_music = (MIDI *)dat_sound[METAMORPHOSIS4].dat;

}

int start()
{
int i;

stop_midi();
//play_rand_bg_music();

	//set_color_depth(16);
	//init_screen();		
	// center_window();
	
	clear_to_color(tmpscreen, makecol(0,0,0));
	blit (tmpscreen, screen, 0, 0, 0, 0, screensize_x, screensize_y);

	time_proc();
	

	/* 
		printff_direct("Building map in buffer");
		buildmap();
	*/



	
	
// snipped: asdfasdfasdf

	
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
	for (i=0; i<CON_LINES; i++)
		console[i][0] = '\0';
	printff_direct("Preparing lag array");
	for (i=0; i<5; i++)
		lag[i] = 0.0;
	
	printff_direct("Connecting socket to %s:%d", serv_addr, serv_port);
	if (-1 == socket_connect())
		return -1;


	return 0;
}

void init_screen()
{
	/* Set the resolution with SAFE autodetection. */

	if (fullscreen == 1)
	{
		set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, screensize_x, screensize_y, 0, 0);
	} else {
		set_gfx_mode(GFX_AUTODETECT_WINDOWED, screensize_x, screensize_y, 0, 0);
	}
}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!! DEPRECATED !!!!!!!!!!!!!!!!!!! ?? LOL ik laat echt te veel staan ;)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
struct data *add_player(int _id, int _x, int _y, int _deg, int _move, int _turn,  
				int _velocity, int _alive, int _kills, int _killavg, 
				int _bot, double _speed, int _dead, char *_nick)
{
	/* create first node in linked list */
	if (!(new_node = (PLAYER)malloc(sizeof(user))))
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
	head->power = ship_maxpower;
	head->TTL = 0;
	head->vel_time = ourtime;
	head->x = _x;
	head->y = _y;


	head->deg = _deg;
	head->t = ourtime;
	head->move = _move;
	head->freeze = 0;
	head->turn = _turn;
	head->turn_t = ourtime;
	head->velocity = _velocity;
	head->speed = _speed;
	head->max_speed = 0.20;
	head->alive = _alive; /* seconds in the game*/
	head->kills = _kills;
	head->kills_avg = _killavg;
	
	head->invincible = 0;
	head->invincibility_t = 0;
	head->invincibility_t2 = ourtime;

	head->hit_t = ourtime;
	head->impact = 0;

	strcpy(head->nick, _nick);

	return head;
}

void del_player(int sock_fd)
{
	PLAYER old = NULL;
	PLAYER future = NULL;

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


double getdistance(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// getangle() -> I ported this java code i got from www.scottandrew.com 
// Gives the angle of the radian described by the two specified points
double getangle(double x1, double y1, double x2, double y2)
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

	return (double)dgrs;
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
	clear_to_color(bmp_command, makecol(0, 0, 0));
	textprintf(bmp_command, (FONT *)dat_base[NOKIA].dat, 
		5, 5, makecol(255, 255, 255), "%s", msg);
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
	our_spawnstatus = 0;
	fullscreen = 0;
	high_gfx = 1;
	mod_bounce = 0;
	ship_movespeed_multiplier = 1; 
	bullet_movespeed_multiplier = 1;
	ship_maxpower = 3;
	// DEPRECATED SPEED = 0.20; now: playerObj->max_speed!!!!!!
	bullet_movespeed = 0.40;

	fps = 0; 
	fps_count = 0;
	ourtime = 0;
	our_id = -1;
	game_started = 0;
	our_bullet_count = 0;
	our_bullet_max = 10;
	our_bullet_ttl = 3000;
	our_bullet_autofiredelay = 150;	
	
	show_radar = 1;
	
	talk_mode = 0;

	options_activeitem = 0;
	
	strcpy(serv_addr, "localhost");
	serv_port = 8099;
	our_direction = 1;
	ftime(&started); /* ret starting time */

	PLAYER head = NULL;
	PLAYER new_node = NULL;
	PLAYER current = NULL;
	PLAYER our_node = NULL;
	EXPLOSION expl_head = NULL;
	EXPLOSION expl_new = NULL;
	EXPLOSION expl_current = NULL;

	buffer = (char *)malloc(8192);
}


void drawflags()
{
#define FLAG_ANIM_ITERATOR 500
static unsigned long prev_flag_time = 0;
static unsigned int flag = 1; // 1 or 2;
int red_x, red_y, blue_x, blue_y;

	if (red_flag_carrier != -1)
	{
	PLAYER lnk = getplayer_byid(red_flag_carrier);

		red_x = lnk->x - (5) - x_on_map + 3;
		red_y = lnk->y - (18) - y_on_map + 3;
	} else {
		red_x = red_flag_x - x_on_map + 3;
		red_y = red_flag_y - y_on_map + 3;
	}

	if (blue_flag_carrier != -1)
	{
	PLAYER lnk = getplayer_byid(blue_flag_carrier);

		blue_x = lnk->x - (5) - x_on_map + 3;
		blue_y = lnk->y - (18) - y_on_map + 3;
	} else {
		blue_x = blue_flag_x - x_on_map + 3;
		blue_y = blue_flag_y - y_on_map + 3;
	}

	if ( (prev_flag_time == 0) || 
		 ((ourtime - prev_flag_time) >= FLAG_ANIM_ITERATOR) )
	{
		prev_flag_time = ourtime;

		flag = flag == 1 ? 2 : 1;
	}

	if (flag == 1)
	{
		// on field
		draw_sprite(bmp_shipfield, (BITMAP *)dat_base[REDFLAG1].dat, red_x, red_y);
		draw_sprite(bmp_shipfield, (BITMAP *)dat_base[BLUEFLAG1].dat, blue_x, blue_y);
		// on radar
		draw_sprite(bmp_radar, (BITMAP *)dat_base[REDFLAG1].dat, radar_redflag_x, radar_redflag_y);
		draw_sprite(bmp_radar, (BITMAP *)dat_base[BLUEFLAG1].dat, radar_blueflag_x, radar_blueflag_y);
	}
	else
	{
		// on field
		draw_sprite(bmp_shipfield, (BITMAP *)dat_base[REDFLAG2].dat, red_x, red_y);
		draw_sprite(bmp_shipfield, (BITMAP *)dat_base[BLUEFLAG2].dat, blue_x, blue_y);		
		// on radar
		draw_sprite(bmp_radar, (BITMAP *)dat_base[REDFLAG2].dat, radar_redflag_x, radar_redflag_y);
		draw_sprite(bmp_radar, (BITMAP *)dat_base[BLUEFLAG2].dat, radar_blueflag_x, radar_blueflag_y);
	}

	return;
}
void blit_field()
{
	// Draw (cached) fieldbuffer on tmpscreen
	blit(bmp_mapfield, tmpscreen, x_on_map, y_on_map, field_x, field_y, field_w, field_h);
	// Draw Radar on screen
	if (show_radar == 1)
		draw_sprite(bmp_radar, (BITMAP *)dat_base[GUI_RADAR_BG].dat, 0, 0);
}

void blit_radar()
{
	if (show_radar == 0)
		return;
	else
	{
	int radar_scale = blocksize; // ja! precies ja!
	double map_x = (our_node == NULL ? 1 : our_node->x) - field_width / 2;
	double map_y = (our_node == NULL ? 1 : our_node->y) - field_height / 2;
	double pad_x = map_x / radar_scale + 1;
	double pad_y = map_y / radar_scale + 1;

		if (pad_x <= 0 ) pad_x = 0;
		if (pad_y <= 0 ) pad_y = 0;
		if (pad_x >= ((double)radar_field_bmp_w - radar_field_w)) pad_x = (double)radar_field_bmp_w - radar_field_w;
		if (pad_y >= ((double)radar_field_bmp_h - radar_field_h)) pad_y = (double)radar_field_bmp_h - radar_field_h;
		//addtext("%.2f %.2f %.2f", pad_x, field_w, );

		// Draw (cached) radarbuffer on tmpscreen
		blit(bmp_radarfield, bmp_radar_display, (int)pad_x, (int)pad_y, 0, 0, (int)radar_field_w, (int)radar_field_h);

		radar_padding_x = pad_x;
		radar_padding_y = pad_y;
	}
}

int mainloop()
{
/********** mainloop init **********/
	//draw_map();

/********** mainloop **********/
	while(!key[KEY_F12] && !terminate_request) 
	{
/********** initialization **********/
		time_proc();
		socket_read();

		// if (!game_started && our_spawnrequested == 0 && key[KEY_ENTER] && our_spawnstatus == 1)
		if (our_spawnrequested == 0 && key[KEY_LCONTROL] && our_spawnstatus == 1)
		{
			send_ispawn();
			addtext("*** To turn music off/on use F8 & F9");
		}

		if (!game_started && our_spawnstatus == 0)
			continue; /* Don't do graphics */

/********** graphics **********/
// clear_to_color(bmp_shipfield, makecol(255, 0, 255)); /* transparent */



		fps_proc();

/* Nice moment to check if invinsibility should be turned off :) */

for (current=head; current!=NULL; current=current->next)
	if (current->invincible == 1)
	{
		int diff;
		diff = ourtime - current->invincibility_t2;
		if (diff > (signed)current->invincibility_t)
		{
			/* No longer invincible */
			current->invincible = 0;
		}
	}

		move_ships();
		move_bullets();
		debug();

		parse_input();
		

// actual drawing
		set_map_coords();

		
		blit_field();
		blit_gui_iter();
		blit_radar();
		
		
		
		draw_talk_box();
		draw_fps();

		draw_console();
		draw_explosions();
		
		draw_bullets();
		draw_ships();
		drawflags();
		
		
		draw_radar();
		
		printulist();
		
		show_graphics();
//

		// MIDI
		if (midi_pos < 0 && bg_music_on == 1)
		{
			play_rand_bg_music();
		}
	}

	// clean up
	while (head != NULL)
		del_player(head->id);

	stop_midi();

	return -1;
}

void die(char *s)
{
	printff_direct("%s", s);
	delay(1);
	exit(-1);
}

struct data *getplayer_byid(unsigned int id)
{
PLAYER current;
	for (current=head; current; current=current->next)
		if (current->id == id)
			return current;
	return NULL;
}


void play_rand_bg_music()
{
	midi_track++;
	//int j = 1+(int) (5*rand()/(RAND_MAX+1.0));

	if (midi_track == 7)
		midi_track = 1;

	switch (midi_track)
	{
		case 1:
			set_volume(255, 130);
			bg_music = (MIDI *)dat_sound[BG_MUSIC ].dat;
			break;
		case 2:
			set_volume(255, 130);
			bg_music = (MIDI *)dat_sound[BG_MUSIC2].dat;
			break;
		case 3:
			set_volume(255, 130);
			bg_music = (MIDI *)dat_sound[BG_MUSIC3].dat;
			break;
		case 4:
			set_volume(255, 130);
			bg_music = (MIDI *)dat_sound[BG_MUSIC4].dat;
			break;
		case 5:
			set_volume(255, 130);
			bg_music = (MIDI *)dat_sound[BG_MUSIC5].dat;
			break;
		case 6:
			set_volume(255, 130);
			bg_music = (MIDI *)dat_sound[BG_MUSIC6].dat;
			break;
	}
	play_midi(bg_music, FALSE);
}