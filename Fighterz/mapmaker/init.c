#include "header.h"

DATAFILE *df;
char bmp_df[32][128];
char current_bmp_df[32][128];
char df_path[512];

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
            makecol(0, 33, 72),
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
}

int init_datafile(int allow_cancel, char *arg_df_path)
{
int ret;

    strcpy(df_path, arg_df_path);

if (allow_cancel != 0)
{
    if (ret = file_select_ex(
        "Select datafile (*.dat)",
        df_path,
        "dat;DAT;",
        sizeof(df_path),
        0, 0
    ) == 0 && ! key[KEY_F12])
    {
        return -1;
    }
}

    if (!(df = load_datafile(df_path)))
    {
    char mbuf[512];
	char buf[512];
        sprintf(mbuf, "Could not load datafile: =%s=", df_path);
		/* try in c:\fighterz... */
		sprintf(buf, "c:\\fighterz\\%s", df_path);
		strcpy(df_path, buf);
		if (df = load_datafile(df_path)) {
			char mbuf2[512];
			sprintf(mbuf2, "using %s instead.", df_path);
			alert("Warning:", mbuf, mbuf2, "OK", NULL, 0, 0);
		} else {
			die(mbuf);
		}
	}

    {
    int index = 0, i = 0;
    DATAFILE *ptr = df;

        while (ptr->type != DAT_END)
        {
            if (ptr->type == DAT_BITMAP)
            {
            char buf[128];
                sprintf(buf, "%d", i);
                strcpy(bmp_df[index++], buf);
            }
            ptr++;
            i++;
        }
    }

	return 0;
}
