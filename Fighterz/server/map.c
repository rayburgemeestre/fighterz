#include "common.h"

/********************************************
*  MAP FUNCTIONS
****************************************************/

/* reads map from file, stores coordinates in array 
   and sets some important variables */
void loadmap() {

	FILE *fileptr;
	char *ptr;
	int x_count = 0;
	int cnt = 0;

	char buff[500];

	if (!(fileptr = fopen(map3, "r"))) {
		printf_("1: Error opening file %s", map3);
		exit(-1);
	}

	while (fgets(buff, 500, fileptr) != NULL) {
		if ( buff[0] == '1') {
			ptr = strchr(buff, ' ');
			if (ptr == NULL) {	
				printf_("TERMKSDFJ\n");
				exit(-1);
			}
			ptr++;
			if (ptr == NULL) {	
				printf_("TERMKSDFJ\n");
				exit(-1);
			}

			strcpy(buff, strtok(ptr, "\n"));
			
			if ( cnt == 0 ) 
				X_BLOCKS = (int)strlen(buff);
			
			buff[X_BLOCKS] = '\0';

			strcpy(field[cnt], buff);
	
			x_count++;

			cnt++;
			
		}
		if ( (buff[0] == '2') ) {
			strcpy(buff, strtok(buff, "\n"));
			ptr = &buff[2];
			BLOCKSIZE = atoi((const char *) ptr);
			// dbg. BLOCKSIZE = 40;
		}
		Y_BLOCKS = x_count;
	}
	printf_("  Vertical blockcount: %d", Y_BLOCKS);
	printf_("  Horizontal blockcount: %d", X_BLOCKS);
	printf_("  Blocksize in pixels: %d\n\n", BLOCKSIZE);

	field_width = (unsigned long)(X_BLOCKS * BLOCKSIZE);
	field_height = (unsigned long)(Y_BLOCKS * BLOCKSIZE);

#if DEBUG2 == 1
	FIELD_X = 10;
	FIELD_Y = 50;	
	LEFT_2 = (SCREEN_X - 150) - LEFT;
	
	/* create the bitmaps */
	destroy_bitmap(fieldbuff);	
	destroy_bitmap(shipbuff);
	tmpscreen = create_bitmap(SCREEN_X, SCREEN_Y);
	shipbuff = create_bitmap(field_width + 1, field_height + 1);
#endif
}


#if DEBUG2 == 1
void set_map_coords()
{
		if ((servertime - map_scroll) >= 10)
		{
			map_scroll = servertime;
			
			if (key[KEY_LEFT])
				MAP_X -= 10;
			if (key[KEY_RIGHT])
				MAP_X += 10;
			if (key[KEY_UP])
				MAP_Y -= 10;
			if (key[KEY_DOWN])
				MAP_Y += 10;
		}

		if (MAP_X < 0) MAP_X = 0;
		if ((unsigned)MAP_X > ((field_width + 1) - MAP_W)) 
			MAP_X = (field_width + 1) - MAP_W;

		if (MAP_Y < 0) MAP_Y = 0;
		if ((unsigned)MAP_Y > ((field_height + 1) - MAP_H)) 
			MAP_Y = (field_height + 1) - MAP_H;

}

void drawmap() {
	int cnt, cnt2;
	int x = 0, length, length2;

	/* Colours */
	int blauw = makecol(0,128,255);
	int grijs = makecol(128,128,128);

	// destroy_bitmap(fieldbuff);
	fieldbuff = create_bitmap(field_width + 1, field_height + 1);
	clear_to_color(fieldbuff, 0);
	clear_to_color(tmpscreen, 0);

	/*	
		fieldbuff = create_bitmap(SCREEN_X, SCREEN_Y);
		clear_to_color (fieldbuff, 0);
	*/
	
	length = X_BLOCKS * BLOCKSIZE;
	length2  = Y_BLOCKS * BLOCKSIZE;
	
	/* Draw lines */
	/*   (horizontal:) */
	for (cnt=0; cnt<Y_BLOCKS; cnt++)
	{
		line(fieldbuff, 0, (cnt * BLOCKSIZE), length, (cnt * BLOCKSIZE), grijs);
	}
	line(fieldbuff, 0, (cnt * BLOCKSIZE), length, (cnt * BLOCKSIZE), grijs);

	/*   (vertical:) */
	for (cnt=0; cnt<X_BLOCKS; cnt++)
	{
		line(fieldbuff, (cnt * BLOCKSIZE), 0, (cnt * BLOCKSIZE), length2, grijs);
	}
	line(fieldbuff, (cnt * BLOCKSIZE), 0, (cnt * BLOCKSIZE), length2, grijs);

	/* Draw walls */
	for (cnt=0; cnt<X_BLOCKS; cnt++) 
	{
		for (cnt2=0; cnt2<Y_BLOCKS; cnt2++)
		{
			if (field[cnt2][cnt] == '1') 
				rectfill(fieldbuff, (cnt * BLOCKSIZE), (cnt2 * BLOCKSIZE), 
				((cnt + 1) * BLOCKSIZE), ((cnt2 + 1) * BLOCKSIZE), blauw);
			/* Draw red team spawn location */
			if (field[cnt2][cnt] == '2') 
			{
				rectfill(fieldbuff, cnt * BLOCKSIZE, cnt2 * BLOCKSIZE, (cnt + 1) * BLOCKSIZE, (cnt2 + 1) * BLOCKSIZE, makecol(255,0,0));
				RED.x = cnt * BLOCKSIZE;
				RED.y = cnt2 * BLOCKSIZE;
			}
			/* Draw blue team spawn location */
			if (field[cnt2][cnt] == '3') 
			{
				rectfill(fieldbuff, cnt * BLOCKSIZE, cnt2 * BLOCKSIZE, (cnt + 1) * BLOCKSIZE, (cnt2 + 1) * BLOCKSIZE, makecol(0,0,255));
				BLUE.x = cnt * BLOCKSIZE;
				BLUE.y = cnt2 * BLOCKSIZE;			
			}
			/* Draw red teams flag location */
			if (field[cnt2][cnt] == '4') 
			{
				rectfill(fieldbuff, cnt * BLOCKSIZE, cnt2 * BLOCKSIZE, (cnt + 1) * BLOCKSIZE, (cnt2 + 1) * BLOCKSIZE, makecol(255,128,0));
				REDFLAG.x = cnt * BLOCKSIZE;
				REDFLAG.y = cnt2 * BLOCKSIZE;
			}
			/* Draw blue teams flag location */			
			if (field[cnt2][cnt] == '5') 
			{
				rectfill(fieldbuff, cnt * BLOCKSIZE, cnt2 * BLOCKSIZE, (cnt + 1) * BLOCKSIZE, (cnt2 + 1) * BLOCKSIZE, makecol(0,128,255));
				BLUEFLAG.x = cnt * BLOCKSIZE;
				BLUEFLAG.y = cnt2 * BLOCKSIZE;			
			}
		}
	}
}

#endif

void fillmap()
{
	
	int cnt, cnt2;

	for (cnt=0; cnt<Y_BLOCKS; cnt++) 
	{
		for (cnt2=0; cnt2<(X_BLOCKS); cnt2++) 
		{
			if (field[cnt][cnt2] == '1')
				map2[cnt][cnt2] = '#';
			else
				map2[cnt][cnt2] = '1';
		}
		for (cnt2=0; cnt2<X_BLOCKS; cnt2++) 
		{
			if (field[cnt][cnt2] == '1')
				map2[cnt][cnt2 + X_BLOCKS] = '#';
			else
				map2[cnt][cnt2 + X_BLOCKS] = '1';
		}
		// fprintf(kuub, "%s\n", map2[cnt]);
	}
	cnt2 = cnt; // - 1; this was a mistake ;) 
	for (cnt=0; cnt<Y_BLOCKS; cnt++) 
	{
		// fprintf(kuub, "%s\n", map2[cnt]);
		strcpy(map2[cnt2], map2[cnt]);
		cnt2++;		
	}	
}

/* CONSOLE DRAWMAP WITHOUT ANY GRAPHIC ROUTINES 
	BECAUSE WE NEED STUFF THIS FUNCTION DOES */
void drawmap_features() {
	int cnt, cnt2;
	int length, length2;

	length = X_BLOCKS * BLOCKSIZE;
	length2  = Y_BLOCKS * BLOCKSIZE;
	
	for (cnt=0; cnt<X_BLOCKS; cnt++) 
	{
		for (cnt2=0; cnt2<Y_BLOCKS; cnt2++)
		{
			/* Got red team spawn location */
			if (field[cnt2][cnt] == '2') 
			{
				RED.x = cnt * BLOCKSIZE;
				RED.y = cnt2 * BLOCKSIZE;
			}
			/* Got blue team spawn location */
			if (field[cnt2][cnt] == '3') 
			{
				BLUE.x = cnt * BLOCKSIZE;
				BLUE.y = cnt2 * BLOCKSIZE;			
			}
			/* got red teams flag location */
			if (field[cnt2][cnt] == '4') 
			{
				REDFLAG.x = cnt * BLOCKSIZE;
				REDFLAG.y = cnt2 * BLOCKSIZE;
			}
			/* got blue teams flag location */			
			if (field[cnt2][cnt] == '5') 
			{
				BLUEFLAG.x = cnt * BLOCKSIZE;
				BLUEFLAG.y = cnt2 * BLOCKSIZE;			
			}
		}
	}
}