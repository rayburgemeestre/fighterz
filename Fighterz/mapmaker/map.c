#include "header.h"

int x_width;
int y_width;
BITMAP *scrn_field = NULL;
BITMAP *sub_field = NULL;
extern int current_bmp_df_index;

int init_map(int allow_cancel)
{
int ret, cont = 1;
char buffer[512] = "";

    strcpy(buffer, DEFAULT_MAP_FILE);

	ret = file_select_ex("Select map file", buffer, "txt;TXT;", sizeof(buffer), 0, 0);

// Temporary disabled:
    if (ret == 0 && cont == 1 && ! key[KEY_F12])
    {
        if (allow_cancel == 1)
            return -1;
        die("");
    }

    x_width = 0;
    y_width = 0;

    // strcpy(buffer, "D:\\idev\\fighterz\\client\\maps\\lvl1.txt");
    // alert("Opening map file:", buffer, "", "OK", NULL, 0, 0);
    strcpy(mapfile, buffer);

    // ret = popup_dialog(the_dialog, -1);

    {
        FILE *fi;
        int cnt = 0;
        char line[512];

        if (!(fi = fopen(buffer, "r")))
           die("Error opening file");

        while (fgets(line, 512 -1, fi) != NULL)
        {
            char *ptr = line;
            if (*ptr == '1')
            {
                if (strlen(line) <= 2)
                    die("Invalid line input while reading mapfile");
                strcpy(mapdata[cnt], (ptr + 2));
                if ((signed)strlen(ptr+2) > x_width)
                    x_width = strlen(ptr+2);
                y_width++;
                cnt++;
            }
			else if (*ptr == '3')
			{
            int df_id, pos_x, pos_y;
            char buf[512];
            char datafile[128];

				sscanf(line, "3 %d %d %d %s\n", &df_id, &pos_x, &pos_y, datafile);
				sprintf(buf, "%d (%d,%d) %s", df_id, pos_x, pos_y, datafile);
				strcpy(current_bmp_df[current_bmp_df_index++], buf);
			}
        }
        x_width--;

        if (x_width <= 0 || y_width <= 0)
            die("both x_width & y_width must exceed zero");

        {
            char tmp_x[32], tmp_y[32];
            sprintf(tmp_x, "Y %d", x_width);
            sprintf(tmp_y, "X %d", y_width);
            // alert("done", tmp_x, tmp_y, "", NULL, 0, 0);
        }
    }
    destroy_bitmap(scrn_field);
	scrn_field = NULL;
    {
        char tmp[512];
        sprintf(tmp, "params %d, %d", (x_width * BLOCKSIZE)+1,
            (y_width * BLOCKSIZE)+1);
        // alert(tmp, "", "", "", NULL, 1, 1);
    }

	return 0;
}


void drawmap()
{
int cnt, cnt2;

	if (scrn_field)
		destroy_bitmap(scrn_field);
    scrn_field = create_bitmap( (x_width * BLOCKSIZE)+1,
        (y_width * BLOCKSIZE)+1);
    {
    int x = SCREEN_X - RIGHT,
        y = SCREEN_Y - BOTTOM;
        if ( (x_width * BLOCKSIZE) + 1 < x )
            x = (x_width * BLOCKSIZE) + 1;
        if ( (y_width * BLOCKSIZE) + 1 < y )
            y = (y_width * BLOCKSIZE) + 1;
        sub_field = create_sub_bitmap(screen, 0, 0, x, y);
    }

    rectfill(screen, 0, 0, SCREEN_X - RIGHT, SCREEN_Y - BOTTOM, 0);
    clear_to_color(scrn_field, 0);

    // draw the bg's
    for (cnt=0; cnt < 32 && strlen(current_bmp_df[cnt]) >= 1; cnt++)
    {
    int df_id, pos_x, pos_y;
    char buf[512], datafile[512];

    char *s;
    char b[256];

        sscanf(current_bmp_df[cnt], "%d (%d,%d) %s", &df_id, &pos_x, &pos_y, datafile);
        sprintf(buf, "system/%s", datafile);
        s = buf;
        fix_filename_path(b, s, sizeof(b));
        // strcpy(s, b);
        // char *fix_filename_path(char *dest, const char *path, int size);
        // alert(buf, "", "", "Ok", NULL, 0, 0);
        init_datafile(0, buf);
        draw_sprite(scrn_field, (BITMAP *)df[df_id].dat, pos_x, pos_y);
    }

	for (cnt=0; cnt < y_width; cnt++)
	{
		line(scrn_field, 0, (cnt * BLOCKSIZE), (x_width * BLOCKSIZE),
            (cnt * BLOCKSIZE), makecol(128, 128, 128));
	}
    line(scrn_field, 0, (cnt * BLOCKSIZE), (x_width * BLOCKSIZE),
        (cnt * BLOCKSIZE), makecol(128, 128, 128));
    for (cnt=0; cnt < x_width; cnt++)
	{
		line(scrn_field, (cnt * BLOCKSIZE), 0, (cnt * BLOCKSIZE),
            (y_width * BLOCKSIZE), makecol(128, 128, 128));
	}
	line(scrn_field, (cnt * BLOCKSIZE), 0, (cnt * BLOCKSIZE),
        (y_width * BLOCKSIZE), makecol(128, 128, 128));

	for (cnt=0; cnt < x_width; cnt++)
	{
		for (cnt2=0; cnt2 < y_width; cnt2++)
		{
			if (mapdata[cnt2][cnt] == '1')
			{
				rectfill(scrn_field, (cnt * BLOCKSIZE), (cnt2 * BLOCKSIZE),
    				((cnt + 1) * BLOCKSIZE), ((cnt2 + 1) * BLOCKSIZE),
                    makecol(52, 80, 126)
                );
				// draw_sprite(scrn_field, (BITMAP *)dataf[WALL].dat,
                // (cnt * BLOCKSIZE), (cnt2 * BLOCKSIZE) );
			}
			/* Draw red team spawn location */
			if (mapdata[cnt2][cnt] == '2')
				rect(scrn_field, cnt * BLOCKSIZE + 1, cnt2 * BLOCKSIZE + 1,
                    (cnt + 1) * BLOCKSIZE - 1, (cnt2 + 1) * BLOCKSIZE - 1,
                    makecol(255,0,0)
                );
			/* Draw blue team spawn location */
			if (mapdata[cnt2][cnt] == '3') 
				rect(scrn_field, cnt * BLOCKSIZE + 1, cnt2 * BLOCKSIZE + 1,
                    (cnt + 1) * BLOCKSIZE - 1, (cnt2 + 1) * BLOCKSIZE - 1,
                    makecol(0,0,255)
                );
			/* Draw red teams flag location */
			if (mapdata[cnt2][cnt] == '4') 
				rect(scrn_field, cnt * BLOCKSIZE + 1, cnt2 * BLOCKSIZE + 1,
                    (cnt + 1) * BLOCKSIZE - 1, (cnt2 + 1) * BLOCKSIZE - 1,
                    makecol(255,128,0)
                );
			/* Draw blue teams flag location */			
			if (mapdata[cnt2][cnt] == '5') 
				rect(scrn_field, cnt * BLOCKSIZE + 1, cnt2 * BLOCKSIZE + 1,
                    (cnt + 1) * BLOCKSIZE - 1, (cnt2 + 1) * BLOCKSIZE - 1,
                    makecol(0,128,255)
                );
		}
	}
}
int df_index = 0;
char *bmp_images(int index, int *list_size)
{
int i = 0;
int item_cnt = 0;
    while (i < 32)
    {
        if (strlen(bmp_df[i]))
            item_cnt++;
        i++;
    }

   if (index < 0) {
      if (list_size)
	    *list_size = item_cnt;
      return NULL;
   }

   df_index = index;
   return bmp_df[index];
}

int current_df_index = 0;
char *current_bmp_images(int index, int *list_size)
{
int i = 0;
int item_cnt = 0;
    while (i < 32)
    {
        if (strlen(current_bmp_df[i]))
            item_cnt++;
        i++;
    }

   if (index < 0) {
      if (list_size)
	    *list_size = item_cnt;
      return NULL;
   }

   current_df_index = index;
   return current_bmp_df[index];
}
