#include "common.h"
//refactored 12 dec 2004

//Include AGUP External C header files
#include "agup/agup.h"
#include "agup/aphoton.h"
//#include "agup/ans.h"

//functions
//--
// void get_settings();

//variables
//--
//Dialog
#define  bg_color 0xFFFFFF /* dialog background colour */
DIALOG settings[] =
{
   /* (proc)                 (x)  (y)  (w)  (h)  (fg) (bg)                    (key) (flags) (d1) (d2) (dp)                                 (dp2) (dp3) */
   { d_agup_shadow_box_proc, 108, 132, 600, 312, 0,   0,        0,    0,      0,   0,   NULL,                                NULL, NULL },
   { d_agup_button_proc,     120, 408, 60,  24,  0,   0,        0,  D_EXIT,   0,   0,   "Back",                              NULL, NULL },
   { d_agup_shadow_box_proc, 444, 144, 252, 252, 0,   0,        0,    0,      0,   0,   NULL,                                NULL, NULL },
   { d_text_proc,            456, 156, 228, 12,  0,   bg_color, 0,    0,      0,   0,   "CONTROLS (are static)",             NULL, NULL },
   { d_text_proc,            456, 180, 228, 12,  0,   bg_color, 0,    0,      0,   0,   "LEFT:       arrow key left",        NULL, NULL },
   { d_text_proc,            456, 192, 228, 12,  0,   bg_color, 0,    0,      0,   0,   "RIGHT:      arrow key right",       NULL, NULL },
   { d_text_proc,            456, 204, 228, 12,  0,   bg_color, 0,    0,      0,   0,   "SPEED UP:   arrow key up",          NULL, NULL },
   { d_text_proc,            456, 216, 228, 12,  0,   bg_color, 0,    0,      0,   0,   "SPEED DOWN: arrow key down",        NULL, NULL },
   { d_text_proc,            456, 228, 228, 12,  0,   bg_color, 0,    0,      0,   0,   "FIRE:       left control",          NULL, NULL },
   { d_text_proc,            456, 240, 228, 12,  0,   bg_color, 0,    0,      0,   0,   "TURN FAST:  left shift",            NULL, NULL },
   { d_agup_shadow_box_proc, 120, 240, 312, 156, 0,   0,        0,    0,      0,   0,   NULL,                                NULL, NULL },
   { d_agup_check_proc,      132, 264, 288, 24,  0,   0,        0,    0,      1,   0,   "Retro graphics (ingame: 'f3')",     NULL, NULL },
   { d_text_proc,            132, 252, 288, 12,  0,   bg_color, 0,    0,      0,   0,   "GFX Details",                       NULL, NULL },
   { d_agup_check_proc,      132, 288, 288, 24,  0,   0,        0,    0,      1,   0,   "Radar (ingame: 'r')",               NULL, NULL },
   { d_agup_check_proc,      132, 312, 288, 24,  0,   0,        0,    0,      1,   0,   "PlayerList (ingame: 'f2')"        , NULL, NULL },
   { d_agup_check_proc,      132, 336, 288, 24,  0,   0,        0,    0,      1,   0,   "Nicknames in field (ingame: 'f5')", NULL, NULL },
   { d_agup_check_proc,      132, 360, 288, 24,  0,   0,        0,    0,      1,   0,   "FPS/lag (ingame: 'f6')",            NULL, NULL },
   { d_agup_shadow_box_proc, 120, 144, 312, 84,  0,   0,        0,    0,      0,   0,   NULL,                                NULL, NULL },
   { d_agup_check_proc,      132, 168, 120, 24,  0,   0,        0,    0,      1,   0,   "Fullscreen",                        NULL, NULL },
   { d_agup_check_proc,      132, 192, 120, 24,  0,   0,        0,    0,      1,   0,   "Vsync/rest",                        NULL, NULL },
   { d_text_proc,            132, 156, 144, 12,  0,   bg_color, 0,    0,      0,   0,   "GFX",                               NULL, NULL },
   { d_agup_check_proc,      264, 192, 156, 24,  0,   0,        0,    0,      1,   0,   "Background Music",                  NULL, NULL },
   { d_agup_check_proc,      264, 168, 144, 24,  0,   0,        0,    0,      1,   0,   "Sound FX",                          NULL, NULL },
   { d_text_proc,            264, 156, 144, 12,  0,   bg_color, 0,    0,      0,   0,   "Sound Options",                     NULL, NULL },
   { NULL,                   0,   0,   0,   0,   0,   0,        0,    0,      0,   0,   NULL,                                NULL, NULL }
};

//external
//--
//type definitions:
typedef struct VTX		 /* vertex data */
{
    fixed x, y, z;
} VTX;
//cube functions:
extern void init_shape();
extern void animate_shape();
extern void translate_shape();
extern void quad(BITMAP *b, VTX *v1, VTX *v2, VTX *v3, VTX *v4);
extern void draw_shape(BITMAP *b);

//get_settings(): [...]
void get_settings()
{
//cube:
//--
int last_retrace_count;
#define bg_color makecol(216, 216, 216)
//dialog:
DIALOG_PLAYER *ds;

	//Initialization:
	gui_bg_color = makecol(255,0,0);
	clear_to_color(tmpscreen, makecol(0,0,0));

	//Cube Initialization
    set_projection_viewport(0, 0, SCREEN_W, SCREEN_H);
    init_shape();
    last_retrace_count = retrace_count;
	//--

	//Agup Initialization
	agup_init(aphoton_theme);
	ds = init_dialog(settings, -1);
	//--

	//cube [static/non moving background image ;), 
	//      update_dialog etc didn't seem to work 
	//      properly in combination with agup]
	while (last_retrace_count < retrace_count)
	{
		animate_shape();
		last_retrace_count++;
	}
	translate_shape(); // 3D -> 2D
	draw_shape(tmpscreen); //Draw

	//Draw Options Text
	textprintf_centre(tmpscreen, (FONT *)dat_base[ARCADE].dat, 
		SCREEN_W / 2, 100, 
		makecol(255, 255, 255), 
		"OPTIONS [Currently doesn't work]");
	
	//--
	blit(tmpscreen, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); 
	do_dialog(settings, -1);

	//while (key[KEY_ESC]);
}