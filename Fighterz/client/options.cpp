#include "common.h"

int getoptions()
{
int i;
unsigned long flicker_time;
#define FLICKER_DELAY 100

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
	strcpy(option[2].str, "OPTIONS *");
	option[2].time = ourtime;
	option[2].r = 255;
	option[2].g = 255;
	option[2].b = 255;
	option[0].flicker = 0;

	clear_to_color(tmpscreen, makecol(0,0,0));
	draw_sprite(tmpscreen, (BITMAP *)dataf[MAINMENU].dat, 0, 0);

	play_sample((SAMPLE *)df_snd[FAST_CONFIRM].dat, 255, 128, 1000, 0);

	flicker_time = ourtime;
	while (1)
	{
		char k;

		time_proc();

		if (ourtime - flicker_time <= FLICKER_DELAY)
		{
			circle(tmpscreen, 10, 10, 10, 10);
			
			option[active_o_item].flicker = 1;
		} else if (ourtime - flicker_time > (1.5 *FLICKER_DELAY))
		{
			flicker_time = ourtime;

		} else if (ourtime - flicker_time > FLICKER_DELAY)
		{
			option[active_o_item].flicker = 0;
		}

		if (keypressed())
		{
			k = (readkey() >> 8);
			if (k == KEY_DOWN)
			{
				if (active_o_item < OPTION_COUNT)
				{
					play_sample((SAMPLE *)df_snd[FAST_CONFIRM].dat, 255, 128, 1000, 0);
					active_o_item++;
				}
			}    
			if (k == KEY_UP)
			{
				
				if (active_o_item > 0)
				{
					play_sample((SAMPLE *)df_snd[FAST_CONFIRM].dat, 255, 128, 1000, 0);
					active_o_item--;
				}
			}
			if (k == KEY_ENTER)
			{
				play_sample((SAMPLE *)df_snd[CONFIRM].dat, 255, 128, 1000, 0);
				return active_o_item;
			}  
			if (k == KEY_ESC)
			{
				play_sample((SAMPLE *)df_snd[CONFIRM].dat, 255, 128, 1000, 0);
				return -1; /* die :) */
			}   			
		}

		for (i=0; i<3; i++)
		{
			if (active_o_item == i)
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
					textprintf(tmpscreen, (FONT *)dataf[ARCADE].dat, 
						option[i].x, option[i].y, makecol(0, 0, 0), 
						option[i].str);				
				}
				else
				{
					/*char buf[512];
					sprintf(buf, "flicker on @ %lu %d %d %d ", ourtime, ((int)ourtime - flicker_time), option[i].r, option[i].g);
					textprintf(tmpscreen, font, 100, 100, makecol(255,255,255), buf);*/
					textprintf(tmpscreen, (FONT *)dataf[ARCADE].dat, 
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
				textprintf(tmpscreen, (FONT *)dataf[ARCADE].dat, 
					option[i].x, option[i].y, makecol(option[i].r,option[i].g,option[i].b), 
					option[i].str);
			}
		}
		acquire_screen();
		blit(tmpscreen, screen, 0, 0, 0, 0, SCREEN_X, SCREEN_Y);
		release_screen();
		rest(10);
		vsync();
	}
}