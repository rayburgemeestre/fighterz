/********************************************
*  VARIABLES
****************************************************/

extern MIDI *bg_music;
extern MIDI *intro_music;
extern BITMAP *tmpscreen;
extern BITMAP *fieldbuff;
extern BITMAP *radarbuff;
extern BITMAP *shipbuff;  
extern BITMAP *talkbuff;
extern BITMAP *ulistbuff;
extern BITMAP *CONSOLE;
extern BITMAP *RADAR;
extern int SCREEN_Y;
extern int SCREEN_X;
extern int FULLSCREEN;
extern int HIGH_GRAPHICS;
extern int BOUNCING_BULLETS;
extern int MOVE_STEPW;
//extern double SPEED; 
extern double B_SPEED;
extern double SPEED_2; 
extern double B_SPEED_2;
extern int TXTPTR;
extern int TXTPTR2;
extern char map2[256];
extern char field[500][500];
extern int Y_BLOCKS;
extern int X_BLOCKS;
extern int BLOCKSIZE;
extern unsigned long field_width, field_height;
extern int BLOCKSIZE_2; 
extern unsigned long field_height_2;
extern unsigned long field_width_2;
extern int LEFT;
extern int TOP;
extern int fps; 
extern int fps_count;
extern long int fps_oldtime, fps_newtime;
extern int show_fps;
extern int show_names;
extern int LEFT_2;
extern int show_ulist;
extern struct timeb t; 
extern struct timeb started; 
extern unsigned long ourtime;
extern unsigned int our_id;
extern SOCKET theSocket;
extern char receivebuff[512];
#define MAX_C_LINES 12
#define MAX_C_LLENGTH 80
extern char console[MAX_C_LINES][MAX_C_LLENGTH];
extern int STARTED;
extern double lag[5];
#if DEBUG == 1
extern FILE *boom;
extern FILE *dbug;
#endif
extern int showscr; 
extern DATAFILE *dataf;
extern DATAFILE *df_snd;
extern unsigned long bullet_time;
extern int BULLET_COUNT;
extern int BULLET_MAX;
extern int BULLET_TTL;
extern int BULLET_RE;
extern int MOVE_BSTEPW;
extern int USE_SOUND;
extern double MAP_X;
extern double MAP_Y; 
extern double MAP_W;
extern double MAP_H;
extern unsigned int CONSOLE_X;
extern unsigned int CONSOLE_Y;
extern unsigned int CONSOLE_W; 
extern unsigned int CONSOLE_H; 
extern unsigned int RADAR_SIZE;
extern double RADAR_SCALE;
extern unsigned int RADAR_W;
extern unsigned int RADAR_H;
extern unsigned int RADAR_X;
extern unsigned int RADAR_Y;
extern unsigned int RADAR_SHOW;
extern unsigned int INDICATOR_WIDTH;
extern unsigned int INDICATOR_DISTANCE_BETWEEN;
extern unsigned int CSCREEN_X;
extern unsigned int CSCREEN_Y;
extern unsigned int CSCREEN_H;
extern unsigned int FIELD_X;
extern unsigned int FIELD_Y;
extern double ZOOM;
extern unsigned long zoom_time;
extern unsigned int MAX_HITS;
extern unsigned int guessed_power;
extern int talk;
extern unsigned int MAX_CHARS_IN_SENTENCE;
extern char msg[80];
extern int active_o_item;
extern int field_mode;
extern char nickname[64];
extern char taddr[512];
extern int tport;
extern int grid;
extern int direction_is_up;
extern LINK head;
extern LINK new_node;
extern LINK current;
extern LINK our_node;
extern LINK2 e_head;
extern LINK2 e_new_node;
extern LINK2 e_current;
extern struct option_ option[3];

extern char *buffer;
extern int inlen;
extern int can_spawn;
extern int requested_spawn;
extern int terminate_request;

extern char large_text_msg[64];
extern unsigned long large_text_time;
extern unsigned int large_text_display_time;

extern struct bg_imgs bg_imgs_data[32];
extern int bg_imgs_data_index;
extern int midi_track;
extern int bg_music_on;

//map.cpp
extern double scale;
extern int radar_w, radar_h; // RADAR_W = from RADAR bitmap (smaller)

//genproc.cpp
extern int radar_padding_x;
extern int radar_padding_y;