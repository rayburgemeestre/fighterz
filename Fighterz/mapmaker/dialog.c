#include "header.h"

#define INGAME_DLG_FG 2031
#define INGAME_DLG_BG 8452
#define INGAME_DLG_FG_LBL 1063
#define INGAME_DLG_BG_LBL 0

DIALOG the_dialog[] =
{
    /* (dialog proc)     (x)   (y)   (w)   (h)
                         (fg)  (bg)  (key) (flags)
                         (d1)  (d2)  (dp) */
// 0
    { d_clear_proc,      0,    0,    0,    0,
                         255,  0,    0,    0,
                         0,    0,    NULL },
// 1
    { d_text_proc,       SCREEN_X - RIGHT + 1, 5, RIGHT - 2, 48,
                         65535,  INGAME_DLG_BG_LBL,   0,    0,
                         512,  0,    "Map specification" },
// 2
    // x_width
    { d_text_proc,       SCREEN_X - RIGHT + 1, 15, RIGHT - 2, 48,
                         INGAME_DLG_FG_LBL,  0,   0,    0,
                         512,  0,    "x_width:" },
// 3
    { d_edit_proc,       SCREEN_X - RIGHT + 1, 25, RIGHT - 2, 48,
                         INGAME_DLG_FG,  INGAME_DLG_BG,   0,    0,
                         64,  0,    edit_x_width },
// 4
    // y_width
    { d_text_proc,       SCREEN_X - RIGHT + 1, 35, RIGHT - 2, 48,
                         INGAME_DLG_FG_LBL,  INGAME_DLG_BG_LBL,   0,    0,
                         512,  0,    "y_width:" },
// 5
    { d_edit_proc,       SCREEN_X - RIGHT + 1, 45, RIGHT - 2, 50,
                         INGAME_DLG_FG,  INGAME_DLG_BG,   0,    0,
                         64,  0,    edit_y_width },
// 6
    { d_button_proc,     SCREEN_X - RIGHT + 1, 60, 30, 12,
                         INGAME_DLG_FG_LBL,  INGAME_DLG_BG_LBL,   0,    D_EXIT,
                         512,  0,    "Ok" },
// 7
    // Adding blocks
    { d_text_proc,       SCREEN_X - RIGHT + 1, 85, RIGHT - 2, 48,
                         65535,  INGAME_DLG_BG_LBL,   0,    0,
                         512,  0,    "Add blocks:" },
// 8
    { d_radio_proc,      SCREEN_X - RIGHT + 1, 95, RIGHT - 2, 9,
                         INGAME_DLG_FG,  0,   0,    0,
                         512,  0,    "Normal Wall" },
// 9
    { d_radio_proc,      SCREEN_X - RIGHT + 1, 105, RIGHT - 2, 9,
                         63488,  0,   0,    0,
                         512,  0,    "Spawn Team red" },
// 10
    { d_radio_proc,      SCREEN_X - RIGHT + 1, 115, RIGHT - 2, 9,
                         31,  0,   0,    0,
                         512,  0,    "Spawn Team blue" },
// 11
    { d_radio_proc,      SCREEN_X - RIGHT + 1, 125, RIGHT - 2, 9,
                         64480,  0,   0,    0,
                         512,  0,    "Flag Team red" },
// 12
    { d_radio_proc,      SCREEN_X - RIGHT + 1, 135, RIGHT - 2, 9,
                         1023,  0,   0,    0,
                         512,  0,    "Flag Team blue" },
// 13
    { d_radio_proc,      SCREEN_X - RIGHT + 1, 155, RIGHT - 2, 9,
                         255,  0,   0,    0,
                         512,  0,    "Background image" },
// 14
    // Map background
    { d_text_proc,       SCREEN_X - RIGHT + 1, 175, RIGHT - 2, 48,
                         65535,  INGAME_DLG_BG_LBL,   0,    0,
                         512,  0,    "Select image" },
// 15
    { d_list_proc,       SCREEN_X - RIGHT + 1, 185, RIGHT - 2, 80,
                         21130,  0,   0,    0,
                         512,  0,    bmp_images },
// 16
    // Map background
    { d_text_proc,       SCREEN_X - RIGHT + 1, 385, RIGHT - 2, 48,
                         65535,  INGAME_DLG_BG_LBL,   0,    0,
                         512,  0,    "Current images" },
// 17
    { d_list_proc,       SCREEN_X - RIGHT + 1, 395, RIGHT - 2, 80,
                         21130,  0,   0,    0,
                         512,  0,    current_bmp_images },
// 18
    { d_button_proc,     SCREEN_X - RIGHT + 1, 480, 35, 12,
                         INGAME_DLG_FG_LBL,  INGAME_DLG_BG_LBL,   0,    D_EXIT,
                         512,  0,    "Del" },

    { d_textbox_proc,    5, SCREEN_Y - BOTTOM + 5, SCREEN_X - 5 - RIGHT , BOTTOM - 2 - 5,
                         21130,  0,   0,    0,
                         512,  0,    help_text },

    { NULL,              0,    0,    0,    0,
                         0,    0,    0,    0,
                         0,    0,    NULL }
};

void init_dlg_vars()
{
char buf[512];

    sprintf(buf, "%d", x_width);
    strcpy(edit_x_width, buf);
    sprintf(buf, "%d", y_width);
    strcpy(edit_y_width, buf);
}
