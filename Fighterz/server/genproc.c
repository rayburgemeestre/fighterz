#include "common.h"

/********************************************
*  GENERAL PROCEDURES
****************************************************/

void settime()
{
#if WINDOWS == 1
	ftime(& t);
	/* this var is used in the entire program */
	servertime = (TIME)(( (t.time - started.time) * 1000) + ( (t.millitm - started.millitm ) / 1));
#else
	ftime(& t);
	servertime = (TIME)( (t.time - started.time) * 1000) + ( (t.millitm - started.millitm) / 1);
	/* initiate some values */
	t2.tv_usec = 5000;
	//t2.tv_sec = 100;
#endif	
}


void lps_proc()
{
	lps_counter++;
	if ( (time(NULL) - lps_time) >= 1 )
	{
		lps = lps_counter;
		lps_counter = 0;
		lps_time = time(NULL);
	}
}

// used when player requested to join party
void lag_check_proc()
{
	LINK current = head; 
	while (current != NULL)
	{
		if (IsPlayer(current) && (current->bullet != 1) && (current->bot != 1))
		{
			if ( current->init_lag > 1 )
			{
				if ( (servertime - (current->last_lag_check) ) >= LAG_DELAY )
				{
					send_servertime(current);
					current->last_lag_check = servertime;
					current->init_lag--;

					if (current->init_lag == 1)
					{	
					int cnt;

						/* do not respawn yet.. send map first */
						SetSynched(current);
						// send map
						send_clearfield(current);
						for (cnt=0; cnt<Y_BLOCKS; cnt++)
						{
							send_fieldline(current, cnt, field[cnt]);
						}
						
						send_blockinfo(current, 
							X_BLOCKS, Y_BLOCKS, BLOCKSIZE);
						for (cnt=0; cnt<32; cnt++)
						{
							if (strlen(bg_imgs_data[cnt].datfile) > 0)
								send_background(
									current, 
									bg_imgs_data[cnt].df_id, 
									bg_imgs_data[cnt].pos_x,
									bg_imgs_data[cnt].pos_y,
									bg_imgs_data[cnt].datfile
								);
						}
						send_fieldend(current);
						SetGotField(current);
					}
				}
			} else {
				/* just check if we need another lagcheck [normal "in game" lagchecks] */
				if (IsRegistered(current) && (servertime - (current->last_lag_check) ) >= (TIME)(LAG_DELAY_SECS * 1000) )
				{
					send_servertime(current);
					//sock__NOTUSED__write(current->id, "1 %lu\n", servertime);
					current->last_lag_check = servertime;
				}
			}
		}

		current = current->next;
	}
}

#if IGNORE_LAG != 1
double current_lag(ID sockid)
{
	int i;
	double _current_lag = 0.0;
	LINK current;

	current = getplayer_byid(sockid);

	for (i=0; i<5; i++)
		_current_lag += current->lag[i];

	return ( _current_lag / 5.0 );
}
#else
double current_lag(ID sockid)
{
	return 0.0;
}
#endif


int printf_(char *pattern, ...)
{
	char buf[1024];
	
	va_list ap;
	va_start (ap, pattern);
	vsnprintf(buf, 1024, pattern, ap);
	va_end(ap);
	
#if DEBUG2 == 1
	/* send output to file */
	fprintf(outp2, "ha: %s\n", buf);
	fflush(outp2);
	addtext("%s", strtok(buf, "\n"));
	//fputs(buf, outp2);
#else
	/* display output on screen */
	printf("%s", buf);
#endif

	return 0;
}

#if DEBUG2 == 1
void init()
{
	allegro_init();
	init_screen();
	install_keyboard();
	
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
void parse_input()
{
int i;
#if 1 == 1
	/* asdfasdf */
	if (keypressed())			
	{
		char k = readkey() & 0xff;
		if ( k == 'f')
		{
			struct data *bot;
			// findpath(head);
			bot = add_bot();
			notify_of_newuser(EVERYONE, NULL, bot);
			flyto(bot->id, REDFLAG.x, REDFLAG.y);
			// addtext("2: %d", head->bot);
			//}
/*
			//extreme debug
			#define PATH_MAX_ 513
			for (i=0; i<PATH_MAX_ && bot->path[i][0] != 0 && bot->path[i][1] != 0; i++)
			{
				int x = bot->path[i][0] * BLOCKSIZE,
					y = bot->path[i][1] * BLOCKSIZE;

				addtext("%d, %d", x, y);
				circlefill(fieldbuff, x, y, 5, makecol(255, 0, 0));
			} */
		}
	}
	/* /asdfasdf*/
#endif
}
void drawgraphics()
{
	int groen = makecol(0, 255, 128);
	clear_to_color(tmpscreen, 0);


	/* calc fps */
	fps_proc();

	/* draw ships */
	drawships();
	drawbullets();	


	/* playerlist */
	if (show_ulist == 1) 
		printulist();

	/* mouse */
	/* maybe later for features like menu's, player 
	   lists with scores or whatever */

	// show_mouse(tmpscreen);
	// poll_mouse();
	
	/* ! fieldbuff -> tmpscreen */
	blit (fieldbuff, tmpscreen, MAP_X, MAP_Y, FIELD_X, FIELD_Y, MAP_W, MAP_H);
	/* ! shipbuff -> tmpscreen */
	//fix later
	masked_blit(shipbuff, tmpscreen, MAP_X, MAP_Y, FIELD_X, FIELD_Y, MAP_W, MAP_H);

	if (show_fps == 1 && head != NULL) 
	{
		textprintf(tmpscreen, font, 1, 1, 15, "FPS: %d spd:%.2f lg:%2.2f bc:%du  %d,%d", fps, (head != NULL?head->speed:0.0), (head != NULL?current_lag(head->id):0.0), head->bulletcnt,
		(int)REDFLAG.x, (int)REDFLAG.y);
			/* shows: fps, speed, lag, bulletcount.. */
	}

	drawconsole();

	/* ! tmpscreen -> screen */
	blit (tmpscreen, screen, 0, 0, 0, 0, SCREEN_X, SCREEN_Y);

	/* vertical retrace not really necessary anymore 
	   now we use double buffering */
	//vsync();
	/* poll_keyboard();    maybe when porting to linux this command should
	                       be uncommented ? ;) */
}

/* fps_proc */
void fps_proc()
{
	fps_newtime = time(NULL);

	// Change fps var..?
	if (fps_oldtime!= fps_newtime) {
		fps_oldtime = fps_newtime; 
		fps = fps_count; /* set fps */
		fps_count = 0; /* reset fps counter */
	}

	fps_count++; /* increase frame counter */
}
#endif


void initialize_vars()
{
	path_calc_dbg_on = 0; // == off

	// lag_check = 0;
	player_nr = 0;
	LAG_CHECKS = 5; 
	LAG_DELAY = 300;
	LAG_DELAY_SECS = 2;
	RESPAWN_DELAY = 5; /* secs */
	MOVE_STEPW = 1;
	BLOCKSIZE = DEF_BLOCKSIZE;
	Y_BLOCKS = 0;
	X_BLOCKS = 0;
	BULLET_MAX = 5; 
	BULLET_TTL = 3000;
	MOVE_BSTEPW = 1;
	B_SPEED = 0.40;
	MAX_HITS = 3;
	MAX_CHARS_IN_SENTENCE = 80;
	bot_id = 1000;
	debug_level = 0;
	temprrr = 0;
#if DEBUG2 == 1
	TXTPTR = 0;
	TXTPTR2 = 0;
	FULLSCREEN = 0;
	SCREEN_Y = 600;
	SCREEN_X = 800;
	LEFT = 20;
	TOP = 50;
	show_fps = 1;
	show_ulist = 1;
	fps = 0; 
	fps_count = 0;
	MAP_W = 600;
	MAP_H = 400;
	VIEW_X = MAP_W / 2;
	VIEW_Y = MAP_H / 2;
	map_scroll = servertime;
#endif

	/* time */
	lps_time = time(NULL);
	lps_time2 = time(NULL);
	lps_counter = 0;
	ftime(&started);
	

	/* map */
	strncpy(map3, "maps/lvl-syz.txt", 64);
	// strncpy(map3, "maps/lvl-x2.txt", 64);
	map3[63] = '\0';
	return;
}

void initialize_debug()
{
	
	if (path_calc_dbg_on)
		path_calc_dbg = fopen("pathcalc.txt", "w+");
	/* Path calculation verbose will be printed to this file */

#if DEBUG == 1
	/* Ship positions/movement log */
	if (!(outp = fopen("debug.log", "w+")))
	{	perror("wlkjdflk\n");
		exit(-1);
	}
#endif
#if DEBUG2 == 1
	if (!(outp2 = fopen(DEBUG_FILE, "w+")))
	{
		perror("sdlkfjas\n");
		exit(-1);
	}
#endif	
}

double getangle(double x1, double y1, double x2, double y2)
{
	double diffH = (x2 - x1);
	double diffV = (y2 - y1);

	double slope;
	double angle;
	double dgrs;

	if (diffH != 0.0)
	{
		slope = diffV / diffH ;
		angle = atan(slope);
		dgrs = ((angle * 180.0) / PI);

		if (diffH < 0.0)
			dgrs += 180.0;
	}
	else if (diffV < 0.0)
		dgrs = 270.0;
	else if (diffV > 0.0)
		dgrs = 90.0;
	else 
		dgrs = 0.0;
	/* .. het zal allemaal wel :) */
	if (dgrs < 0.0) 
		dgrs = 360.0 + dgrs;

	/* leuk maar moet 90 graden naar links ofzo, added:*/
	dgrs -= 90.0;

	if (dgrs < 0.0)
		dgrs = 360.0 - dabs(dgrs);
	else if (dgrs > 360.0)
		dgrs = 0.0 + dabs(dgrs - 360.0);

	return dgrs;
}



int valid_target(double x1, double y1, double x2, double y2, double precision)
{
int collided, radius, i;
double left_wing_deg, right_wing_deg; 
double lw_x, lw_y; /* leftwing x,y */
double rw_x, rw_y; /* rightwing x,y */

double tmp_x, tmp_y;
double cur_x, cur_y;
double deg, ret;

	cur_x    = x1;
	cur_y    = y1;
	radius   = BLOCKSIZE / 2;
	deg      = getangle(x2, y2, cur_x, cur_y);
	collided = 0;
	i        = 0;

	while (
		(dabs(cur_x - x2) > precision || dabs(cur_y - y2) > precision )
    )
	{
		//tmp_x = vtX(cur_x, deg);
		//tmp_y = vtY(cur_y, deg);
		tmp_x = _futureX(cur_x, deg, B_SPEED);
		tmp_y = _futureY(cur_y, deg, B_SPEED);
		
		/* We need some side coords */
		left_wing_deg = deg + 90;
		right_wing_deg = deg - 90;

			/* left wing */
		ret = (PI / 180) * (left_wing_deg - 90);
		lw_x = cos(ret);
		lw_x = lw_x * (radius / 2);
		lw_x = lw_x + radius;
		lw_x = lw_x + tmp_x - (BLOCKSIZE / 2);
		lw_y = sin(ret);
		lw_y = lw_y * (radius / 2);
		lw_y = lw_y + radius;
		lw_y = lw_y + tmp_y - (BLOCKSIZE / 2);

			/* right wing */
		ret = (PI / 180) * (right_wing_deg - 90);
		rw_x = cos(ret);
		rw_x = rw_x * (radius / 2);
		rw_x = rw_x + radius;
		rw_x = rw_x + tmp_x - (BLOCKSIZE / 2);
		rw_y = sin(ret);
		rw_y = rw_y * (radius / 2);
		rw_y = rw_y + radius;
		rw_y = rw_y + tmp_y - (BLOCKSIZE / 2);		

		/* Check if tmp_x/tmp_y collide with something :) */
		
		if (map[(int)(tmp_y / BLOCKSIZE)][(int)(tmp_x / BLOCKSIZE)] == 16384)
			collided = 1;

		if (map[(int)(lw_y / BLOCKSIZE)][(int)(lw_x / BLOCKSIZE)] == 16384)
			collided = 1;

		if (map[(int)(rw_y / BLOCKSIZE)][(int)(rw_x / BLOCKSIZE)] == 16384)
			collided = 1;

		/* temp added */
		if (map[(int)((tmp_y - (BLOCKSIZE / 4)) / BLOCKSIZE)][(int)((tmp_x - (BLOCKSIZE / 4)) / BLOCKSIZE)] == 16384)
		{
			collided = 1;
		}
		if (map[(int)((tmp_y - (BLOCKSIZE / 4)) / BLOCKSIZE)][(int)((tmp_x + (BLOCKSIZE / 4)) / BLOCKSIZE)] == 16384)
		{
			collided = 1;
		}
		if (map[(int)((tmp_y + (BLOCKSIZE / 4)) / BLOCKSIZE)][(int)((tmp_x + (BLOCKSIZE / 4)) / BLOCKSIZE)] == 16384)
		{
			collided = 1;
		}
		if (map[(int)((tmp_y + (BLOCKSIZE / 4)) / BLOCKSIZE)][(int)((tmp_x - (BLOCKSIZE / 4)) / BLOCKSIZE)] == 16384)
		{
			collided = 1;
		}

		cur_x = tmp_x;
		cur_y = tmp_y;

		if (collided == 1)
			return 1;	
	}

	return 0;
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

void cleanstr(char *str)
{
char *p;
	for (p=str; *p; p++)
		if ((*p < ' ') || (*p > 127))
		{
			*p = '\0';
			break;
		}
}

void verbose(char *pattern, ...) 
{
	char buf[512];
	
	va_list ap;
	va_start (ap, pattern);
	vsnprintf(buf, 512, pattern, ap);
	va_end(ap);

	printf_("%s\n", buf);
	fflush(stdout);
}
void die(char *s)
{
	printf_("%s", s);
// 	delay(1);
	exit(-1);
}
