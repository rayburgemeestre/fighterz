#include "data.h"
#include <allegro.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

#define SCREEN_X                             800
#define SCREEN_Y                             600
#define RIGHT                                150
#define BOTTOM                               100
#define BLOCKSIZE                             20
// #define DATA_FILE                 "fighterz.dat"
#define DEFAULT_MAP_FILE                "maps\\"
#define DEFAULT_DATA_FILE "system\\base_gfx.dat"
#define DLG_MAP_SPECS_OK_BUTTON                6
#define DLG_MAP_CURIMGS_DEL_BUTTON            19
#define DLG_NORMAL_WALL_CHECK                  8
#define DLG_RED_SPAWN_CHECK                    9
#define DLG_BLUE_SPAWN_CHECK                  10
#define DLG_RED_FLAG_CHECK                    11
#define DLG_BLUE_FLAG_CHECK                   12
#define DLG_BG_IMG_CHECK                      13
#define DLG_BG_IMG_LIST                       15
#define DLG_BG_LOAD_DATAFILE                  16
#define DLG_BG_IMG_LIST2                      18
#define DLG_CURRENT_MAP_SAVE                  21
#define DLG_CURRENT_MAP_SAVE_AS               22
#define DLG_CURRENT_MAP_LOAD                  23
#define DLG_CURRENT_MAP_NEW                   24

extern BITMAP *scrn_field;
extern BITMAP *sub_field;
extern DIALOG the_dialog[];
extern DATAFILE *df;

extern char df_path[512];
extern char mapfile[512];
extern char mapdata[512][512];
extern char help_text[9200];

extern int x_width;
extern int y_width;

extern char edit_x_width[64];
extern char edit_y_width[64];

extern char *bmp_images(int index, int *list_size);
extern char *current_bmp_images(int index, int *list_size);
extern char bmp_df[32][128];
extern char current_bmp_df[32][128];

extern int df_index;
