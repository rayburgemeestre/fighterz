/********************************************
*  VARIABLES
****************************************************/

extern MIDI *bg_music;
extern MIDI *intro_music;


extern int screensize_y;
extern int screensize_x;
extern int fullscreen;
extern int high_gfx;
extern int mod_bounce;
extern int ship_movespeed_multiplier;

extern int fakeconsole_y;


extern unsigned int our_id;

extern int game_started;
#if DEBUG == 1
extern FILE *debugfile;
#endif
extern DATAFILE *dat_base;
extern DATAFILE *dat_sound;
extern unsigned long bullet_fire_time;
extern int our_bullet_count;
extern int our_bullet_max;
extern int our_bullet_ttl;
extern int our_bullet_autofiredelay;
extern int bullet_movespeed_multiplier;

//extern unsigned int CONSOLE_X;
//extern unsigned int CONSOLE_Y;
//extern unsigned int CONSOLE_W; 
//extern unsigned int CONSOLE_H; 




extern unsigned int ship_maxpower;
extern int talk_mode;

extern char msg[MSG_LINE_LENGTH];

extern char our_nickname[64];
extern int mod_grid;
extern int our_direction;



extern int our_spawnstatus;
extern int our_spawnrequested;
extern int terminate_request;

extern char large_text_msg[64];
extern unsigned long large_text_time;
extern unsigned int large_text_display_time;

extern int midi_track;
extern int bg_music_on;


//objects.cpp

//bullets
extern double bullet_movespeed;
extern PLAYER head;
extern PLAYER new_node;
extern PLAYER current;
extern PLAYER our_node;
extern EXPLOSION expl_head;
extern EXPLOSION expl_new;
extern EXPLOSION expl_current;


//map.cpp
extern char field[500][500];
extern int map_blocks_y;
extern int map_blocks_x;
extern int blocksize;
extern unsigned long field_width;
extern unsigned long field_height;
extern struct bg_imgs bg_imgs_data[32];
extern int bg_imgs_data_index;
//--
extern double scale;
extern int radar_field_bmp_w, radar_field_bmp_h; // RADAR_W = from RADAR bitmap (smaller)
extern int red_spawn_x, red_spawn_y;
extern int blue_spawn_x, blue_spawn_y;
extern int red_flag_x, red_flag_y;
extern int blue_flag_x, blue_flag_y;
extern int red_flag_carrier;
extern int blue_flag_carrier;
extern int x_on_map;
extern int y_on_map;

//genproc.cpp
extern int radar_padding_x;
extern int radar_padding_y;


//collition.cpp
extern double X2(double x, double y, double angle, double radius);
extern double Y2(double x, double y, double angle, double radius);

//global.cpp
extern int save_frames_to_bitmap;
extern int scoreboard_current_y;
extern int fps, fps_count;
extern long int fps_oldtime, fps_newtime;
extern struct timeb t; 
extern struct timeb started; 
extern unsigned long ourtime;

//interface.cpp
extern BITMAP *tmpscreen;
extern BITMAP *bmp_mapfield;
extern BITMAP *bmp_radarfield;
extern BITMAP *bmp_shipfield;  
extern BITMAP *bmp_command;
extern BITMAP *bmp_console;
extern BITMAP *bmp_radar;
extern BITMAP *bmp_radar_display;
extern BITMAP *bmp_scoreboard;
extern ALFONT_FONT *lcdfont;
extern ALFONT_FONT *tccmfont;

extern void init_gui();
extern void blit_gui();
extern void blit_gui_iter();
extern int field_x, field_y; /* field position */
extern int field_w, field_h; /* field size */
extern int header_x, header_y; /* header position */
extern int footer_x, footer_y; /* footer position */
extern int scoreboard_x, scoreboard_y; /* scoreboard position */
extern int scoreboard_w, scoreboard_h; /* scoreboard size */
extern int scoreboard_padding_top;
extern int scoreboard_padding_left;
extern int scoreboard_padding_right;
extern int header_mask1_x, header_mask1_y;
extern int footer_mask1_x, footer_mask1_y;
extern int console_x, console_y; /* console position */
extern int console_w, console_h; /* console size */
extern int command_x, command_y; /* command position */
extern int command_w, command_h; /* command size */
extern int radar_x, radar_y; /* radar position */
extern int radar_w, radar_h; /* radar size */
extern int radar_field_x, radar_field_y; /* radar field position on radar subbitmap */
extern int radar_field_w, radar_field_h; /* radar field size */
extern int radar_shiptype_x, radar_shiptype_y; /* radar: shiptype location */
extern int radar_redflag_x, radar_redflag_y; /* radar: redflag ico location */
extern int radar_blueflag_x, radar_blueflag_y; /* radar: blueflag ico location */
extern int radar_powerbar_x, radar_powerbar_y; /* radar: power bar location */
extern int radar_bulletbar_x, radar_bulletbar_y; /* radar: bullet bar location */
extern int radar_bar_w, radar_bar_h; /* radar: bullet/power bar size */
extern int info_time_x, info_time_y; /* information: time position */
extern int info_lag_x, info_lag_y; /* information: lag position */
extern int info_fps_x, info_fps_y; /* information: fps position */

extern int show_fps;
extern int show_scoreboard;
extern int show_names;
extern int show_radar;
extern char console[CON_LINES][CON_LINE_LENGTH];

//options.cpp
extern int options_activeitem;
extern struct option_ option[3];

//sockets.cpp
extern SOCKET theSocket;
extern double lag[5];
extern char serv_addr[512];
extern int serv_port;
extern char *buffer;
extern int inlen;
