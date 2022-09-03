/********************************************
*   This will become the remake of tachyon fighters,
*   I'm still thinking about the new name I will give it.
*   Work in progress.. ;)
*								Started: 07.08.02
****************************************************/

/* #define ALLEGRO_STATICLINK */
/* #define USE_CONSOLE */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <allegro.h>
#include <math.h>

/* for time */
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

#include "main.h"
#include "variables.h"


int main() 
{
	init();

	mainloop();

	/* Exit program. */
	allegro_exit();
	return 0; 
}	 

/* Some Allegro magic to deal with WinMain(). */
END_OF_MAIN();

/********************************************
*  GLOBAL FUNCTIONS
****************************************************/

/* WARNING: vsnprintf() wasn't available 
   char buf[512] could be overflowed */
void printff(char *pattern, ...) 
{
	char buf[512];
	
	va_list ap;
	va_start (ap, pattern);
	vsprintf(buf, pattern, ap);
	va_end(ap);

	if (TXTPTR >= (SCREEN_Y - 10)) {
		TXTPTR = 0;
		clear_to_color (screen, 0);
	} else {
		TXTPTR = TXTPTR + 10;
	}

	if (TXTPTR == 0) { 
		textprintf(screen, font, 10, 1, 15, buf);
	} else { 
		textprintf(screen, font, 10, TXTPTR, 15, buf);
	}
}

/* Prints userlist */
void printulist() 
{
	TXTPTR2 = TOP;
	textprintf(screen, font, LEFT_2, TXTPTR2 , 15, "Player List:");
	TXTPTR2 = TOP + 20;

	current = head;
	while (current != NULL)
	{
		printul((char *)(current->nick), (long int)(current->kills), (long int)(current->kills_avg) );
		current = current->next;
	}
	
}

/* Used in printulist(); */
void printul(char *nick, long int one, long int two) 
{
	char buf[128];
	/* again no snprintf().. arghl*/
	if (strlen(nick) > 30) nick[30] = '\0'; /* frag >30 */
	sprintf(buf, "%8s %4d %3d", nick, one, two);

	TXTPTR2 = TXTPTR2 + 10;

	if (TXTPTR2 == 0) { 
		textprintf(screen, font, LEFT_2, 1, 42, buf); /* 42 = makecol(255, 128, 0) (orange) */
	} else { 
		textprintf(screen, font, LEFT_2, TXTPTR2, 42, buf);
	}
}

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

	char buff[81];

	if (!(fileptr = fopen(map, "r"))) {
		printff("Error opening file");
		terminate();
	}

	while (fgets(buff, 81, fileptr) != NULL) {
		if ( (buff[0] == '0') || (buff[0] == '1') ) {
			strcpy(buff, strtok(buff, "\n"));
			
			if ( cnt == 0 ) X_BLOCKS = strlen(buff);
			
			buff[X_BLOCKS] = '\0';

			strcpy(field[cnt], buff);
	
			x_count++;

			cnt++;
			
		}
		if ( (buff[0] == '2') ) {
			strcpy(buff, strtok(buff, "\n"));
			ptr = &buff[2];
			BLOCKSIZE = atoi((const char *) ptr);
		}

		Y_BLOCKS = x_count;
	}
	printff("  Vertical blockcount: %d", Y_BLOCKS);
	printff("  Horizontal blockcount: %d", X_BLOCKS);
	printff("  Blocksize in pixels: %d", BLOCKSIZE);
}

/* procedure to draw map on screen */
void drawmap() {
	int cnt, cnt2;
	int x = 0, length, length2;

	/* Colours */
	int blauw = makecol(0,128,255);
	int grijs = makecol(128,128,128);

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
		line(screen, LEFT, (cnt * BLOCKSIZE) + TOP, length + LEFT, (cnt * BLOCKSIZE) + TOP, grijs);
	}
	line(screen, LEFT, (cnt * BLOCKSIZE) + TOP, length + LEFT, (cnt * BLOCKSIZE) + TOP, grijs);

	/*   (vertical:) */
	for (cnt=0; cnt<X_BLOCKS; cnt++)
	{
		line(screen, LEFT + (cnt * BLOCKSIZE), TOP, LEFT + (cnt * BLOCKSIZE), TOP + length2, grijs);
	}
	line(screen, LEFT + (cnt * BLOCKSIZE), TOP, LEFT + (cnt * BLOCKSIZE), TOP + length2, grijs);

	/* Draw walls */
	for (cnt=0; cnt<X_BLOCKS; cnt++) 
	{
		for (cnt2=0; cnt2<Y_BLOCKS; cnt2++)
		{
			if (field[cnt2][cnt] == '1') 
			{
				rectfill(screen, LEFT + (cnt * BLOCKSIZE), TOP + (cnt2 * BLOCKSIZE), LEFT + ((cnt + 1) * BLOCKSIZE), TOP + ((cnt2 + 1) * BLOCKSIZE), blauw);
			}
		}
	}
}


/********************************************
*  SHIP FUNCTIONS
****************************************************/

void drawships() 
{
	int groen = makecol(0, 255, 128);
	/* BLOCKSIZE = diameter */
	int radius;	
	double ret;
	double pos_x, pos_y; /* ship's x,y */
	int left_wing_deg, right_wing_deg; 
	double lw_x, lw_y; /* leftwing x,y */
	double rw_x, rw_y; /* rightwing x,y */
	
	radius = BLOCKSIZE / 2;

	current = head;
	while (current != NULL)
	{
		/**********************
		 **   CALCULATIONS   **
		 **********************/

		/* ship coordinates */
		ret = (PI / 180) * (current->deg - 90);

		pos_x = cos(ret);
		pos_x = pos_x * (radius - (BLOCKSIZE / 4));
		pos_x = pos_x + radius;
		pos_x = pos_x + current->x - (BLOCKSIZE / 2);

		pos_y = sin(ret);
		pos_y = pos_y * (radius - (BLOCKSIZE / 4));
		pos_y = pos_y + radius;
		pos_y = pos_y + current->y - (BLOCKSIZE / 2);

		/* wing coordinates */
		left_wing_deg = current->deg + 120;
		right_wing_deg = current->deg - 120;

		/* left wing */
		ret = (PI / 180) * (left_wing_deg - 90);
		lw_x = cos(ret);
		lw_x = lw_x * radius;
		lw_x = lw_x + radius;
		lw_x = lw_x + current->x - (BLOCKSIZE / 2);

		lw_y = sin(ret);
		lw_y = lw_y * radius;
		lw_y = lw_y + radius;
		lw_y = lw_y + current->y - (BLOCKSIZE / 2);

		/* right wing */
		ret = (PI / 180) * (right_wing_deg - 90);
		rw_x = cos(ret);
		rw_x = rw_x * radius;
		rw_x = rw_x + radius;
		rw_x = rw_x + current->x - (BLOCKSIZE / 2);

		rw_y = sin(ret);
		rw_y = rw_y * radius;
		rw_y = rw_y + radius;
		rw_y = rw_y + current->y - (BLOCKSIZE / 2);

		/**********************
		 **     DRAW SHIP    **
		 **********************/
		
		/* ship base */
		line(screen, LEFT + current->x, TOP + current->y, LEFT + pos_x, TOP + pos_y, groen);
		/* left wing */
		line(screen, LEFT + pos_x, TOP + pos_y, LEFT + lw_x, TOP + lw_y, groen);
		line(screen, LEFT + current->x, TOP + current->y, LEFT + lw_x, TOP + lw_y, groen);
		/* right wing */
		line(screen, LEFT + pos_x, TOP + pos_y, LEFT + rw_x, TOP + rw_y, groen);
		line(screen, LEFT + current->x, TOP + current->y, LEFT + rw_x, TOP + rw_y, groen);

		/* next ship */
		current = current->next;
	}
}

void moveships()
{
	current = head;
	while (current != NULL)
	{
		if ( (current->velocity == 1) || (current->velocity == 2) )
		{
			moveship(current->id, ourtime);
		}

		current = current->next;
	}
}
void moveship(unsigned int id2, unsigned long int t2)
{
	long int diff = 0, cnt;
	int radius;
	double ret;
	double pos_x, pos_y;

	current = head;
	while (current != NULL)
	{
		if (current->id == id2)
		{
			/* move this ship*/
			if (current->t > t2)
			{	/* time is not synched correctly */
				/* todo: call function to sync again*/
				return;
			}
			diff = (ourtime - current->t);
			if (current->velocity == 1)
			{
				diff = diff / (MOVE_INTERVAL / 2);
				current->t += (MOVE_INTERVAL / 2) * diff;
			} else if (current->velocity == 2) {
				diff = diff / MOVE_INTERVAL;
				current->t += MOVE_INTERVAL * diff;
			} else {
				return;
			}

			radius = BLOCKSIZE / 2;
			ret = (PI / 180) * (current->deg - 90);


			for (cnt=0; cnt<diff; cnt++)
			{
				pos_x = cos(ret);
				pos_x = pos_x * 0.5 ; /* ? */
				pos_x = pos_x + radius;
				pos_x = pos_x + current->x - (BLOCKSIZE / 2);

				pos_y = sin(ret);
				pos_y = pos_y * 0.5;
				pos_y = pos_y + radius;
				pos_y = pos_y + current->y - (BLOCKSIZE / 2);

				current->x = pos_x;
				current->y = pos_y;
				
				/* did the ship fly out the field? */
				if (current->y < 0) 
				{	// *warp*
					current->y = field_height - 1;
				} else if (current->y > field_height) {
					current->y = 1;
				} else if (current->x < 0) {
					current->x = field_width - 1;
				} else if (current->x > field_width) {
					current->x = 1;
				}
			}

			return;
		}
		current = current->next;
	}
}

/*

 Public Sub CHECK_SHIP_POSITIONS()
    Dim i As Integer
    For i = 0 To MAX_PLAYERS
        If PLAYER(i).y1 < 0 Then
            ' *warp* :)
            PLAYER(i).y1 = FHEIGHT - 1
        ElseIf PLAYER(i).y1 > FHEIGHT Then
            PLAYER(i).y1 = 1
        ElseIf PLAYER(i).x1 < 0 Then
            PLAYER(i).x1 = FWIDTH - 1
        ElseIf PLAYER(i).x1 > FWIDTH Then
            PLAYER(i).x1 = 1
        End If
        ' ow and _btw_ check if invincibility counter needs to
        ' be decreased..
        If PLAYER(i).invincible = True Then
            PLAYER(i).invcount = PLAYER(i).invcount - 1
            If PLAYER(i).invcount = 0 Then
                ' invincibility turned off
                PLAYER(i).invincible = False
                If i = 0 Then FIELD_.addSTATUS "STATUS: NO LONGER INVINCIBLE"
            End If
        End If
    Next i
End Sub

  */
/********************************************
*  PROCEDURES
****************************************************/

void mainloop()
{
	char buf[128];
	int groen = makecol(0, 255, 128);

/* move to our node in the player list */
current = head;
while ( (current != NULL) && (current->id != our_id) )
{
	current = current->next;
}
our_node = current;

	/* loop until escape key is pressed */
	while(! key[KEY_ESC]) {
		/* cls */
		clear_to_color (screen, 0);

		/* calc fps */
		fps_proc();

		/* draw map */
		drawmap();
		
		/* move ships */
		moveships();

		/* draw ships */
		drawships();
		
		/* show fps? */
		if (show_fps == 1) textprintf(screen, font, LEFT + 10, TOP, 15, "FPS: %d", fps);

		/* playerlist */
		if (show_ulist == 1) printulist();

		/* check keyboard */
		check_keys();
		
		/* Arrow keys */
		if (key[KEY_RIGHT] || key[KEY_LEFT]) 
		{	
			/* arrow_keys isn't a very suitable name since
			   i split key down/up from this part */
			if (arrow_keys == 0) 
			{
				arrow_keys = ourtime;
			}

			if ((ourtime - arrow_keys) >= KEY_INTERVAL)
			{
				if (key[KEY_RIGHT])
				{
					our_node->deg += (key[KEY_LSHIFT]?4:2);
					if ( (our_node->deg == 362) || (our_node->deg == 364) )
					{
						our_node->deg = (key[KEY_LSHIFT]?4:2);
					}
				}
				if (key[KEY_LEFT])
				{
					our_node->deg -= (key[KEY_LSHIFT]?4:2);
					if ( (our_node->deg == -2) || (our_node->deg == -4) )
					{
						our_node->deg = (key[KEY_LSHIFT]?356:358);
					}
				}
			}
		} else {
			/* none of the keys is pressed */
			arrow_keys = 0;
		}
		if (key[KEY_UP] || key[KEY_DOWN]) 
		{
			if (thrust == 0)
			{
				thrust = ourtime;
			}
			/* you have to press up/down for 200 ms in order to get effect */
			if ((ourtime - arrow_keys) >= 200)
			{
				if (key[KEY_UP])
				{
					if (our_node->velocity == 0) our_node->velocity = 1;
					else if (our_node->velocity == 1) our_node->velocity = 2;
				}
				if (key[KEY_DOWN])
				{
					if (our_node->velocity == 2) our_node->velocity = 1;
					else if (our_node->velocity == 1) our_node->velocity = 0;
				}
			}
		} else {
			thrust = 0;
		}

		check_option_keys++;

		check2_keys();
		
		/* time calculation procedure */
		time_proc();

		show_mouse(screen);

		poll_mouse();
		sprintf(buf, "x:%4d y:%4d", mouse_x, mouse_y);
		textprintf(screen, font, LEFT + (SCREEN_X - 150), (SCREEN_Y - 10), 15, buf);


		/* delay */
		rest(8);
		/* wait for vertical retrace (reduces flickering) */
		vsync();
		poll_keyboard(); /* This shouldn't be necessary in Windows. */
	}
}

void init()
{
	/* Initialize Allegro. */
	allegro_init();	  

	/* Set the resolution with SAFE autodetection. */
	set_gfx_mode(GFX_SAFE, SCREEN_X, SCREEN_Y, 0, 0);
	//set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_X, SCREEN_Y, 0, 0);
	
	install_keyboard();
	install_mouse();
	install_timer();

	printff("Initializing:");	
	
	printff("Calling time procedure");
	time_proc();
	
	printff("Loading map: %s", map);

	strncpy(map, "maps/lvl1.txt", 64);
	map[64] = '\0';


	/* Read map from file and set some important variables */
	loadmap();

	/* 
		printff("Building map in buffer");
		buildmap();
	*/


	printff("Some variables %s", map);

	LEFT_2 = (SCREEN_X - 150) - LEFT;
	/*					^ lists width */
	field_width = X_BLOCKS * BLOCKSIZE;
	field_height = Y_BLOCKS * BLOCKSIZE;
	printff("field_width: %d", field_width);
	printff("field_height: %d", field_height);

	printff("Initializing linked list for users");

	/* create first node in linked list */
	if (!(new_node = (LINK)malloc(sizeof(user))))
	{   printff("Error allocating memory");
		exit(-1);
	}
	/* insert the created node */
	new_node->next = head;
	head = new_node;
	/* intiate some values */
	head->id = 0;
	head->x = 150;
	head->y = 0;
	head->deg = 180;
	head->t = ourtime;
	head->move = 0;
	head->velocity = 2;
	head->alive = 0; /* seconds in the game*/
	head->score_update = time(NULL);
	head->kills = 0;
	head->kills_avg = 0;
	strcpy(head->nick, "TriGen");

	/* DEBUG::::: */
	if (!(new_node = (LINK)malloc(sizeof(user))))
	{   printff("Error allocating memory");
		exit(-1);
	}
	/* insert the created node */
	new_node->next = head;
	head = new_node;
	/* intiate some values */
	head->id = 1;
	head->x = 50;
	head->y = 70;
	head->deg = 45;
	head->t = ourtime;
	head->move = 0;
	head->velocity = 2;
	head->alive = 0; /* seconds in the game*/
	head->score_update = time(NULL);
	head->kills = 0;
	head->kills_avg = 0;
	strcpy(head->nick, "Player2");

	/* ///////DEBUG::::: */

	/* initiate some values */
	fps_oldtime = time(NULL);
	
	printff("Press enter to proceed..");
	while(! key[KEY_ENTER]);

}


/********************************************
*  GLOBAL PROCEDURES
****************************************************/

/* exit program */
void terminate() {
	delay(5);
	allegro_exit();
	exit(-1); 
}

/* delay proc */
void delay(int secs) {
	long int oldtime, newtime;
	
	oldtime = newtime = time(NULL);
	
	while ( (newtime - oldtime) != secs ) {
		newtime = time(NULL);	
	}
}

/* fps_proc */
void fps_proc()
{
	fps_newtime = time(NULL);

	// Change fps var..?
	if (fps_oldtime!= fps_newtime) {
		fps_oldtime = fps_newtime; 
		fps = fps_count; /* set fps */
		fps_count = 0; /* reset fps counter */
	}

	fps_count++; /* increase frame counter */
}

/* time_proc */
void time_proc() 
{
	char buf[128];

	ftime(& t);
	/* this var is used in the entire program */
	ourtime = ((t.time * 100) + (t.millitm / 10));
	sprintf(buf, "%u", ourtime  );
	/* debug: */
	textprintf(screen, font, LEFT + 10, (SCREEN_Y - 10), 15, buf);

}

/* keyboard procedure */
void check_keys()
{
	if (key[KEY_F2]) f2_pressed = 1; /* mark f2 as pressed */
	if (key[KEY_F6]) f6_pressed = 1; 
}

void check2_keys()
{
	/* check function keys */
	if (check_option_keys == 50) 
	{	
		check_option_keys = 0;

		/* player list */
		if (f2_pressed == 1) {
			f2_pressed = 0;
			if (show_ulist == 1) 
			{	
				show_ulist = 0;
			} else {
				show_ulist = 1;
			}
		}
		
		/* fps counter */
		if (f6_pressed == 1) {
			f6_pressed = 0;
			if (show_fps == 1) 
			{	
				show_fps = 0;
			} else {
				show_fps = 1;
			}
		}
	}
}