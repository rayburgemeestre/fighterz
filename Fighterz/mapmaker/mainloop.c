#include "header.h"
int current_bmp_df_index = 0;

void mainloop()
{
DIALOG_PLAYER *player;
int x_offset = 0, y_offset = 0;
struct timeb t, starttime;
unsigned long ourtime = 0;
int last_zoom_time = 0;
int redraw_map = 0;
int bmp_df_h, bmp_df_w;

    ftime(&starttime);

    drawmap();

    // set_dialog_color(the_dialog, makecol(0, 0, 0), makecol(52, 80, 126));
    init_dlg_vars();
    player = init_dialog(the_dialog, -1);
    // return shutdown_dialog(player);

    while (! key[KEY_F12] )
    {
    int x, y, diff;
        x = (int)((x_offset + mouse_x) / BLOCKSIZE);
        y = (int)((y_offset + mouse_y) / BLOCKSIZE);

    	ftime(&t);
    	ourtime = (( (t.time - starttime.time) * 1000) +
            ( (t.millitm - starttime.millitm ) / 1));


        // keep track of offsets
        diff = ((x_width * BLOCKSIZE) + 1) - (SCREEN_X - RIGHT);
        if (x_offset > diff)
            x_offset = (diff > 0 ? diff : 0);
        diff = ((y_width * BLOCKSIZE) + 1) - (SCREEN_Y - BOTTOM);
        if (y_offset > diff)
            y_offset = (diff > 0 ? diff : 0);

        // zoom part
        #define ZOOM_DELAY_MILLISECS 2
        #define ZOOM_DELAY_MOVEPIXELS 1

        poll_mouse();
        if ( mouse_x >= 0 && mouse_x <= (SCREEN_X - RIGHT) &&
             mouse_y >= 0 && mouse_y <= (SCREEN_Y - BOTTOM) &&
             x <= x_width && y <= y_width )
        {
            if ( mouse_b & 1 )
            {
                if (the_dialog[DLG_NORMAL_WALL_CHECK].flags & D_SELECTED)
                {
                    mapdata[y][x] = '1';
                    if (mouse_x <= (x_width * BLOCKSIZE) &&
                        mouse_y <= (y_width * BLOCKSIZE))
                    circlefill(screen, mouse_x, mouse_y, 3, makecol(255, 0, 0));
                    redraw_map = 1;
                    continue;
                }
                else if (the_dialog[DLG_RED_SPAWN_CHECK].flags & D_SELECTED)
                {
                int cnt, cnt2;
                    // Remove old red spawn location
                   	for (cnt=0; cnt < x_width; cnt++)
                    {
                		for (cnt2=0; cnt2 < y_width; cnt2++)
                			if (mapdata[cnt2][cnt] == '2')
                                mapdata[cnt2][cnt] = '0';
                    }
                    // The new location
                    mapdata[y][x] = '2';
                    redraw_map = 1;
                    continue;
                }
                else if (the_dialog[DLG_BLUE_SPAWN_CHECK].flags & D_SELECTED)
                {
                int cnt, cnt2;
                    // Remove old blue spawn location
                   	for (cnt=0; cnt < x_width; cnt++)
                    {
                		for (cnt2=0; cnt2 < y_width; cnt2++)
                			if (mapdata[cnt2][cnt] == '3')
                                mapdata[cnt2][cnt] = '0';
                    }
                    // The new location
                    mapdata[y][x] = '3';
                    redraw_map = 1;
                    continue;
                }
                else if (the_dialog[DLG_RED_FLAG_CHECK].flags & D_SELECTED)
                {
                int cnt, cnt2;
                    // Remove old red flag location
                   	for (cnt=0; cnt < x_width; cnt++)
                    {
                		for (cnt2=0; cnt2 < y_width; cnt2++)
                			if (mapdata[cnt2][cnt] == '4')
                                mapdata[cnt2][cnt] = '0';
                    }
                    // The new location
                    mapdata[y][x] = '4';
                    redraw_map = 1;
                    continue;
                }
                else if (the_dialog[DLG_BLUE_FLAG_CHECK].flags & D_SELECTED)
                {
                int cnt, cnt2;
                    // Remove old blue flag location
                   	for (cnt=0; cnt < x_width; cnt++)
                    {
                		for (cnt2=0; cnt2 < y_width; cnt2++)
                			if (mapdata[cnt2][cnt] == '5')
                                mapdata[cnt2][cnt] = '0';
                    }
                    // The new location
                    mapdata[y][x] = '5';
                    redraw_map = 1;
                    continue;
                }
                else if (the_dialog[DLG_BG_IMG_CHECK].flags & D_SELECTED)
                {
                char buf[512], *ptr = df_path, *ptr2 = df_path;
                int i = 0, bool = 0;
                int w = ((BITMAP *)df[atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1])].dat)->w,
                    h = ((BITMAP *)df[atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1])].dat)->h,
                    x = x_offset + (mouse_x - (w / 2)),
                    y = y_offset + (mouse_y - (h / 2));

                    while (*(ptr + i) != '\0')
                    {
                        if (*(ptr + i) == '\\')
                            ptr2 = ptr + i + 1;
                        i++;
                    }
                    i = 0;
                    // draw_sprite(screen, (BITMAP *)dataf[STARS1].dat, (cnt * BLOCKSIZE_2), (cnt2 * BLOCKSIZE_2));
                    // void stretch_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int w, int h);
                    // draw_sprite((BITMAP *)df[atoi(bmp_df[2])].dat, screen, SCREEN_X - RIGHT, SCREEN_Y - 100);
                    // sprintf(buf, "x: %d y: %d", mouse_x, mouse_y);
//                     alert(buf, "", "", "Ok", NULL, 0, 0);

                    if (mouse_x <= (SCREEN_X - RIGHT) &&
                        mouse_y <= (SCREEN_Y - BOTTOM))
                    {
                        sprintf(buf, "%d (%d,%d) %s",
                            atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1]),
                            x,
                            y,
                            ptr2
                        );

                        while (i < 32)
                        {
                            if (!strcmp(current_bmp_df[i], buf))
                                if (bool != 1)
                                    bool = 1;
                            i++;
                        }
                        if (bool == 0)
                        {
                            // alert(buf, ptr2, "", "Ok", NULL, 0, 0);
                            strcpy(current_bmp_df[current_bmp_df_index++], buf);
                            redraw_map = 1;
                            rest(100); // Needed, else read memory failure:
                            // TODO: put project in MSVC++ & remove this delay
                        }
                        continue;
                    }
                }
            }
            if ( mouse_b & 2 )
            {
                mapdata[y][x] = '0';
                if (mouse_x <= (x_width * BLOCKSIZE) &&
                    mouse_y <= (y_width * BLOCKSIZE))
                circlefill(screen, mouse_x, mouse_y, 3, makecol(255, 0, 0));
                redraw_map = 1;
            }
        }

        if ( !(mouse_b & 1) && !(mouse_b & 2) && redraw_map == 1)
        {
            drawmap();
            redraw_map = 0;
        }

        while ( ((ourtime - last_zoom_time) / ZOOM_DELAY_MILLISECS) >= 1  )
        {
            last_zoom_time += ZOOM_DELAY_MILLISECS;

            if (key[KEY_LSHIFT])
            {
                if (key[KEY_LEFT] && x_offset > 0)
                {
                    x_offset -= ZOOM_DELAY_MOVEPIXELS;
                }
                if (key[KEY_RIGHT] && x_offset < ((x_width * BLOCKSIZE) -
                    (SCREEN_X - RIGHT - 1)))
                {
                    x_offset += ZOOM_DELAY_MOVEPIXELS;
                }
                if (key[KEY_UP] && y_offset > 0)
                {
                    y_offset -= ZOOM_DELAY_MOVEPIXELS;
                }
                if (key[KEY_DOWN] && y_offset < ((y_width * BLOCKSIZE) -
                    (SCREEN_Y - BOTTOM - 1)))
                {
                    y_offset += ZOOM_DELAY_MOVEPIXELS;
                }
            }
        }

        {
        char buf[512];
        BITMAP *tussen;
        #define SIZE_X (RIGHT - 2)
        #define SIZE_Y 100

            bmp_df_h = ((BITMAP *)df[ atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1]) ].dat)->h;
            bmp_df_w = ((BITMAP *)df[ atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1]) ].dat)->w;

            tussen = create_bitmap(SIZE_X, SIZE_Y);

            if (bmp_df_w <= SIZE_X && bmp_df_h <= SIZE_Y)
                blit((BITMAP *)df[ atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1]) ].dat, tussen, 0, 0, 0, 0, bmp_df_w, bmp_df_h);
            else
                stretch_sprite(tussen, (BITMAP *)df[ atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1]) ].dat, 0, 0, SIZE_X, SIZE_Y);

            sprintf(buf, "%d", the_dialog[DLG_BG_IMG_LIST].d1);

            blit(tussen, screen, 0, 0, SCREEN_X - RIGHT + 1, 285,
                SIZE_X, SIZE_Y
            );
        }

        blit(scrn_field, sub_field, x_offset, y_offset, 0, 0,
            SCREEN_X - RIGHT, SCREEN_Y - BOTTOM);

        if (the_dialog[DLG_BG_IMG_CHECK].flags & D_SELECTED)
        {
        int w = ((BITMAP *)df[atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1])].dat)->w,
            h = ((BITMAP *)df[atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1])].dat)->h,
            x = mouse_x - (w / 2),
            y = mouse_y - (h / 2),
            x2= x + w,
            y2= y + h;

            // void rect(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
            rect(sub_field, x, y, x2, y2, makecol(255, 0, 0));
            // circlefill(sub_field, mouse_x, mouse_y, 50, makecol(255, 0, 0));
        }
        textprintf(screen, font, SCREEN_X - RIGHT + 3, SCREEN_Y - 31, makecol(255, 255, 255),
            "Pos. in blocks:" );
        textprintf(screen, font, SCREEN_X - RIGHT + 3, SCREEN_Y - 21, 1023,
            "x_width:%3d", x );
        textprintf(screen, font, SCREEN_X - RIGHT + 3, SCREEN_Y - 11, 1023,
            "y_width:%3d", y);

        show_mouse(screen);
        if (! key[KEY_LSHIFT])
        {
        int ret = update_dialog(player);

            if (!ret &&
                player->obj == DLG_MAP_SPECS_OK_BUTTON)
            {
            int new_x = atoi(edit_x_width),
                new_y = atoi(edit_y_width);
            int cnt, cnt2;

                // alert("x", "", "", "", NULL, 0, 0);

               	for (cnt=0; cnt < 512; cnt++) // y
                {
                    for (cnt2=0; cnt2 < 512; cnt2++) // x
                    {
                        if (cnt >= new_x ||
                            cnt2 >= new_y)
                        {
                            mapdata[cnt2][cnt] = '\0';
                        }
                        else if (cnt >= x_width ||
                                 cnt2 >= y_width )
                        {
                            mapdata[cnt2][cnt] = '0';
                        }
                    }
                }
                x_width = new_x;
                y_width = new_y;
                redraw_map = 1;
            }
            else if ( !ret && player->obj == DLG_MAP_CURIMGS_DEL_BUTTON)
            {
            int cnt = 0;
            char buf[512];

                while (strlen(current_bmp_df[cnt]))
                    cnt++;

                cnt--;

                sprintf(buf, "%d, %d", the_dialog[DLG_BG_IMG_LIST2].d1, cnt);
                if (the_dialog[DLG_BG_IMG_LIST2].d1 < cnt)
                {
                char *tussen;
                // we need to get the last and put it over this one
                    tussen = (char *)strdup(current_bmp_df[cnt]);
                    strcpy(current_bmp_df[the_dialog[DLG_BG_IMG_LIST2].d1], tussen);
                    strcpy(current_bmp_df[cnt], "");
                    current_bmp_df_index--;
                    redraw_map = 1;
                } else {
                // we only need to delete the last item
                    strcpy(current_bmp_df[cnt], "");
                    current_bmp_df_index--;
                    redraw_map = 1;
                }
            }
            else if ( !ret && player->obj == DLG_CURRENT_MAP_LOAD)
            {
                if (init_map(1) != -1)
                {
                    init_dlg_vars();
                    redraw_map = 1;
                }
            }
            else if ( !ret && player->obj == DLG_BG_LOAD_DATAFILE)
            {
                if (init_datafile(1, DEFAULT_DATA_FILE) != -1)
                {
                    the_dialog[DLG_BG_IMG_LIST].d1 = 0;
                    init_dlg_vars();
                }
            }
            else if ( !ret && player->obj == DLG_CURRENT_MAP_SAVE_AS)
            {
            char buffer[512] = "maps\\lvl-new.txt";
                if (ret = file_select_ex(
                    "Save file as",
                    buffer,
                    "txt;TXT",
                    sizeof(buffer),
                    0, 0
                ) != 0 )
                {
                FILE *fp;
                int cnt;

                    if (!(fp = fopen(buffer, "w")))
                    {
                        alert("Could not open file", "", "", "Ok", NULL, 0, 0);
                    } else {
                        fputs("; This map is generated using mapmaker v1.0-alpha\n\n", fp);
                       	for (cnt=0; cnt < y_width; cnt++)
                    	{
                            mapdata[cnt][x_width] = '\0'; // could be that
                            // there was a garbage '1' or something (which is not
                            // a problem because the program won't look further
                            // then x_width anyways, but it is a problem here ;)
                            fputs("1 ", fp);
                            fputs(mapdata[cnt], fp);
                            fputs("\n", fp);
                        }
                        fputs("2 20\n\n", fp); // the blocksize line..
                        // the blocksize was supposed to be dynamic, but is no longer
                        // used in newer versions of the game (and this mapmaker)

                        // The background decorations
                        for (cnt = 0; cnt < 32 && strlen(current_bmp_df[cnt]) >= 1; cnt++)
                        {
                        int df_id, pos_x, pos_y;
                        char buf[512];
                        char datafile[128];

                            sscanf(current_bmp_df[cnt], "%d (%d,%d) %s", &df_id, &pos_x, &pos_y, &datafile);
                            sprintf(buf, "3 %d %d %d %s\n", df_id, pos_x, pos_y, datafile);
                            fputs(buf, fp);
                        }

                        fclose(fp);
                    }
                }
            }
        }

        rest(8);
        vsync();
    }
}
