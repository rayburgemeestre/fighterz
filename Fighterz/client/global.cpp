#include "common.h"

// FPS (frames per sec)
int fps; /**< current frames per seconds count */
int fps_count; /**< used to calculate FPS */
long int fps_oldtime; /**< used to calculate FPS */
long int fps_newtime; /**< used to calculate FPS */

// our time
struct timeb started; /**< time the game started (clientside) */
struct timeb t; /**< used to calculate 'ourtime' in millisecs */
unsigned long ourtime; /**< our timer used for all time calculations in millisecs */

/********************************************
*  GLOBAL FUNCTIONS
****************************************************/

int bg_music_on = 0;
int save_frames_to_bitmap = 0;

/* WARNING: vsnprintf() wasn't available 
   char buf[512] could be overflowed */
void printff_direct(char *pattern, ...) 
{
int clr = makecol(0,128,255);

	if (game_started == 1 || our_spawnstatus == 1)
		return;

	char buf[512];
	
	va_list ap;
	va_start (ap, pattern);
	vsprintf(buf, pattern, ap);
	va_end(ap);

	acquire_screen();

	if (fakeconsole_y >= (screensize_y - 10)) {
		fakeconsole_y = 0;
		clear_to_color(screen, makecol(0, 0, 0));
	} else {
		fakeconsole_y = fakeconsole_y + 10;
	}

	if (fakeconsole_y == 0) { 
		textprintf(screen, font, 10, 1, clr, buf);
	} else { 
		textprintf(screen, font, 10, fakeconsole_y, clr, buf);
	}

	release_screen();

}

void parse_input()
{
int newturn = 0;

if (!our_node)
	return;

	if (our_node->dead == 0 || our_node->invincible == 1) 
	{
		// left ctrl or 'z' for fire
		if ((key[KEY_LCONTROL]||key[KEY_Z]) && our_node->invincible != 1 && 
			our_node->dead != 2)
		{	/* Requesting fire */
			if ( (bullet_fire_time == 0) || ((ourtime - bullet_fire_time) >= (unsigned)our_bullet_autofiredelay) )
			{	
				bullet_fire_time = ourtime;

				// SUPER FIRE (5 BULLETS)
				if (key[KEY_Z] && (our_bullet_count < our_bullet_max) && (our_bullet_count <= 5))
				{
				struct data* pRet;
				int i;
					our_bullet_autofiredelay = 1200;

					for (i=0; i<5; i++)
					{
					int deg = our_node->deg + 360;
					#define TWIST 4

						switch (i) {
							case 0: deg -= TWIST*2; break;
							case 1: deg -= TWIST; break;
							case 2: break;
							case 3: deg += TWIST; break;
							case 4: deg += TWIST*2; break;
						}
						
						deg = deg % 360;

						pRet = add_bullet(our_node, our_node->x, our_node->y, deg, ourtime);						
						send_newbullet(pRet->x, pRet->y, pRet->deg);
					}
					bullet_fire_time = ourtime;
					
					our_bullet_count += 5;
					
					play_sample((SAMPLE *)dat_sound[SHOOT].dat, 50, 128, 1000, 0);
				}
				// NORMAL FIRE
				else if (key[KEY_LCONTROL] && (our_bullet_count < our_bullet_max))
				{	
					struct data* pRet;
					/* add */
					our_bullet_autofiredelay = 150;

						// play_sample((SAMPLE *)dat_base[FATALITY].dat, 255, 128, 1000, 0);

	
					pRet = add_bullet(our_node, our_node->x, our_node->y, our_node->deg, ourtime);
					bullet_fire_time = ourtime;
					our_bullet_count++;
					send_newbullet(pRet->x, pRet->y, pRet->deg);
					play_sample((SAMPLE *)dat_sound[SHOOT].dat, 50, 128, 1000, 0);
				}
			} 
		} else {
			// UNCOMMENT THE FOLLOWING IF our_bullet_autofiredelay SHOULD BE RESET ON
			// FIRE KEY RELEASE::::::::::::::

			/* if ( (ourtime - bullet_fire_time) < (unsigned)our_bullet_autofiredelay)
				bullet_fire_time = 0; */
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
				
				our_direction = 1;

				oldvel = our_node->velocity;

				ret = collidecheck2(our_node->id, 1, 0);

				if (ret != 1 || DONT_LOSE_VELOCITY_AT_COLLISION) 
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

				our_direction = 0;
				oldvel = our_node->velocity;

				ret = collidecheck2(our_node->id, 1, 0);

				if (ret != 1 || DONT_LOSE_VELOCITY_AT_COLLISION)
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

	if (talk_mode == 1 ||  talk_mode == 2)
	{
		if (key[KEY_ESC])
		{
			talk_mode = 0;
			msg[0] = '\0';
			clear_keybuf();
		}
	}

	if (key[KEY_T])
		if (talk_mode == 0)
		{
			talk_mode = 1;
			msg[0] = '\0';
			clear_keybuf();
		}
	if (key[KEY_X])
		our_node->deg++;
	if (key[KEY_TILDE]) // '~'
	{

	}
	if (key[KEY_STOP])
		if (talk_mode == 0)
		{
			talk_mode = 2;
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

		if (talk_mode == 1)
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
					talk_mode = 0;
				}
			}
			else if (strlen(msg) < 60)
			{
				msg[strlen(msg) + 1] = '\0';
				msg[strlen(msg)] = k2;
			}
		} 
		else if (talk_mode == 2)
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
					talk_mode = 0;
				}
			}
			else if (strlen(msg) < MSG_LINE_LENGTH)
			{
				msg[strlen(msg) + 1] = '\0';
				msg[strlen(msg)] = k2;
			}
		} 
		else {
			if ( k == KEY_1 ) { our_node->shiptype = 1; send_newship(); }
			if ( k == KEY_2 ) { our_node->shiptype = 2; send_newship(); }
			if ( k == KEY_3 ) { our_node->shiptype = 3; send_newship(); }
			if ( k == KEY_4 ) { our_node->shiptype = 4; send_newship(); }
			if ( k == KEY_5 ) { our_node->shiptype = 5; send_newship(); }

			if ( k == KEY_P )
			{
			char nam[80]; sprintf(nam, "screen%lu.pcx", ourtime);
			PALETTE pal;			
				get_pallete(pal);
				save_bitmap(nam, tmpscreen, pal);
			}
			if ( k == KEY_O ) {
				save_frames_to_bitmap = save_frames_to_bitmap == 1 ? 0 : 1;
			}
			if ( k == KEY_H )
			{
				send_cmd("addbot");
			}
		//	if ( k == KEY_C )
		//		while (1);
			if ( k == KEY_R )
			{
				if (show_radar == 1)
					show_radar = 0;
				else
					show_radar = 1;
			}
			if ( k == KEY_B && false )
			{
				add_explosion(our_node->x, our_node->y, 100, 10, makecol(255,0,0));
			}
			if ( k == KEY_Y )
			{
			int i;
				for (i=0; i<map_blocks_y; i++)
				{
					verbose(field[i]);
				}
			}
			if ( k == KEY_F8 )
			{
				bg_music_on = 1;
				stop_midi();
			}
			if ( k == KEY_F9 )
			{
				bg_music_on = 0;
				stop_midi();
			}
			if ( k == KEY_F11 /*&& false*/ )
			{
				if (mod_bounce == 1)
				{
					large_text("Normal!");
					mod_bounce = 0;
					our_bullet_max = 10;
					//FIX!!!!!!!! SPEED = 0.20;
					bullet_movespeed = 0.40;
	                our_bullet_ttl = 3000;
	                our_bullet_autofiredelay = 150;
					// local test (temp):
					if (our_node->speed < our_node->max_speed)
						our_node->speed = our_node->max_speed;
				}
				else
				{
					large_text("Bullet time!");
					mod_bounce = 1;
					our_bullet_max = 20;
					//SPEED = 0.05;
					bullet_movespeed = 0.10;
	                our_bullet_ttl = 6000; /* was 3000; */
	                our_bullet_autofiredelay = 600;
					// local test (temp):
					if (our_node->speed > our_node->max_speed)
						our_node->speed = our_node->max_speed;
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
				if (show_scoreboard == 1)
					show_scoreboard = 0;
				else
					show_scoreboard = 1;
			}
			if ( k == KEY_F5 )
			{
				if (show_names == 1)
					show_names = 0;
				else
					show_names = 1;
			}
			if ( k == KEY_F6 )
			{
				if (show_fps == 1)
					show_fps = 0;
				else
					show_fps = 1;
			}
			if ( k == KEY_F3 )
			{
				if (high_gfx == 1)
				{
					addtext("*** SMOOTH GFX: OFF");
					high_gfx = 0;
				}
				else
				{
					addtext("*** SMOOTH GFX: ON");
					high_gfx = 1;
				}
			}
			if ( k == KEY_F4 )
			{
				if (fullscreen == 1)
				{
					fullscreen = 0;
				}
				else
				{
					fullscreen = 1;
				}

				set_color_depth(16);
				//set_display_switch_mode(SWITCH_BACKGROUND);
				init_screen();	
			}
			if ( k == KEY_G )
			{
				if (mod_grid == 1)
					mod_grid = 0;
				else
					mod_grid = 1;
				
				draw_map();
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

	if (game_started)
		addtext("%s", buf);
	else
		printff_direct("%s", buf);

	return;
}

/* Prints userlist */
int scoreboard_current_y; /* scoreboard current y-position on which the next nickname should be printed */
void printulist() 
{
	if (show_scoreboard != 1)
		return;

	draw_sprite(bmp_scoreboard, (BITMAP *)dat_base[GUI_SCOREBOARD_BG].dat, 0, 0);

	scoreboard_current_y = scoreboard_padding_top;
	current = head;
	
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

	int color;
	
	if (node->invincible == 2)
		color = makecol(0, 135, 185); // invincible
	else if (node->invincible == 1)
		color = makecol(0, 156, 241); // respawned 
	else if (node->dead == 1)
		color = makecol(144, 144, 144); // dead
	else if (node->dead == 3)
		color = makecol(0, 49, 67); // spectate
	else
		color = makecol(0, 186, 255); // normal (alive)
	
	sprintf(buf, "%d", node->kills);

	if (scoreboard_current_y == 0) { 
		alfont_textout_aa_ex(bmp_scoreboard, lcdfont, node->nick, scoreboard_padding_left, 1, color, -1);
		alfont_textout_right_aa_ex(bmp_scoreboard, lcdfont, buf, scoreboard_w - scoreboard_padding_right, 1, color, -1);
	} else { 
		alfont_textout_aa_ex(bmp_scoreboard, lcdfont, node->nick, scoreboard_padding_left, scoreboard_current_y, color, -1);
		alfont_textout_right_aa_ex(bmp_scoreboard, lcdfont, buf, scoreboard_w - scoreboard_padding_right, scoreboard_current_y, color, -1);
	}

	scoreboard_current_y = scoreboard_current_y + 18;

}


/********************************************
*  GLOBAL PROCEDURES
****************************************************/

/* exit program */
void terminate()
{
	alert("Terminated", "", "", "Ok", NULL, 0, 0);
	// delay(3);
	alfont_destroy_font(lcdfont);
	alfont_destroy_font(tccmfont);
	alfont_exit();
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
			"----------------+-----------------+-----+----------\n",debugfile);
		for (current=head; current; current=current->next)
			if (current->bullet != 1)
			{
				fprintf(debugfile, "%-15lu | %-15d | %-3u | %-12s %s\n", ourtime, current, current->id, current->nick, (our_id == current->id?"<-- our id":""));
				fflush(debugfile);
			}
		fputs("\n",debugfile);
		dtime = ourtime;
	}
#endif 
}
void draw_fps()
{
char buf[512];

	if (show_fps == 1) 
	{
/*		textprintf(tmpscreen, font, 2, 2, makecol(255,255,255), 
			"FPS: %d LAG: %2.2f VEL: %d SPD: %.2f FRE:%d POW:%d SOCK:%d map-w:%d h:%d             ", 
			fps, current_lag(), our_node->velocity, our_node->speed, 
			our_node->freeze, our_node->power, theSocket,
				field_width, field_height
		);
*/
// 		textprintf(tmpscreen, (FONT *)dat_base[NOKIA].dat, 2, 2, makecol(128, 128, 128), 
//			"FPS: %d LAG: %2.2f MIDItrack:%d   ", 
//			fps, current_lag(), midi_track
//		);

		// info: time
		sprintf(buf, "TIME: %lu", ourtime);
		alfont_textout_aa_ex(bmp_command, lcdfont, buf, info_time_x, info_time_y, makecol(255, 255, 255), -1);

		// info: lag
		sprintf(buf, "LAG: %2.2f", current_lag());
		alfont_textout_aa_ex(bmp_command, lcdfont, buf, info_lag_x, info_lag_y, makecol(255, 255, 255), -1);

		// info: fps
		sprintf(buf, "FPS: %d", fps);
		alfont_textout_aa_ex(bmp_command, lcdfont, buf, info_fps_x, info_fps_y, makecol(255, 255, 255), -1);
	}
}

void show_graphics()
{
	large_text_draw();

	// show_mouse(tmpscreen);
	poll_mouse();	

	//acquire_screen();
	blit(tmpscreen, screen, 0, 0, 0, 0, screensize_x, screensize_y);
	if (save_frames_to_bitmap == 1)
	{
	char nam[80]; sprintf(nam, "D:\\fighterz\\%lu.pcx", ourtime);
	PALETTE pal;
	
		get_pallete(pal);
		save_bitmap(nam, tmpscreen, pal);
	}
	//release_screen();

	if (high_gfx == 1)
	{
		// rest(8);
		vsync(); /* wait for vertical retrace (can reduce flickering) 
					though a few fps slower :( */
	}
	/* poll_keyboard(); *//* This shouldn't be necessary in Windows. */
}