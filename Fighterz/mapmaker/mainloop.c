#include "header.h"
int current_bmp_df_index = 0;

void mainloop()
{
DIALOG_PLAYER *player;
int x_offset = 0, y_offset = 0, ret;
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
             mouse_y >= 0 && mouse_y <= (SCREEN_Y - BOTTOM) )
        {
            if ( mouse_b & 1 )
            {
                if (the_dialog[DLG_NORMAL_WALL_CHECK].flags & D_SELECTED)
                {
                char buf[512];

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
                char buf[512];
                int i = 0, bool = 0;
                    // draw_sprite(screen, (BITMAP *)dataf[STARS1].dat, (cnt * BLOCKSIZE_2), (cnt2 * BLOCKSIZE_2));
                    // void stretch_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int w, int h);
                    // draw_sprite((BITMAP *)df[atoi(bmp_df[2])].dat, screen, SCREEN_X - RIGHT, SCREEN_Y - 100);
                    // sprintf(buf, "x: %d y: %d", mouse_x, mouse_y);
//                     alert(buf, "", "", "Ok", NULL, 0, 0);

                    sprintf(buf, "%d [%dx%d]", atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1]),
                        (x_offset + mouse_x), (y_offset + mouse_y));

                    while (i < 32)
                    {
                        if (!strcmp(current_bmp_df[i], buf))
                            if (bool != 1)
                                bool = 1;
                        i++;
                    }
                    if (bool == 0)
                    {
                        strcpy(current_bmp_df[current_bmp_df_index++], buf);
                        redraw_map = 1;
                        rest(100); // Needed, else read memory failure:
                        // TODO: put project in MSVC++ & remove this delay
                    }
                    continue;
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

            stretch_sprite(tussen, (BITMAP *)df[ atoi(bmp_df[the_dialog[DLG_BG_IMG_LIST].d1]) ].dat, 0, 0, SIZE_X, SIZE_Y);
            sprintf(buf, "%d", the_dialog[DLG_BG_IMG_LIST].d1);

            blit(tussen, screen, 0, 0, SCREEN_X - RIGHT + 1, 275,
                SIZE_X, SIZE_Y
            );
        }
        blit(scrn_field, screen, x_offset, y_offset, 0, 0,
            SCREEN_X - RIGHT, SCREEN_Y - BOTTOM);

        textprintf(screen, font, SCREEN_X - RIGHT + 3, SCREEN_Y - 31, makecol(255, 255, 255),
            "Pos. in blocks:" );
        textprintf(screen, font, SCREEN_X - RIGHT + 3, SCREEN_Y - 21, makecol(197, 230, 254),
            "x_width:%3d", x );
        textprintf(screen, font, SCREEN_X - RIGHT + 3, SCREEN_Y - 11, makecol(197, 230, 254),
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
        }

        rest(8);
        vsync();
    }
}
