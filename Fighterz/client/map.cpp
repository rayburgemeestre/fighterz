#include "common.h"

char field[500][500]; /**< array of chars that represent the map (e.g.:'1'=block,'2'=redflag,etc.) */
int map_blocks_y = 0; /**< vertical block count of the map as rcvd from serv */
int map_blocks_x = 0; /**< horizontal block count of the map as rcvd from serv */
int blocksize = 20; /**< Size of blocks in pixels (also the ships are made using this size) */
unsigned long field_width; /**< field width in pixels */
unsigned long field_height; /**< field height in pixels */
struct bg_imgs bg_imgs_data[32]; /**< background images on map container */
int bg_imgs_data_index; /**< current index variable used to fill the container */

double scale = 1.0; /**< a block is 1 pixel on the radar */
int radar_field_bmp_w, radar_field_bmp_h; /**< radar field buffer size */

int red_spawn_x, red_spawn_y;
int blue_spawn_x, blue_spawn_y;

int red_flag_def_x, red_flag_def_y; /**< red flag default location */
int blue_flag_def_x, blue_flag_def_y; /**< red flag default location */
int red_flag_x, red_flag_y; /**< red flag location */
int blue_flag_x, blue_flag_y; /**< blue flag location */
int red_flag_carrier = -1; /**< redflag carrier */
int blue_flag_carrier = -1; /**< blueflag carrier */

int x_on_map = 1; /**< x-position we display from the mapfield bmp buffer (source for blit) */
int y_on_map = 1; /**< y-position we display from the mapfield bmp buffer (source for blit) */

/********************************************
*  MAP FUNCTIONS
****************************************************/

// tmp here

void plot_datimg(struct bg_imgs bg)
{
char df_path[512];
DATAFILE *df;

	sprintf(df_path, "system\\%s", bg.datfile);

	if (!(df = load_datafile(df_path)))
	{
		printff_direct("Could not specified datafile (%s) its images will not be displayed", df_path);
	} else {
	int index = 0, i = 0;
	DATAFILE *ptr = df;

		printff_direct("Loaded datafile....%s", df_path);

		while (ptr->type != DAT_END)
		{
			if (ptr->type == DAT_BITMAP)
			{
				if (bg.df_id == index)
				{
					// plot it
					draw_sprite(bmp_mapfield, (BITMAP *)df[index].dat, bg.pos_x, bg.pos_y);
				}
				index++;
			}
			ptr++;
			i++;
		}
	}	
}

/* procedure to draw map on screen */
void draw_map() 
{
	int cnt, cnt2;
	int x = 0, length, length2;

	/* Colours */
	int blauw = makecol(0,128,255);
	int grijs = makecol(128,128,128);

	destroy_bitmap(bmp_mapfield);
	bmp_mapfield = create_bitmap(field_width + 1, field_height + 1);
//	bmp_radarfield = create_bitmap(RADAR_W, RADAR_H);

	clear_to_color(bmp_mapfield, 0);
//	clear_to_color(bmp_radarfield, 0);
	clear_to_color(tmpscreen, 0);

	/*	
		bmp_mapfield = create_bitmap(screensize_x, screensize_y);
		clear_to_color (bmp_mapfield, 0);
	*/
	
	length = map_blocks_x * blocksize;
	length2  = map_blocks_y * blocksize;

	/* stars */
	for (cnt=0; cnt<map_blocks_x; cnt++) 
	{
		for (cnt2=0; cnt2<map_blocks_y; cnt2++)
		{
			{
				int ret;
				if (!mod_grid)
				{
					/* temporary stars :) */
					ret = 1+(int) (15*rand()/(RAND_MAX+1.0));
					
					if (ret == 1)
						draw_sprite(bmp_mapfield, (BITMAP *)dat_base[STARS1].dat, (cnt * blocksize), (cnt2 * blocksize));
					else if (ret == 2)
						draw_sprite(bmp_mapfield, (BITMAP *)dat_base[STARS2].dat, (cnt * blocksize), (cnt2 * blocksize));
					else if (ret == 2)
						draw_sprite(bmp_mapfield, (BITMAP *)dat_base[STARS3].dat, (cnt * blocksize), (cnt2 * blocksize));
					else if (ret == 4)
						draw_sprite(bmp_mapfield, (BITMAP *)dat_base[STARS4].dat, (cnt * blocksize), (cnt2 * blocksize));
				}
			}
		}
	}


	{
	int i;

		for (i=0; i < 32; i++)
		{
			if (strlen(bg_imgs_data[i].datfile) > 0)
			{
				if (!mod_grid)
					plot_datimg(bg_imgs_data[i]);
			}
		}		
	}

	if (mod_grid)
	{
		/* Draw lines */
		/*   (horizontal:) */
		for (cnt=0; cnt<map_blocks_y; cnt++)
		{
			line(bmp_mapfield, 0, (cnt * blocksize), length, (cnt * blocksize), grijs);
		}
		line(bmp_mapfield, 0, (cnt * blocksize), length, (cnt * blocksize), grijs);

		/*   (vertical:) */
		for (cnt=0; cnt<map_blocks_x; cnt++)
		{
			line(bmp_mapfield, (cnt * blocksize), 0, (cnt * blocksize), length2, grijs);
		}
		line(bmp_mapfield, (cnt * blocksize), 0, (cnt * blocksize), length2, grijs);
	}

	/* Draw walls */
	for (cnt=0; cnt<map_blocks_x; cnt++) 
	{
		for (cnt2=0; cnt2<map_blocks_y; cnt2++)
		{
			if (field[cnt2][cnt] == '1')
			{
				if (mod_grid)
					rectfill(bmp_mapfield, (cnt * blocksize), (cnt2 * blocksize), 
					((cnt + 1) * blocksize), ((cnt2 + 1) * blocksize), 
					( mod_grid ? blauw : makecol(128, 128, 128)) ); // was blauw, nu weer :)
				else
					draw_sprite(bmp_mapfield, (BITMAP *)dat_base[WALL].dat, (cnt * blocksize), (cnt2 * blocksize) );
			}
			/* Draw red team spawn location */
			if (field[cnt2][cnt] == '2') 
			{
				red_spawn_x = cnt * blocksize;
				red_spawn_y = cnt2 * blocksize;
				rect(bmp_mapfield, cnt * blocksize + 1, cnt2 * blocksize + 1, (cnt + 1) * blocksize - 1, (cnt2 + 1) * blocksize - 1, makecol(255,0,0));
			}
			/* Draw blue team spawn location */
			if (field[cnt2][cnt] == '3') 
			{
				blue_spawn_x = cnt * blocksize;
				blue_spawn_y = cnt2 * blocksize;
				rect(bmp_mapfield, cnt * blocksize + 1, cnt2 * blocksize + 1, (cnt + 1) * blocksize - 1, (cnt2 + 1) * blocksize - 1, makecol(0,0,255));
			}
			/* Draw red teams flag location */
			if (field[cnt2][cnt] == '4') 
			{
				red_flag_x = cnt * blocksize;
				red_flag_y = cnt2 * blocksize;
				red_flag_def_x = red_flag_x;
				red_flag_def_y = red_flag_y;
				rect(bmp_mapfield, red_flag_x + 1, red_flag_y + 1, (cnt + 1) * blocksize - 1, (cnt2 + 1) * blocksize - 1, makecol(255,128,0));
			}
			/* Draw blue teams flag location */			
			if (field[cnt2][cnt] == '5') 
			{
				blue_flag_x = cnt * blocksize;
				blue_flag_y = cnt2 * blocksize;
				blue_flag_def_x = blue_flag_x;
				blue_flag_def_y = blue_flag_y;
				rect(bmp_mapfield, cnt * blocksize + 1, cnt2 * blocksize + 1, (cnt + 1) * blocksize - 1, (cnt2 + 1) * blocksize - 1, makecol(0,128,255));
			}
		}
	}

	// DRAW RADARBUFF
	{

		radar_field_bmp_w = (map_blocks_x+1) * scale; /* TODO: blocksize / radar_scale i.p.v. scale? */
		radar_field_bmp_h = (map_blocks_y+1) * scale;

		bmp_radarfield = create_bitmap(radar_field_bmp_w, radar_field_bmp_h);
		clear_to_color(bmp_radarfield, makecol(0,0,0));
		//RADARBG 

		addtext("scale = %.2f, %d", scale, (int)scale);

		for (cnt=0; cnt<map_blocks_x; cnt++) 
		{
			for (cnt2=0; cnt2<map_blocks_y; cnt2++)
			{
				if (field[cnt2][cnt] == '1') 
				{
					rectfill(bmp_radarfield, cnt * scale,  cnt2 * scale, 
						(cnt + 1) * scale, (cnt2 + 1) * scale, 
						makecol(53, 18, 22));
				}
			}
		}
	}

	blit_gui();
}

void draw_radar()
{
// flicker effect on radar
static unsigned long prev_radar_flickertime = 0;
// scanner effect on radar
static unsigned long prev_radar_scannertime = 0;
static double deg = 0;
int on = 1; // effect 1=on 2=off

	if (show_radar != 1)
	{
		return;
	}
	else
	{
	PLAYER tmp;
	static BITMAP *a_redbar = NULL;

		if (a_redbar == NULL)
		{
			a_redbar = create_bitmap(radar_bar_w, radar_bar_h);
			draw_sprite(a_redbar, (BITMAP *)dat_base[GUI_RADAR_REDBAR].dat, 0, 0);			
		}

		double _power, _bullets;

		if (!our_node)
			return;

		// flickering
		if ( (prev_radar_flickertime == 0) || 
			 ((ourtime - prev_radar_flickertime) >= 100) )
		{
			prev_radar_flickertime = ourtime;
			on = on == 1 ? 2 : 1;
		}
		//scanner
		while ( (prev_radar_scannertime == 0) || 
			 ((ourtime - prev_radar_scannertime) >= 10) )
		{
			prev_radar_scannertime += 10;
			deg += 4;
			if (deg > 360.0)
				deg -= 360.0;
		}

		_power = (our_node->power * 100) / ship_maxpower;
		_bullets = ((our_bullet_max - our_bullet_count) * 100) / our_bullet_max;

		/* Power indicator */
		blit(a_redbar, bmp_radar, 
			0, 0, 
			radar_powerbar_x, radar_powerbar_y, 
			radar_bar_w, radar_bar_h * ((100.0 - _power) / 100.0)
		);

		/* Weapon indicator */
		blit(a_redbar, bmp_radar, 
			0, 0, 
			radar_bulletbar_x, radar_bulletbar_y, 
			radar_bar_w, radar_bar_h * ((100.0 - _bullets) / 100.0)
		);

		{
		int x, y, i;
		int radar_scale = blocksize / scale;

			tmp = head;
			while (tmp)
			{
				x = (tmp->x / radar_scale) - radar_padding_x;
				y = (tmp->y / radar_scale) - radar_padding_y;

				if (tmp->bullet != 1)
				{
					if (tmp->bot == 1 && 
						getdistance(tmp->x, tmp->y, our_node->x, our_node->y) >
						(RADAR_SENSOR/2)*blocksize )
					{
						; // to far away to detect with radar
						  // don't 'continue' because the tmp = tmp->next
						  // needs to be executed.
					}
					else if (tmp->id == our_id)
					{
					int radius;
					int x2, y2;
					int color;

						if (on == 1)
							color = makecol(255,255,255);
						else if (on == 2)
							color = makecol(0, 0, 0);

						// show radar range
						radius = ((RADAR_SENSOR/2)*blocksize)/(blocksize/scale); // RADAR_SENSOR(50) blocks in diameter = half: (25 * blocksize) / (blocksize / scale)
							     //                                   500pixels / 20 = 25pixels radius

						circle(bmp_radar_display, x, y, radius, makecol(43,37,56));
						circle(bmp_radar_display, x, y, radius - 0.5, makecol(43-20,37-20,56-20));
						deg -= 0.25 * 80.0;
						for (i=1; i<=80; i++)
						{
							x2 = X2(x, y, deg, radius); 
							y2 = Y2(x, y, deg, radius);
							line(bmp_radar_display, x, y, x2, y2, makecol(43+i,37+i,56+i));
							deg += 0.25;
						}
						//circlefill(bmp_radar_display, x, y, 1, color);
					}
					else
					{
						circlefill(bmp_radar_display, x, y, 1, on == 1 ? makecol(128,255,128) : makecol(0,0,0));
					}
				}
				tmp = tmp->next;
			}
		}
	}
}

unsigned long map_scroll;
void set_map_coords()
{
//if (our_node == NULL)
//	return;

	if (  ((our_node == NULL) && (game_started == 1)) ||
		  ((our_node != NULL) && (our_node->dead == 2))
	   )
	{
		if ((ourtime - map_scroll) >= 10)
		{
			map_scroll = ourtime;
			
			if (key[KEY_LEFT])
				x_on_map -= 10;
			if (key[KEY_RIGHT])
				x_on_map += 10;
			if (key[KEY_UP])
				y_on_map -= 10;
			if (key[KEY_DOWN])
				y_on_map += 10;
		}
	} else {
		x_on_map = our_node->x - field_w / 2;
		y_on_map = our_node->y - field_h / 2;
	}
	
	if (x_on_map < 0 || field_width <= (unsigned)field_w) 
		x_on_map = 0;
	if ((unsigned)x_on_map > ((field_width + 1) - field_w)) 
		x_on_map = (field_width + 1) - field_w;
	
	
	if (y_on_map < 0 || field_height <= (unsigned)field_h ) 
		y_on_map = 0;
	if ((unsigned)y_on_map > ((field_height + 1) - field_h)) 
		y_on_map = (field_height + 1) - field_h;

}

