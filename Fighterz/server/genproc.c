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
					{	/* init lag stuff done, ready to respawn! */
						SetOk(current);
						send_spawnready(current);
						current->dead = 2; /* mark ready to respawn */
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
#if 1 == 2
	/* asdfasdf */
	if (keypressed())			
	{
		char k = readkey() & 0xff;
		if ( k == 'h')
		{
			struct data *bot;
			// findpath(head);
			bot = add_bot();
			notify_of_newuser(EVERYONE, NULL, bot);
			flyto((bot_id - 1), REDFLAG.x, REDFLAG.y);
			// addtext("2: %d", head->bot);
			//}
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

	drawconsole();

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
	masked_blit(shipbuff, tmpscreen, MAP_X, MAP_Y, FIELD_X, FIELD_Y, MAP_W, MAP_H);

	if (show_fps == 1 && head != NULL) 
		textprintf(tmpscreen, font, 1, 1, 15, "FPS: %d spd:%.2f lg:%2.2f bc:%du", fps, (head != NULL?head->speed:0.0), (head != NULL?current_lag(head->id):0.0), head->bulletcnt);
			/* shows: fps, speed, lag, bulletcount.. */

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

	SPEED = 0.20;
	// lag_check = 0;
	player_nr = 0;
	LAG_CHECKS = 5; 
	LAG_DELAY = 100;
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
	MAX_HITS = 10;
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
	strncpy(map3, "maps/lvl1.txt", 64);
	map3[64] = '\0';
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

	cur_x = x1;
	cur_y = y1;

//	fprintf(juub, "IN THE BEGINNING: X: %.2f Y:%.2f\n", cur_x, cur_y);

	radius = BLOCKSIZE / 2;

	deg = getangle(x2, y2, cur_x, cur_y);
//	fprintf(juub, "I> x: %.2f y: %.2f x: %.2f y: %.2f deg: %.2f\n", x2, y2, cur_x, cur_y, deg);

//	circlefill(fieldbuff, 970, 110, 22, makecol(255,255,255));

	collided = 0;
	//fprintf(juub, "abs(%.2f - %.2f) = %d\n", cur_x, x2, abs(cur_x - x2) );
	//fprintf(juub, "abs(%.2f - %.2f) = %d\n", cur_y, y2, abs(cur_y - y2) );

	i = 0;

	/* werkt alleen in 'we are 4' XXXXXXZ */

	/* KEEP */
	//circlefill(fieldbuff, x2 - field_width, y2 - field_height, 5, makecol(0,255,temprrr));

	temprrr += 5;

//	circle(fieldbuff, x1, y1, 10, makecol(255,0,0));

							/* was BLOCKSIZE / 2 */
	while (
		(dabs(cur_x - x2) > precision || dabs(cur_y - y2) > precision )
    )
	{

//fprintf(juub, "1> x: %.2f y: %.2f deg: %.2f\n", cur_x, cur_y, deg);

	//	i++;
	//	if (i == 100)
	//		exit(-1);


/*if (element > 0)
{
	blit (fieldbuff, tmpscreen, MAP_X, MAP_Y, FIELD_X, FIELD_Y, MAP_W, MAP_H);
	blit (tmpscreen, screen, 0, 0, 0, 0, SCREEN_X, SCREEN_Y);
	alert("","","","","",1,1);
}*/
		
		
		/* if (deg == 270)
		{

			char buf[500];

	blit (fieldbuff, tmpscreen, MAP_X, MAP_Y, FIELD_X, FIELD_Y, MAP_W, MAP_H);
	blit (tmpscreen, screen, 0, 0, 0, 0, SCREEN_X, SCREEN_Y);

			sprintf(buf, "x1:%.2f y1:%.2f x2:%.2f y2:%.2f", x1, y1, x2, y2);
		alert(buf, "","","","",1,1);
		} */

		//addtext("%.2f <---", deg);

		//fprintf(juub, "ATM x is: %.2f y is:%.2f\n", cur_x, cur_y);
		tmp_x = vtX(cur_x, deg);

		//fprintf(juub, "BEFORE y is: %.2f \n", cur_y);
		tmp_y = vtY(cur_y, deg);
		//fprintf(juub, "AFTER y is: %.2f \n", tmp_y);

//fprintf(juub, "abs(%.2f - %.2f) = %d\n", tmp_x, x2, abs(tmp_x - x2) );
//fprintf(juub, "abs(%.2f - %.2f) = %d\n", tmp_y, y2, abs(tmp_y - y2) );

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
		
		// circle(tmpscreen, tmp_x, tmp_y, 5, makecol(255, 255, 255));
		//fprintf(juub, "t> x: %.2f y: %.2f deg: %.2f\n", tmp_x, tmp_y, deg);
		/* Werkt nu dus alleen in een 'We are 4' situatie :) XXXXXXZ */
		//putpixel(fieldbuff, tmp_x - field_width, tmp_y - field_height , makecol(255,0,0));
		//putpixel(fieldbuff, lw_x - field_width, lw_y - field_height, makecol(0,255,0));
		//putpixel(fieldbuff, rw_x - field_width, rw_y - field_height, makecol(0,0,255));


	//	fprintf(juub, "  -> %d - %d - %d\n", (int)(tmp_x / BLOCKSIZE), (int)(tmp_y / BLOCKSIZE), 
	//		map[(int)(tmp_y / BLOCKSIZE)][(int)(tmp_x / BLOCKSIZE)]	
	//	);
		
		//fprintf(juub, "2> x: %.2f y: %.2f deg: %.2f\n", cur_x, cur_y, deg);
		if (map[(int)(tmp_y / BLOCKSIZE)][(int)(tmp_x / BLOCKSIZE)] == 16384)
		{
			collided = 1;
			//fprintf(juub, "!COLLIDED!1\n");
			//circlefill(fieldbuff, tmp_x, tmp_y, 11, makecol(0,0,255));
			//circlefill(fieldbuff, tmp_x - field_width, tmp_y, 11, makecol(0,0,255));
		}
		if (map[(int)(lw_y / BLOCKSIZE)][(int)(lw_x / BLOCKSIZE)] == 16384)
		{
			collided = 1;
			//fprintf(juub, "!COLLIDED!2\n");
		}
		if (map[(int)(rw_y / BLOCKSIZE)][(int)(rw_x / BLOCKSIZE)] == 16384)
		{
			collided = 1;
			//fprintf(juub, "!COLLIDED!3\n");
		}
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
/* /end */
		/*if (map[(int)((tmp_y + (BLOCKSIZE / 2)) / BLOCKSIZE)][(int)((tmp_x + (BLOCKSIZE / 2)) / BLOCKSIZE)] == 16384)
			collided = 1;
		if (map[(int)((lw_y + (BLOCKSIZE / 2)) / BLOCKSIZE)][(int)((lw_x + (BLOCKSIZE / 2)) / BLOCKSIZE)] == 16384)
			collided = 1;
		if (map[(int)((rw_y + (BLOCKSIZE / 2)) / BLOCKSIZE)][(int)((rw_x + (BLOCKSIZE / 2)) / BLOCKSIZE)] == 16384)
			collided = 1;
			*/

		cur_x = tmp_x;
		cur_y = tmp_y;

		if (collided == 1)
			return 1;

		

			//map_draw_path2();

	}

	//fprintf(juub, "RETURNING\n");
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
