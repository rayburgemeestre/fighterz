#include "common.h"

/********************************************
*  GLOBAL FUNCTIONS
****************************************************/

/* WARNING: vsnprintf() wasn't available 
   char buf[512] could be overflowed */
void printff_direct(char *pattern, ...) 
{
	if (STARTED == 1)
		return;

	char buf[512];
	
	va_list ap;
	va_start (ap, pattern);
	vsprintf(buf, pattern, ap);
	va_end(ap);

	acquire_screen();

	if (TXTPTR >= (SCREEN_Y - 10)) {
		TXTPTR = 0;
		clear_to_color (screen, 0);
	} else {
		TXTPTR = TXTPTR + 10;
	}

	if (TXTPTR == 0) { 
		textprintf(screen, font, 10, 1, makecol(255,255,255), buf);
	} else { 
		textprintf(screen, font, 10, TXTPTR, makecol(255,255,255), buf);
	}

	release_screen();

}

void parse_input()
{
int newturn = 0;
	if (our_node->dead == 0 || our_node->invincible == 1) 
	{
		if (key[KEY_LCONTROL] && our_node->invincible != 1)
		{	/* Requesting fire */
			if ( (bullet_time == 0) || ((ourtime - bullet_time) >= (unsigned)BULLET_RE) )
			{	bullet_time = ourtime;
				if (BULLET_COUNT < BULLET_MAX)
				{	
					struct data* pRet;
					/* add */

#if NOSOUND != 1
					if (USE_SOUND == 1)
						// play_sample((SAMPLE *)dataf[FATALITY].dat, 255, 128, 1000, 0);
#endif
	
					pRet = add_bullet(our_node, our_node->x, our_node->y, our_node->deg, ourtime);
					bullet_time = ourtime;
					BULLET_COUNT++;
					send_newbullet(pRet->x, pRet->y, pRet->deg);
				}
			} 
		} else {
			if ( (ourtime - bullet_time) < (unsigned)BULLET_RE)
				bullet_time = 0;
		}


		/***** TURN (left/right) HANDLING HERE ****/
		
		if (key[KEY_RIGHT])
		{
			/*  spin right */
			if (key[KEY_LSHIFT])
				newturn = 4;
			else
				newturn = 2;
		}
		if (key[KEY_LEFT])
		{
			/*  spin left */
			if (key[KEY_LSHIFT])
				newturn = -4;
			else
				newturn = -2;
		}

		if (newturn != our_node->turn)
		{
			/* Update! :) */
			our_node->turn = newturn;
			send_turn();
		}
		// EONewcode

		if (key[KEY_UP] || key[KEY_DOWN]) 
		{
			if (key[KEY_UP])
			{
				int oldvel;
				int ret = 0;
				
				direction_is_up = 1;

				oldvel = our_node->velocity;

				ret = collidecheck2(our_node->id, 1);

				if (ret != 1) 
				{ 
					/* this wouldn't cause a collition*/
					if (our_node->velocity == -1) our_node->velocity = 0;
					else if (our_node->velocity == 0) our_node->velocity = 1;

					/* notify server */
					if (oldvel != our_node->velocity)
						send_accel(5);

				} else {
					our_node->velocity = 0;
					if (oldvel != our_node->velocity)
						send_accel(6);
				}
			}
			if (key[KEY_DOWN] && !key[KEY_UP])
			{
				int oldvel;
				int ret = 0;

				direction_is_up = 0;
				oldvel = our_node->velocity;

				ret = collidecheck2(our_node->id, 1);

				if (ret != 1) 
				{ 
					if (our_node->velocity == 1) our_node->velocity = 0;
					else if (our_node->velocity == 0) our_node->velocity = -1;

					/* notify server */
					if (oldvel != our_node->velocity)
						send_accel(1);

				} else {
					our_node->velocity = 0;
					if (oldvel != our_node->velocity)
						send_accel(2);
				}
			}
		} else {
			if (our_node->velocity != 0)
			{
				our_node->velocity = 0;
				send_accel(3);
			}
		}
	}	

	if (talk == 1 || talk == 2)
	{
		if (key[KEY_ESC])
		{
			talk = 0;
			msg[0] = '\0';
			clear_keybuf();
		}
	} else {
		if (!key[KEY_Q] && !key[KEY_P])
			zoom_time = 0;

		if (key[KEY_P])
		{
			if (zoom_time == 0)
			{
				zoom_time = ourtime;
				zoom_(1);
			} else {
				if ( (ourtime - zoom_time) >= 1000)
				{
					zoom_time = ourtime;
					zoom_(1);
				}
			}
		}
		if (key[KEY_Q])
		{
			if (zoom_time == 0)
			{
				zoom_time = ourtime;
				zoom_(0);
			} else {
				if ( (ourtime - zoom_time) >= 1000)
				{
					zoom_time = ourtime;
					zoom_(0);
				}
			}
		}
	}

	if (key[KEY_T])
		if (talk == 0)
		{
			talk = 1;
			msg[0] = '\0';
			clear_keybuf();
		}
	if (key[KEY_X])
		our_node->deg++;
	if (key[KEY_TILDE]) // '~'
	{

	}
	if (key[KEY_STOP])
		if (talk == 0)
		{
			talk = 2;
			msg[0] = '\0';
			clear_keybuf();
		}

	while (keypressed())
	{
		int ret;
		char k, k2;

		ret = readkey();
		
		k = (ret >> 8);
		k2 = (ret & 0xff);

		if (talk == 1)
		{
			if (k == KEY_BACKSPACE)
			{
				if (strlen(msg) > 0)
					msg[strlen(msg)-1] = '\0';
			}
			else if ( k == KEY_ENTER )
			{
				if (strlen(msg) > 0)
				{
					if (msg[0] != '\0')
						send_say(msg);
					msg[0] = '\0';
					talk = 0;
				}
			}
			else if (strlen(msg) < 60)
			{
				msg[strlen(msg) + 1] = '\0';
				msg[strlen(msg)] = k2;
			}
		} 
		else if (talk == 2)
		{
			if (k == KEY_BACKSPACE)
			{
				if (strlen(msg) > 0)
					msg[strlen(msg)-1] = '\0';
			}
			else if ( k == KEY_ENTER )
			{
				if (strlen(msg) > 0)
				{
					if (msg[0] != '\0')
					{
						addtext("[%s@fighterz]# %s", our_node->nick, msg);
						send_cmd(msg);
					}
					msg[0] = '\0';
					talk = 0;
				}
			}
			else if (strlen(msg) < 60)
			{
				msg[strlen(msg) + 1] = '\0';
				msg[strlen(msg)] = k2;
			}
		} 
		else {
			if ( k == KEY_H )
			{
				//addtext("<%s> %s", our_node->nick, ".addbot");
				//sockwrite("T test\n");
				//msg[0] = '\0';
				//talk = 0;
			}
		//	if ( k == KEY_C )
		//		while (1);
			if ( k == KEY_R )
			{
				if (RADAR_SHOW == 1)
					RADAR_SHOW = 0;
				else
					RADAR_SHOW = 1;
			}
			if ( k == KEY_B && false )
			{
				explosion(our_node->x, our_node->y, 100, 10, makecol(255,0,0));
			}
			if ( k == KEY_F11 /*&& false*/ )
			{
				if (BOUNCING_BULLETS == 1)
				{
					BOUNCING_BULLETS = 0;
					BULLET_MAX = 10;
					B_SPEED = 0.40;
	                BULLET_TTL = 3000;
	                BULLET_RE = 150;
				}
				else
				{
					BOUNCING_BULLETS = 1;
					BULLET_MAX = 100;
					B_SPEED = 0.10;
	                BULLET_TTL = 6000; /* was 3000; */
	                BULLET_RE = 50;
				}
			}
			if ( k == KEY_F1 )
			{
				char *t1 = "F1 = this help                ";
				char *t2 = "F2 = show/hide player list    ";
				char *t3 = "F3 = smooth graphics on/off   ";
				char *t4 = "F4 = fullscreen on/off        ";
				char *t5 = "F5 = player names on/off      ";
				char *t6 = "F6 = fps on/off               ";
				char *t7 = "F12 = exit game               ";
				char *t8 = "R = radar on/off              ";
				char *t9 = "T = talk                      ";
				char *t10 = "G = grid on/off              ";
				char *t11 = ".nick <nick> = to change nick";

				alert(t1, t2, t3, "More", NULL, KEY_ENTER, 0);
				alert(t4, t5, t6, "More", NULL, KEY_ENTER, 0);
				alert(t7, t8, t9, "More", NULL, KEY_ENTER, 0);
				alert(t10, NULL, t11, "Ok", NULL, KEY_ENTER, 0);
			}
			if ( k == KEY_F2 )
			{
				if (show_ulist == 1)
				{
					show_ulist = 0;
					clear_to_color(tmpscreen, makecol(0,0,0));
				}
				else
					show_ulist = 1;
			}
			if ( k == KEY_F5 )
			{
				if (show_names == 1)
				{
					show_names = 0;
				}
				else
					show_names = 1;
			}
			if ( k == KEY_F6 )
			{
				if (show_fps == 1)
				{
					clear_to_color(tmpscreen, makecol(0,0,0));
					show_fps = 0;
				}
				else
					show_fps = 1;
			}
			if ( k == KEY_F3 )
			{
				if (HIGH_GRAPHICS == 1)
				{
					addtext("*** SMOOTH GFX: OFF");
					HIGH_GRAPHICS = 0;
				}
				else
				{
					addtext("*** SMOOTH GFX: ON");
					HIGH_GRAPHICS = 1;
				}
			}
			if ( k == KEY_F4 )
			{
				if (FULLSCREEN == 1)
				{
					FULLSCREEN = 0;
				}
				else
				{
					FULLSCREEN = 1;
				}

				set_color_depth(16);
				//set_display_switch_mode(SWITCH_BACKGROUND);
				init_screen();	
			}
			if ( k == KEY_G )
			{
				if (grid == 1)
					grid = 0;
				else
					grid = 1;
				
				drawmap();
			}
		}
	}
}

void verbose(char *pattern, ...) 
{
	char buf[512];
	
	va_list ap;
	va_start (ap, pattern);
	vsprintf(buf, pattern, ap);
	va_end(ap);

	if (STARTED)
		addtext("%s", buf);
	else
		printff_direct("%s", buf);

	return;
}

/* Prints userlist */
void printulist() 
{
	if (show_ulist != 1)
		return;

	TXTPTR2 = 20;
	current = head;
	clear_to_color(ulistbuff, makecol(0,0,0));
	/* rect(ulistbuff, 0, 0, 150 - 1, (MAP_H + 5 + CSCREEN_H) - 1, makecol(27, 27, 27)); */
	textprintf(ulistbuff, font, 1, 1, makecol(255,255,255), "Player List:");
	while (current != NULL)
	{
		if (current->bullet != 1)
			printul(current);

		current = current->next;
	}
	
}

/* Used in printulist(); */
void printul(struct data *node) 
{
	char buf[128];
	/* again no snprintf().. arghl*/
	if (strlen(node->nick) > 10) node->nick[10] = '\0'; /* frag >10 */
	sprintf(buf, "%8s %4d", node->nick, node->kills);

	TXTPTR2 = TXTPTR2 + 10;
	
	int color;
	
	if (node->invincible == 2)
		color = makecol(255, 255, 255); // invincible
	else if (node->invincible == 1)
		color = makecol(0, 255, 128); // respawned 
	else if (node->dead == 1)
		color = makecol(128, 64, 0); // dead
	else if (node->dead == 3)
		color = makecol(128, 128, 128); // spectate
	else
		color = makecol(255, 128, 0); // normal (alive)
		

	if (TXTPTR2 == 0) { 
		//textprintf(tmpscreen, font, LEFT_2, 1, 42, buf); /* 42 = makecol(255, 128, 0) (orange) */
		textprintf(ulistbuff, font, 0, 1, color, buf); 
	} else { 
		//textprintf(tmpscreen, font, LEFT_2, TXTPTR2, 42, buf);
		textprintf(ulistbuff, font, 0, TXTPTR2, color, buf);
	}
}


/********************************************
*  GLOBAL PROCEDURES
****************************************************/

/* exit program */
void terminate() {
	delay(3);
	allegro_exit();
	WSACleanup();
	exit(-1); 
}

/* delay proc */
void delay(int secs) {
	long int oldtime, newtime;
	
	oldtime = newtime = time(NULL);
	
	while ( (newtime - oldtime) != secs ) {
		newtime = time(NULL);	
	}
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

/* time_proc */
void time_proc() 
{
	//char buf[128];

	ftime(& t);
	/* this var is used in the entire program */
	ourtime = (( (t.time - started.time) * 1000) + ( (t.millitm - started.millitm ) / 1));
	//sprintf(buf, "%u", ourtime  );
	/* debug: */
	//textprintf(tmpscreen, font, LEFT + 10, (SCREEN_Y - 10), 15, buf);
}

void debug()
{
#if DEBUG == 1
	static unsigned long dtime = ourtime;
	// dtime = ourtime;

	if ((ourtime - dtime) > 5000) /* ~5 secs delay */
	{
		fputs("Clients time:   | Addr. of node:  | Id: | Nickname:\n"
			"----------------+-----------------+-----+----------\n",dbug);
		for (current=head; current; current=current->next)
			if (current->bullet != 1)
			{
				fprintf(dbug, "%-15lu | %-15d | %-3u | %-12s %s\n", ourtime, current, current->id, current->nick, (our_id == current->id?"<-- our id":""));
				fflush(dbug);
			}
		fputs("\n",dbug);
		dtime = ourtime;
	}
#endif 
}
void drawfps()
{
	if (show_fps == 1) 
		textprintf(tmpscreen, font, 2, 2, makecol(255,255,255), 
			"FPS: %d LAG: %2.2f VEL: %d SPD: %.2f FRE:%d POW:%d SOCK:%d map-w:%d h:%d             ", 
			fps, current_lag(), our_node->velocity, our_node->speed, 
			our_node->freeze, our_node->power, theSocket,
				field_width, field_height
		);
}

void show_graphics()
{
	large_text_draw();
	masked_blit(shipbuff, tmpscreen, MAP_X, MAP_Y, FIELD_X, FIELD_Y, MAP_W, MAP_H);
		/* ^- if hardware supports this, then this goes fast */

	// show_mouse(tmpscreen);
	poll_mouse();	

	if (STARTED == 1)
	{
		acquire_screen();
		blit (tmpscreen, screen, 0, 0, 0, 0, SCREEN_X, SCREEN_Y);
		release_screen();
	}
	if (HIGH_GRAPHICS == 1)
	{
		rest(8);
		vsync(); /* wait for vertical retrace (can reduce flickering) */
	}
	/* poll_keyboard(); *//* This shouldn't be necessary in Windows. */
}