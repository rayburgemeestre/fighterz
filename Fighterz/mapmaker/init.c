#include "header.h"

DATAFILE *df;
char bmp_df[32][12];
char current_bmp_df[32][12];

void init()
{
    // Initialize Allegro.
    allegro_init();

    set_color_depth(16);

    // Set the resolution to 640 by 480 with SAFE autodetection.
    // set_gfx_mode(GFX_SAFE, SCREEN_X, SCREEN_Y, 0, 0);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_X, SCREEN_Y, 0, 0);

    // Installing the keyboard handler.
    install_keyboard();
    install_mouse();

    gui_bg_color = makecol(52, 80, 126);
    gui_fg_color = makecol(255, 255, 255);

    if (0)
    {
        char buf[512];
        sprintf(buf, "%d, %d, %d, %d, %d, %d, %d, %d",
            makecol(32, 32, 32),
            makecol(0, 255, 125),
            makecol(255, 0, 0),
            makecol(0, 0, 255),

            makecol(255, 125, 0),
            makecol(0, 125, 255),
            makecol(255, 255, 255),
            makecol(81, 81, 81)
        );
        alert(buf, "", "", "", NULL, 0, 0);
    }

    df = load_datafile(DATA_FILE);

    {
    int index = 0, i = 0;
    DATAFILE *ptr = df;

        while (ptr->type != DAT_END)
        {
            if (ptr->type == DAT_BITMAP)
            {
            char buf[12];
                sprintf(buf, "%d", i);
                strcpy(bmp_df[index++], buf);
            }
            ptr++;
            i++;
        }
    }
}
