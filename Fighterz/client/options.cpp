#include "common.h"

//Options
struct option_ option[3]; /**< menu items in option menu */

//cube.cpp
extern void init_shape();
extern void animate_shape();
extern void translate_shape();
typedef struct VTX		 /* vertex data */
{
    fixed x, y, z;
} VTX;
extern void quad(BITMAP *b, VTX *v1, VTX *v2, VTX *v3, VTX *v4);
extern void draw_shape(BITMAP *b);

int options_activeitem; /**< Active item in the options menu */

int getoptions()
{
int i;
unsigned long flicker_time;
#define FLICKER_DELAY 100
// for cube:
int last_retrace_count;
//--

	time_proc();

	option[0].x = 280;
	option[0].y = 280;
	strcpy(option[0].str, "MULTIPLAYER");
	option[0].time = ourtime;
	option[0].r = 255;
	option[0].g = 255;
	option[0].b = 255;
	option[0].flicker = 0;

	option[1].x = 280;
	option[1].y = 310;
	strcpy(option[1].str, "SINGLE PLAYER *");
	option[1].time = ourtime;
	option[1].r = 255;
	option[1].g = 255;
	option[1].b = 255;
	option[0].flicker = 0;

	option[2].x = 280;
	option[2].y = 370;
	strcpy(option[2].str, "OPTIONS");
	option[2].time = ourtime;
	option[2].r = 255;
	option[2].g = 255;
	option[2].b = 255;
	option[0].flicker = 0;

	play_sample((SAMPLE *)dat_sound[FAST_CONFIRM].dat, 255, 128, 1000, 0);

	flicker_time = ourtime;


	// cube stuff:
    /* set up the viewport for the perspective projection */
    set_projection_viewport(0, 0, SCREEN_W, SCREEN_H);
    /* initialise the bouncing shapes */
    init_shape();
    last_retrace_count = retrace_count;
	//--
	while (1)
	{
		char k;

		clear_to_color(tmpscreen, makecol(0,0,0));
		time_proc();

		//cube
		while (last_retrace_count < retrace_count)
		{
			animate_shape();
			last_retrace_count++;
		}
		translate_shape();
		draw_shape(tmpscreen);

		draw_sprite(tmpscreen, (BITMAP *)dat_base[MAINMENU].dat, 0, 0);

		//blit(tmpscreen, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); 
		//--

		if (ourtime - flicker_time <= FLICKER_DELAY)
		{
			circle(tmpscreen, 10, 10, 10, 10);
			
			option[options_activeitem].flicker = 1;
		} else if (ourtime - flicker_time > (1.5 *FLICKER_DELAY))
		{
			flicker_time = ourtime;

		} else if (ourtime - flicker_time > FLICKER_DELAY)
		{
			option[options_activeitem].flicker = 0;
		}

		if (keypressed())
		{
			k = (readkey() >> 8);
			if (k == KEY_DOWN)
			{
				if (options_activeitem < OPTION_COUNT)
				{
					play_sample((SAMPLE *)dat_sound[FAST_CONFIRM].dat, 255, 128, 1000, 0);
					options_activeitem++;
				}
			}    
			if (k == KEY_UP)
			{
				
				if (options_activeitem > 0)
				{
					play_sample((SAMPLE *)dat_sound[FAST_CONFIRM].dat, 255, 128, 1000, 0);
					options_activeitem--;
				}
			}
			if (k == KEY_ENTER)
			{
				play_sample((SAMPLE *)dat_sound[CONFIRM].dat, 255, 128, 1000, 0);
				return options_activeitem;
			}  
			if (k == KEY_ESC)
			{
				play_sample((SAMPLE *)dat_sound[CONFIRM].dat, 255, 128, 1000, 0);
				return -1; /* die :) */
			}   			
		}

		for (i=0; i<3; i++)
		{
			if (options_activeitem == i)
			{
				int diff, tmp, times, cnt;

				diff = (ourtime - option[i].time);

				tmp = 1;
				times = 0;

				while (diff >= tmp)
				{
					diff -= tmp;
					option[i].time += tmp;
					times += 1;
				}

				for (cnt=0; cnt<times; cnt++)
				{
					if (option[i].r > 0)
						option[i].r--;
					else
						if (option[i].g > 128)
							option[i].g--;
				}
				if (option[i].flicker == 0 && (option[i].r == 0 && option[i].g == 128))
				{
					textprintf(tmpscreen, (FONT *)dat_base[ARCADE].dat, 
						option[i].x, option[i].y, makecol(0, 0, 0), 
						option[i].str);				
				}
				else
				{
					/*char buf[512];
					sprintf(buf, "flicker on @ %lu %d %d %d ", ourtime, ((int)ourtime - flicker_time), option[i].r, option[i].g);
					textprintf(tmpscreen, font, 100, 100, makecol(255,255,255), buf);*/
					textprintf(tmpscreen, (FONT *)dat_base[ARCADE].dat, 
						option[i].x, option[i].y, makecol(option[i].r,option[i].g,option[i].b), 
						option[i].str);
				}
			} else {
				if (option[i].r < 255)
				{
					int diff, tmp, times, cnt;

					diff = (ourtime - option[i].time);

					tmp = 1;
					times = 0;

					while (diff >= tmp)
					{
						diff -= tmp;
						option[i].time += tmp;
						times += 1;
					}

					for (cnt=0; cnt<times; cnt++)
					{
						if (option[i].g < 255)
							option[i].g++;
						else 
							if (option[i].r < 255)
								option[i].r++;
					}
				} else {
					option[i].time = ourtime;
				}
				textprintf(tmpscreen, (FONT *)dat_base[ARCADE].dat, 
					option[i].x, option[i].y, makecol(option[i].r,option[i].g,option[i].b), 
					option[i].str);
			}
		}
		acquire_screen();
		blit(tmpscreen, screen, 0, 0, 0, 0, screensize_x, screensize_y);
		release_screen();
		rest(10);
		vsync();
	}
}