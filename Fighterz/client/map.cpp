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
	int cnt = 0, cnt2 = 0;

	char buff[500];

	if (!(fileptr = fopen(map2, "r"))) {
		char tyfus[512];

		sprintf(tyfus, "..\\%s", map2);
		fileptr = fopen(tyfus, "r");
		if (!fileptr)
		{
			printff_direct("Error opening file %s\\%s", getcwd(tyfus, 512), map2);
			terminate();
		}
		strcpy(map2, tyfus);
	}

	while (fgets(buff, 800, fileptr) != NULL) {
		if ( buff[0] == '1') {
			ptr = strchr(buff, ' ');
			if (ptr == NULL) {	
				printff_direct("TERMKSDFJ\n");
				terminate();
			}
			ptr++;
			if (ptr == NULL) {	
				printff_direct("TERMKSDFJ\n");
				terminate();
			}

			strcpy(buff, strtok(ptr, "\n"));
			
			if ( cnt == 0 ) 
				X_BLOCKS = strlen(buff);
			
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
	printff_direct("  Vertical blockcount: %d", Y_BLOCKS);
	printff_direct("  Horizontal blockcount: %d", X_BLOCKS);
	printff_direct("  Blocksize in pixels: %d", BLOCKSIZE);
}

/* procedure to draw map on screen */
void drawmap() {
	int cnt, cnt2;
	int x = 0, length, length2;

	/* Colours */
	int blauw = makecol(0,128,255);
	int grijs = makecol(128,128,128);

	destroy_bitmap(fieldbuff);
	fieldbuff = create_bitmap(field_width_2 + 1, field_height_2 + 1);
	//fieldbuff = create_sub_bitmap(tmpscreen, FIELD_X, FIELD_Y, MAP_W, MAP_H);
	clear_to_color(fieldbuff, 0);
	clear_to_color(tmpscreen, 0);

	/*	
		fieldbuff = create_bitmap(SCREEN_X, SCREEN_Y);
		clear_to_color (fieldbuff, 0);
	*/
	
	length = X_BLOCKS * BLOCKSIZE_2;
	length2  = Y_BLOCKS * BLOCKSIZE_2;

	/* stars */
	for (cnt=0; cnt<X_BLOCKS; cnt++) 
	{
		for (cnt2=0; cnt2<Y_BLOCKS; cnt2++)
		{
			{
				int ret;
				if (1 == 1)
				{
					/* temporary stars :) */
					ret = 1+(int) (15*rand()/(RAND_MAX+1.0));
					
					if (ret == 1)
						draw_sprite(fieldbuff, (BITMAP *)dataf[STARS1].dat, (cnt * BLOCKSIZE_2), (cnt2 * BLOCKSIZE_2));
					else if (ret == 2)
						draw_sprite(fieldbuff, (BITMAP *)dataf[STARS2].dat, (cnt * BLOCKSIZE_2), (cnt2 * BLOCKSIZE_2));
					else if (ret == 2)
						draw_sprite(fieldbuff, (BITMAP *)dataf[STARS3].dat, (cnt * BLOCKSIZE_2), (cnt2 * BLOCKSIZE_2));
					else if (ret == 4)
						draw_sprite(fieldbuff, (BITMAP *)dataf[STARS4].dat, (cnt * BLOCKSIZE_2), (cnt2 * BLOCKSIZE_2));
				}
			}
		}
	}
	{
		/* random stuff */
		unsigned long rw, rh;
		int i;
		/* draw 2 nevels */
		for (i=0; i<2; i++)
		{
			rw = 1+(int) (field_width*rand()/(RAND_MAX+1.0));
			rh = 1+(int) (field_height*rand()/(RAND_MAX+1.0));
			draw_sprite(fieldbuff, (BITMAP *)dataf[NEVEL].dat, rw, rh);
		}
		
		/* draw 2 nevels */
		for (i=0; i<2; i++)
		{
			rw = 1+(int) (field_width*rand()/(RAND_MAX+1.0));
			rh = 1+(int) (field_height*rand()/(RAND_MAX+1.0));
			draw_sprite(fieldbuff, (BITMAP *)dataf[NEVEL2].dat, rw, rh);
		}

		/* draw 2 nevels */
		for (i=0; i<2; i++)
		{
			rw = 1+(int) (field_width*rand()/(RAND_MAX+1.0));
			rh = 1+(int) (field_height*rand()/(RAND_MAX+1.0));
			draw_sprite(fieldbuff, (BITMAP *)dataf[NEVEL3].dat, rw, rh);
		}

	}
	
	if (grid)
	{
		/* Draw lines */
		/*   (horizontal:) */
		for (cnt=0; cnt<Y_BLOCKS; cnt++)
		{
			line(fieldbuff, 0, (cnt * BLOCKSIZE_2), length, (cnt * BLOCKSIZE_2), grijs);
		}
		line(fieldbuff, 0, (cnt * BLOCKSIZE_2), length, (cnt * BLOCKSIZE_2), grijs);

		/*   (vertical:) */
		for (cnt=0; cnt<X_BLOCKS; cnt++)
		{
			line(fieldbuff, (cnt * BLOCKSIZE_2), 0, (cnt * BLOCKSIZE_2), length2, grijs);
		}
		line(fieldbuff, (cnt * BLOCKSIZE_2), 0, (cnt * BLOCKSIZE_2), length2, grijs);
	}

	/* Draw walls */
	for (cnt=0; cnt<X_BLOCKS; cnt++) 
	{
		for (cnt2=0; cnt2<Y_BLOCKS; cnt2++)
		{
			if (field[cnt2][cnt] == '1')
			{
				rectfill(fieldbuff, (cnt * BLOCKSIZE_2), (cnt2 * BLOCKSIZE_2), 
				((cnt + 1) * BLOCKSIZE_2), ((cnt2 + 1) * BLOCKSIZE_2), makecol(128, 128, 128) ); // was blauw
				/* test :P */
				draw_sprite(fieldbuff, (BITMAP *)dataf[WALL].dat, (cnt * BLOCKSIZE_2), (cnt2 * BLOCKSIZE_2) );
			}
			/* Draw red team spawn location */
			if (field[cnt2][cnt] == '2') 
				rect(fieldbuff, cnt * BLOCKSIZE + 1, cnt2 * BLOCKSIZE + 1, (cnt + 1) * BLOCKSIZE - 1, (cnt2 + 1) * BLOCKSIZE - 1, makecol(255,0,0));
			/* Draw blue team spawn location */
			if (field[cnt2][cnt] == '3') 
				rect(fieldbuff, cnt * BLOCKSIZE + 1, cnt2 * BLOCKSIZE + 1, (cnt + 1) * BLOCKSIZE - 1, (cnt2 + 1) * BLOCKSIZE - 1, makecol(0,0,255));
			/* Draw red teams flag location */
			if (field[cnt2][cnt] == '4') 
				rect(fieldbuff, cnt * BLOCKSIZE + 1, cnt2 * BLOCKSIZE + 1, (cnt + 1) * BLOCKSIZE - 1, (cnt2 + 1) * BLOCKSIZE - 1, makecol(255,128,0));
			/* Draw blue teams flag location */			
			if (field[cnt2][cnt] == '5') 
				rect(fieldbuff, cnt * BLOCKSIZE + 1, cnt2 * BLOCKSIZE + 1, (cnt + 1) * BLOCKSIZE - 1, (cnt2 + 1) * BLOCKSIZE - 1, makecol(0,128,255));
		}
	}
}

void drawradar()
{
	if (RADAR_SHOW != 1)
		return;
	int cnt, cnt2, color;
	int LEFT = (INDICATOR_WIDTH * 2) + (INDICATOR_DISTANCE_BETWEEN * 2);
	int grijs = makecol(0,255,128);
	int c1 = makecol(255, 0, 0);
	int c2 = makecol(255, 128, 0);
	int c3 = makecol(255, 255, 0);
	LINK x;

	double _power, _bullets, _guessed_power;

	rectfill(RADAR, (INDICATOR_WIDTH * 2) + (INDICATOR_DISTANCE_BETWEEN * 2) + 1, 
		1, RADAR_W - 2, RADAR_H - 2, makecol(0, 0, 0));

	for (cnt=0; cnt<X_BLOCKS; cnt++) 
		for (cnt2=0; cnt2<Y_BLOCKS; cnt2++)
			if (field[cnt2][cnt] == '1') 
				rectfill(RADAR, LEFT + (cnt * BLOCKSIZE) / RADAR_SCALE, 
					(cnt2 * BLOCKSIZE) / RADAR_SCALE, 
					LEFT + ((cnt + 1) * BLOCKSIZE) / RADAR_SCALE, 
					((cnt2 + 1) * BLOCKSIZE) / RADAR_SCALE, grijs);
	


	rect(RADAR, (INDICATOR_WIDTH * 2) + (INDICATOR_DISTANCE_BETWEEN * 2), 
		0, RADAR_W - 1, RADAR_H - 1, makecol(69, 69, 69));
	

	
	_power = (our_node->power * 100) / MAX_HITS;
	_guessed_power = (guessed_power * 100) / MAX_HITS;
	_bullets = ((BULLET_MAX - BULLET_COUNT) * 100) / BULLET_MAX;

	/* Power indicator */
	rectfill(RADAR, 1, 1,
		INDICATOR_WIDTH - 1,
		((RADAR_H - 2) * (100.0 - _power)) / 100.0,
		c1
	);
	rectfill(RADAR, 1, 
		((RADAR_H - 2) * (100.0 - _power)) / 100.0,
		INDICATOR_WIDTH - 1,
		RADAR_H - 2,
		c3
	);
	rectfill(RADAR, 1, 
		((RADAR_H - 2) * (100.0 - _guessed_power)) / 100.0,
		INDICATOR_WIDTH - 1,
		RADAR_H - 2,
		c2
	);

	rect(RADAR, 0, 0,
		INDICATOR_WIDTH,
		RADAR_H - 1,
		grijs
	);

	/* Weapon indicator */
	rectfill(RADAR, INDICATOR_DISTANCE_BETWEEN + INDICATOR_WIDTH + 1,
		1,
		(INDICATOR_WIDTH * 2) + INDICATOR_DISTANCE_BETWEEN - 1,
		((RADAR_H - 2) * (100.0 - _bullets)) / 100.0,
		c1
	);
	rectfill(RADAR, INDICATOR_DISTANCE_BETWEEN + INDICATOR_WIDTH + 1, 
		((RADAR_H - 2) * (100.0 - _bullets)) / 100.0,
		(INDICATOR_WIDTH * 2) + INDICATOR_DISTANCE_BETWEEN - 1,
		RADAR_H - 2,
		c2
	);
	rect(RADAR, INDICATOR_DISTANCE_BETWEEN + INDICATOR_WIDTH,
		0,
		(INDICATOR_WIDTH * 2) + INDICATOR_DISTANCE_BETWEEN,
		RADAR_H - 1,
		grijs
	);

	x = head;
	while (x)
	{
		if (x->bullet != 1)
		{

			if (x->id == our_id)
				color = makecol(255,255,255);
			else
				color = makecol(128,255,128);

			
			if (x->bot == 1)
				circlefill(RADAR, (INDICATOR_WIDTH * 2) + (INDICATOR_DISTANCE_BETWEEN * 2) + x->x / RADAR_SCALE, x->y / RADAR_SCALE, 1, color);
			else 
				circlefill(RADAR, (INDICATOR_WIDTH * 2) + (INDICATOR_DISTANCE_BETWEEN * 2) + x->x / RADAR_SCALE, x->y / RADAR_SCALE, 1, color);

		}
		x = x->next;
	}
}

void set_map_coords()
{
		MAP_X = our_node->x2 - MAP_W / 2;
		if (field_mode == 1)
		{
			if (MAP_X < 0 || field_width <= (unsigned)MAP_W) 
				MAP_X = 0;
			if ((unsigned)MAP_X > ((field_width_2 + 1) - MAP_W)) 
				MAP_X = (field_width_2 + 1) - MAP_W;
		}

		MAP_Y = our_node->y2 - MAP_H / 2;
		if (field_mode == 1)
		{
			if (MAP_Y < 0 || field_height <= (unsigned)MAP_H ) 
				MAP_Y = 0;
			if ((unsigned)MAP_Y > ((field_height_2 + 1) - MAP_H)) 
				MAP_Y = (field_height_2 + 1) - MAP_H;
		}
}

void zoom_(int i)
{
retry: 
	if (i == 1)
	{
		if (BLOCKSIZE_2 <= 60)
		ZOOM += 1;
	}
	else if (i == 0)
	{
		if (BLOCKSIZE_2 >= 5)
			ZOOM -= 1;
	}
	else 
		return;

	if (ZOOM > 0.0)
	{
		BLOCKSIZE_2 = BLOCKSIZE * (1 + ZOOM);
		if ( (BLOCKSIZE_2 % 5) != 0 && BLOCKSIZE_2 > 5)
			goto retry;
	}
	else if (ZOOM < 0.0)
	{
		BLOCKSIZE_2 = BLOCKSIZE / (1 - ZOOM);		
		if ( (BLOCKSIZE_2 % 5) != 0 && BLOCKSIZE_2 > 5)
			goto retry;
	}
	else 
	{
		BLOCKSIZE_2 = BLOCKSIZE;
	}	

/*	if ( (BLOCKSIZE_2 % 5) != 0 )
		BLOCKSIZE_2 = ((int)(BLOCKSIZE_2 / 5)) * 5;
		*/

	field_width_2 = X_BLOCKS * BLOCKSIZE_2;
	field_height_2 = Y_BLOCKS * BLOCKSIZE_2;

	drawmap();
	
	destroy_bitmap(shipbuff);
	shipbuff = create_bitmap(field_width_2 + 1, field_height_2 + 1);
}