#include "common.h"

//graphics
BITMAP *tmpscreen; /**< bitmap used for double buffering (primary bitmap) */
BITMAP *bmp_mapfield; /**< bitmap that contains the rendered map as rcvd from serv */
BITMAP *bmp_radarfield; /**< bitmap that contains the rendered radar as rcvd from serv */
BITMAP *bmp_shipfield; /**< subbitmap on tmpscreen for ships & bullets */
BITMAP *bmp_console; /**< subbitmap on tmpscreen for chat-console a.k.a. partyline */
BITMAP *bmp_command; /**< subbitmap on tmpscreen for command console */
BITMAP *bmp_radar; /**< subbitmap on tmpscreen for radar (including powerbars etc) */ 
BITMAP *bmp_radar_display; /**< subbitmap on radar for radarfield (only the field-part) */
BITMAP *bmp_scoreboard; /**< subbitmap on tmpscreen for scoreboard */
//fonts
ALFONT_FONT *lcdfont; /**< reference to lcdn.ttf for text in e.g.: scoreboard */
ALFONT_FONT *tccmfont; /**< reference to tccm.ttf for e.g.: centered text on screen */ 

//positioning
int field_x, field_y; /* field position */
int field_w, field_h; /* field size */
int header_x, header_y; /* header position */
int footer_x, footer_y; /* footer position */
int scoreboard_x, scoreboard_y; /* scoreboard position */
int scoreboard_w, scoreboard_h; /* scoreboard size */
int scoreboard_padding_top;
int scoreboard_padding_left;
int scoreboard_padding_right;
int header_mask1_x, header_mask1_y; /* header mask 1 position */
int footer_mask1_x, footer_mask1_y; /* footer mask 1 position */
int console_x, console_y; /* console position */
int console_w, console_h; /* console size */
int command_x, command_y; /* command position */
int command_w, command_h; /* command size */
int radar_x, radar_y; /* radar position */
int radar_w, radar_h; /* radar size */
int radar_field_x, radar_field_y; /* radar field position on radar subbitmap */
int radar_field_w, radar_field_h; /* radar field size */
int radar_shiptype_x, radar_shiptype_y; /* radar: shiptype location */
int radar_redflag_x, radar_redflag_y; /* radar: redflag ico location */
int radar_blueflag_x, radar_blueflag_y; /* radar: blueflag ico location */
int radar_powerbar_x, radar_powerbar_y; /* radar: power bar location */
int radar_bulletbar_x, radar_bulletbar_y; /* radar: bullet bar location */
int radar_bar_w, radar_bar_h; /* radar: bullet/power bar size */
int info_time_x, info_time_y; /* information: time position */
int info_lag_x, info_lag_y; /* information: lag position */
int info_fps_x, info_fps_y; /* information: fps position */

//more dynamic settings
int show_fps = 1; /**< boolean: 0=don't,1=do show FPS */
int show_scoreboard = 1; /**< boolean: 0=don't,1=do show scoreboard */
int show_names = 1; /**< boolean: 0=don't,1=do show ingame nicknames */
int show_radar; /**< boolean: 0=don't,1=do show radar on screen */
char console[CON_LINES][CON_LINE_LENGTH]; /**< buffer for lines of console */

void init_gui() 
{
	// field
	field_x = 12;
	field_y = 61;
	field_w = 780;
	field_h = 400;

	// console
	console_x = 14;
	console_y = 490;
	console_w = 564;
	console_h = 94;

	// command
	command_x = 15;
	command_y = 8;
	command_w = 563;
	command_h = 36;

	// radar
	radar_x = 626;
	radar_y = 502;
	radar_w = 175;
	radar_h = 99;

	// radar_field
	radar_field_x = 56;
	radar_field_y = 5;
	radar_field_w = 100 - 2;
	radar_field_h = 50 - 2;

	// radar: shiptype
	radar_shiptype_x = 5;
	radar_shiptype_y = 63;
	// radar: redflag
	radar_redflag_x = 36;
	radar_redflag_y = 62;
	radar_blueflag_x = 36;
	radar_blueflag_y = 76;
	//radar: powerbar
	radar_powerbar_x = 12;
	radar_powerbar_y = 4;
	//radar: bulletbar
	radar_bulletbar_x = 36;
	radar_bulletbar_y = 4;
	radar_bar_w = 11;
	radar_bar_h = 50;

	// scoreboard
	scoreboard_x = 640;
	scoreboard_y = 69;
	scoreboard_w = 134;
	scoreboard_h = 383;
	scoreboard_padding_top = 7;
	scoreboard_padding_left = 5;
	scoreboard_padding_right = 5;

	// header = 800x60px
	header_x = 1;
	header_y = 1;

	// footer = 800x140px
	footer_x = 1;
	footer_y = 460;

	// header_mask1 = 152x8px
	header_mask1_x = 631;
	header_mask1_y = 61;

	// footer_mask1 = 152x9px
	footer_mask1_x = 631;
	footer_mask1_y = 452 - 1;

	// information drawn in command buffer
	// fps etc..
	info_time_x = 422 - command_x;
	info_time_y = 12 - command_y - 5;

	info_lag_x = 512 - command_x;
	info_lag_y = 12 - command_y - 5;

	info_fps_x = 512 - command_x;
	info_fps_y = 29 - command_y - 5;

}

/* gui stuff that needs be blit only once on tmpscreen */
void blit_gui()
{
	draw_sprite(tmpscreen, (BITMAP *)dat_base[GUI_HEADER].dat, header_x, header_y);
	draw_sprite(tmpscreen, (BITMAP *)dat_base[GUI_FOOTER].dat, footer_x, footer_y);
}

/* gui stuff that needs be blit every iteration! */
void blit_gui_iter()
{
	draw_sprite(tmpscreen, (BITMAP *)dat_base[GUI_HEADER_MASK1].dat, header_mask1_x, header_mask1_y);
	draw_sprite(tmpscreen, (BITMAP *)dat_base[GUI_FOOTER_MASK1].dat, footer_mask1_x, footer_mask1_y);
}