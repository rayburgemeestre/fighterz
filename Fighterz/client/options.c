#include "common.h"
//refactored 12 dec 2004

//functions
// int get_options();

//variables:
//--

//Options
struct option_ option[3]; /**< menu items in option menu */
int options_activeitem;   /**< Active item in the options menu */

//type definitions:
//--
typedef struct VTX	/* vertex data */
{
    fixed x, y, z;
} VTX;

//externals:
//--

//cube.cpp
extern void init_shape();
extern void animate_shape();
extern void translate_shape();
extern void quad(BITMAP *b, VTX *v1, VTX *v2, VTX *v3, VTX *v4);
extern void draw_shape(BITMAP *b);


//add_option macro used by get_options
#define add_option(_item, _x, _y, _text, _r, _g, _b, _flickering) \
	do { \
		option[_item].x = _x; \
		option[_item].y = _y; \
		strcpy(option[_item].str, _text); \
		option[_item].time = ourtime; \
		option[_item].r = _r; \
		option[_item].g = _g; \
		option[_item].b = _b; \
		option[_item].flicker = _flickering; \
	} while(0)

/* get_options(); 
 * displays option screen and returns chosen menu item 
 */
int get_options()
{
// for cube:
//--
int last_retrace_count;
// for menu items flickering effect:
//--
int i; // used to loop through option menu items
unsigned long flicker_time;
#define FLICKER_DELAY 100
//--

	//Initialize:
	time_proc(); //make sure we use have a current time
	flicker_time = ourtime; // Initialize flicker_time variable:

	//Add Option Menu items.
	add_option(0, 280, 280, "MULTIPLAYER",		255, 255, 255, 0);
	add_option(1, 280, 310, "SINGLE PLAYER *",	255, 255, 255, 0);
	add_option(2, 280, 370, "OPTIONS",			255, 255, 255, 0);

    //Cube initialization
	// set up the viewport for the perspective projection
    set_projection_viewport(0, 0, screensize_x, screensize_y);
    // initialise the bouncing shapes
    init_shape();
    last_retrace_count = retrace_count;
	//--

	while (1)
	{
	char k; // Used to store Keyboard input

		clear_to_color(tmpscreen, makecol(0,0,0)); /* clear "double" buffer */
		time_proc(); // update time every iteration

		//Prepare/Animate shape (the Cube) if necessary
		while (last_retrace_count < retrace_count)
		{
			animate_shape();
			last_retrace_count++;
		}
		translate_shape(); // 3D -> 2D

		//Prepare Menu item (flickering effect)
		if (ourtime - flicker_time <= FLICKER_DELAY)
			option[options_activeitem].flicker = 1;
		else if (ourtime - flicker_time > (1.5 *FLICKER_DELAY))
			flicker_time = ourtime;
		else if (ourtime - flicker_time > FLICKER_DELAY)
			option[options_activeitem].flicker = 0;
		
		//Handle Keyboard Input
		if (keypressed() && (k = (readkey() >> 8)))
		{
			if ((k == KEY_DOWN) && (options_activeitem < OPTION_COUNT))
			{
				play_sample((SAMPLE *)dat_sound[FAST_CONFIRM].dat, 128, 128, 1000, 0);
				options_activeitem++;
			}
			if ((k == KEY_UP) && (options_activeitem > 0))
			{
				play_sample((SAMPLE *)dat_sound[FAST_CONFIRM].dat, 128, 128, 1000, 0);
				options_activeitem--;
			}
			if (k == KEY_ENTER)
			{
				play_sample((SAMPLE *)dat_sound[CONFIRM].dat, 255, 128, 1000, 0);
				return options_activeitem; // return selected option
			}  
			if (k == KEY_ESC)
			{
				play_sample((SAMPLE *)dat_sound[CONFIRM].dat, 255, 128, 1000, 0);
				return -1; // return exit code
			}
		}

		//Draw Cube to tmpscreen
		draw_shape(tmpscreen);

		//Draw Fighterz logo to tmpscreen
		draw_sprite(tmpscreen, (BITMAP *)dat_base[MAINMENU].dat, 0, 0);

		//Draw Menu Items to tmpscreen (with colour/flickering)
		for (i=0; i<3; i++)
		{
		int diff = (ourtime - option[i].time), 
			tmp = 1, times = 0, cnt;

			while (diff >= tmp)
			{
				diff -= tmp;
				option[i].time += tmp;
				times += 1;
			}

			if (options_activeitem == i) {

				for (cnt = 0; cnt < times; cnt++)
					if (option[i].r > 0)
						option[i].r--;
					else if (option[i].g > 128)
						option[i].g--;

				if ((option[i].flicker == 0) && (option[i].r == 0 && option[i].g == 128)) {
					// Flickering: hide text
					textprintf(tmpscreen, (FONT *) dat_base[ARCADE].dat, option[i].x, option[i].y,
							   makecol(0, 0, 0), "%s", option[i].str);
				} else {
					// Flickering: show text
					textprintf(tmpscreen, (FONT *) dat_base[ARCADE].dat, option[i].x, option[i].y,
							   makecol(option[i].r, option[i].g, option[i].b), "%s", option[i].str);
				}

			} else {

				if (option[i].r < 255) {
					for (cnt = 0; cnt < times; cnt++)
						if (option[i].g < 255) {
							option[i].g++;
						} else {
							if (option[i].r < 255)
								option[i].r++;
						}
				} else {
					option[i].time = ourtime;
				}

				//show text
				textprintf(tmpscreen, (FONT *)dat_base[ARCADE].dat, option[i].x, option[i].y, 
					makecol(option[i].r,option[i].g,option[i].b), "%s", option[i].str);
			}
		}
		
		
		//Draw Double buffer to screen

		// acquire_screen();
		if (enable_stretch == 0 || (screensize_y == desktop_y && screensize_x == desktop_x)) {
			blit(tmpscreen, screen, 0, 0, 0, 0, screensize_x, screensize_y);
		} else {
			blit(tmpscreen, tmpscreen2, 0, 0, 0, 0, screensize_x, screensize_y);
			stretch_blit(tmpscreen2, screen, 0, 0, screensize_x, screensize_y, 0, 0, desktop_x, desktop_y);
		}
		// release_screen();
		// rest(10);
		vsync();
	}
}