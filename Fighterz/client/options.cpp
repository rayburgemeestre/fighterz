#include "common.h"

int getoptions()
{
	int i;

	time_proc();

	option[0].x = 300;
	option[0].y = 280;
	strcpy(option[0].str, "MULTIPLAYER");
	option[0].time = ourtime;
	option[0].r = 255;
	option[0].g = 255;
	option[0].b = 255;

	option[1].x = 300;
	option[1].y = 310;
	strcpy(option[1].str, "SINGLE PLAYER *");
	option[1].time = ourtime;
	option[1].r = 255;
	option[1].g = 255;
	option[1].b = 255;

	option[2].x = 300;
	option[2].y = 370;
	strcpy(option[2].str, "OPTIONS *");
	option[2].time = ourtime;
	option[2].r = 255;
	option[2].g = 255;
	option[2].b = 255;

	clear_to_color(tmpscreen, makecol(0,0,0));
	draw_sprite(tmpscreen, (BITMAP *)dataf[MAINMENU].dat, 125, 60);

	while (1)
	{
		char k;

		time_proc();

		if (keypressed())
		{
			k = (readkey() >> 8);
			if (k == KEY_DOWN)
			{
				play_sample((SAMPLE *)df_snd[FAST_CONFIRM].dat, 255, 128, 1000, 0);
				if (active_o_item < OPTION_COUNT)
					active_o_item++;
			}    
			if (k == KEY_UP)
			{
				play_sample((SAMPLE *)df_snd[FAST_CONFIRM].dat, 255, 128, 1000, 0);
				if (active_o_item > 0)
					active_o_item--;
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
			}
			textprintf(tmpscreen, (FONT *)dataf[ARCADE].dat, 
				option[i].x, option[i].y, makecol(option[i].r,option[i].g,option[i].b), 
				option[i].str);
				
		}
		acquire_screen();
		blit(tmpscreen, screen, 0, 0, 0, 0, SCREEN_X, SCREEN_Y);
		release_screen();
		rest(10);
		vsync();
	}
}