/********************************************
*  VARIABLES
****************************************************/

//DEPRECATED: extern double SPEED;
extern struct timeb t; 
extern struct timeb started; 
#if WINDOWS != 1
extern struct timeval t2;
#else
extern struct timeb t2; 
#endif
extern TIME servertime;
extern unsigned long int lps_time;
extern unsigned long int lps_time2;
extern unsigned int lps;
extern unsigned int lps_counter;
extern int player_nr;
extern unsigned int LAG_CHECKS;
extern TIME LAG_DELAY;
extern int LAG_DELAY_SECS;
extern unsigned int RESPAWN_DELAY;
extern int MOVE_STEPW;
extern int BLOCKSIZE;
extern unsigned long field_width, field_height;
extern int Y_BLOCKS;
extern int X_BLOCKS;
extern char field[500][500];
extern char map2[1000][1000];
extern char map3[64];
extern int map[1000][1000];
#if DEBUG == 1
extern FILE *outp;
#endif
extern unsigned int BULLET_MAX;
extern TIME BULLET_TTL;
extern int MOVE_BSTEPW;
extern double B_SPEED;
#if DEBUG2 == 1
extern FILE *outp2;
extern BITMAP *tmpscreen;
extern char console[MAX_C_LINES][MAX_C_LLENGTH];
extern int TXTPTR;
extern int TXTPTR2;
extern int LEFT_2;
extern int FULLSCREEN;
extern int SCREEN_Y;
extern int SCREEN_X;
extern int LEFT;
extern int TOP;
extern int show_fps;
extern int show_ulist;
extern int fps; 
extern int fps_count;
extern long int fps_oldtime, fps_newtime;
extern BITMAP *fieldbuff; 
extern BITMAP *shipbuff;
extern int MAP_X;
extern int MAP_Y;
extern int MAP_W;
extern int MAP_H;
extern int VIEW_X;
extern int VIEW_Y;
extern unsigned int FIELD_X;
extern unsigned int FIELD_Y;
extern unsigned int map_scroll;
#endif
extern unsigned int MAX_HITS; 
extern unsigned int MAX_CHARS_IN_SENTENCE;
extern struct flagpositions flagpos;
extern struct location RED;
extern struct location BLUE;
extern struct location REDFLAG;
extern struct location BLUEFLAG;
extern struct location red_flag_default;
extern struct location blue_flag_default;
extern int extra_x;
extern int extra_y;
extern ID bot_id;
extern int path_calc_dbg_on;
extern FILE *path_calc_dbg;
extern LINK head;
extern int dist[1000][1000];
extern int parent[1000][1000][2];
extern int startr, startc;
extern int endr, endc;
extern int rows_read, cols_read;
extern int pops, pushes;
extern int debug_level;
extern int temprrr;
extern int element;

extern struct bg_imgs bg_imgs_data[32];

//server.c
extern int id_has_redflag;
extern int id_has_blueflag;
extern unsigned int red_team_score;
extern unsigned int blue_team_score;

//player.c
extern int red_team_count;
extern int blue_team_count;