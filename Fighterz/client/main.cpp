/********************************************
*   This will become the remake of my vb version,
*
*								Started: 07.08.02
****************************************************/

/*

	The reason why this is all in .cpp extensions is because 
	I needed some C++ code, so i couldn't stick to .c

*/


#include "common.h"

/********************************************
*  VARIABLES
****************************************************/

MIDI *bg_music;
MIDI *intro_music;
/* (sub)bitmap buffers */
BITMAP *tmpscreen; /**< Primary bitmap screen buffer used to draw everything on */
BITMAP *fieldbuff; /**< Screen bitmap buffer used to draw the field to once (after that this 
                        bitmap will be constantly blitted to tmpscreen )*/
BITMAP *shipbuff; /**< Screen bitmap buffer where the ships/bullets are drawn to */
BITMAP *talkbuff; /**< sub bitmap buffer on tmpscreen used to print the (chat) messages on */ 
BITMAP *ulistbuff; /**< sub bitmap buffer on tmpscreen used to print the userlists on */ 
BITMAP *CONSOLE; /**< sub bitmap buffer on tmpscreen used to print the (console) text / commands on */ 
BITMAP *RADAR; /**< sub bitmap buffer on tmpscreen used to print the radar on */ 
int SCREEN_Y; /**< Screen height in pixels */
int SCREEN_X; /**< Screen width in pixels */
int FULLSCREEN; /**< indicates whether to use fullscreen or not, 1 = yes */
int HIGH_GRAPHICS; /**< indicates whether to try and make smoother graphics: adds: vsync() + rest() */
int BOUNCING_BULLETS; /**< indicates whether bouncing bullets is activated or not */
int MOVE_STEPW; /**< Amount of times to move will be multiplied by this. So
                     i recommend you to let this stay @ 1. But if you make it 2
                     all the ships  shall go twice as fast, because instead of
                     moving the ships 1 time, it will be done 2 times. */
//DEPR: double SPEED; /**< ship/bot: pixels to move every step :) 
                   //should be in sync with clients, example: 0.20 (pixels) */
double B_SPEED; /**< bullet: pixels to move every step :) 
					 should be in sync with clients, example: 0.40 (pixels) */
/* ON SCREEN: example; if you are zoomed in everything should move twice as fast 
   but in 'reality' you still move as fast as in SPEED.. */
double SPEED_2; /**< ON SCREEN ship/bot: pixels to move every step :) 
                     should be in sync with clients, example: 0.20 (pixels) */
double B_SPEED_2; /**< ON SCREEN bullet: pixels to move every step :) 
					   should be in sync with clients, example: 0.40 (pixels) */
int TXTPTR; /**< Text 'pointer', will contain positions of the text on the screen
               used in textprintf(); */
int TXTPTR2; /**< Text 'pointer', will contain positions of the text on the screen
               used in textprintf(); */
char map2[256]; /**< Contains active map's filename */
char field[500][500]; /**< Used to store map data in */
int Y_BLOCKS; /**< Amount of blocks vertically on the map */
int X_BLOCKS; /**< Amount of blocks horizontally on the map */
int BLOCKSIZE; /**< Size of blocks in pixels (also the ships are made using this size) */
unsigned long field_width; /**< Contains field width in pixels */
unsigned long field_height; /**< Contains field height in pixels */
int BLOCKSIZE_2; /**< ON SCREEN Size of blocks in pixels (also the ships are made 
                      using this size) */
unsigned long field_height_2; /**< ON SCREEN Contains field width in pixels */
unsigned long field_width_2; /**< ON SCREEN Contains field height in pixels */
int LEFT; /**< Used margin from the left of the screen */
int TOP; /**< Used margin from the top of the screen */
int fps; /**< Contains the current amount of frames/sec */
int fps_count; /**< Used to calculate the frames/sec */
long int fps_oldtime; /**< Used to calculate the frames/sec */
long int fps_newtime; /**< Used to calculate the frames/sec */
int show_fps; /**< Indicates whether to show frames/sec on the screen, 1 = yes */
int show_ulist; /**< Indicates whether to show playerslist on the screen, 1 = yes */
int show_names; /**< Indicates whether to show player names on the screen 
                     in the field, 1 = yes */
int LEFT_2; /**< Distance from the left of the screen, where the player list should be situated */
struct timeb t; /**< used to calculate our time in millisecs (our_time) */
struct timeb started; /**< used to keep track of the time the game started for us as a client */
unsigned long ourtime; /**< the client's timer used for all time calculations in millisecs */
unsigned int our_id; /**< contains the id we currently have in game */
SOCKET theSocket; /**< the socket descriptor for our connection with server */
char receivebuff[512]; /**< buffer to store data received from recv(); */
#define MAX_C_LINES 12
#define MAX_C_LLENGTH 80
char console[MAX_C_LINES][MAX_C_LLENGTH]; /**< Array used to store the entire console with messages */
int STARTED; /**< set to 1 = yes, and the program now knows printff_direct will not print
                  to the screen anymore, but the an outfile  */
double lag[5]; /**< Array used to store the last 5 lag check results.. the average of these
                    5 lag indications will be considered as the current lag.. */
#if DEBUG == 1
FILE *boom; /**< debug file descriptor: used for logging coords */
FILE *dbug; /**< debug file descriptor: used for logging some data */
#endif
int showscr; /**< set to 1 = yes, and all graphics will be blitted on the screen */
/* TODO: STARTED & showscr should be combined, they serve the same purpose */
DATAFILE *dataf; /** link to allegro datafile which shall contain all fonts/graphics/sounds */
DATAFILE *df_snd; /** asdjflkasjdf */
unsigned long bullet_time; /**< used for delay in firing shots while holding fire key down */
int BULLET_COUNT; /**< current amount of bullets we own (in field/screen) */
int BULLET_MAX; /**< user can only have this amount of bullets at a time in the field */
int BULLET_TTL; /**< millisecs that a bullet can exist in the field (TTL) */
int BULLET_RE; /**< millisecs to wait until a new bullet is fired (when holding fire key down) */
int MOVE_BSTEPW; /**< Amount of times to move will be multiplied by this. So
                      i recommend you to let this stay @ 1. But if you make it 2
                      all the bullets shall go twice as fast, because instead of 
                      moving the bullets 1 time, it will be done 2 times. */
int USE_SOUND; /**< set to zero means sound is disabled */
double MAP_X; /**< X position on tmpscreen where the map will be blitted */
double MAP_Y; /**< Y position on tmpscreen where the map will be blitted */
double MAP_W; /**< Width of fieldbuff that will be blitted on tmpscreen */
double MAP_H; /**< Length of fieldbuff that will be blitted on tmpscreen */
/* Console */
unsigned int CONSOLE_X; /**< X position on tmpscreen where the console will be blitted */
unsigned int CONSOLE_Y; /**< Y position on tmpscreen where the console will be blitted */
unsigned int CONSOLE_W; /**< Width of the console that will be blitted on tmpscreen */
unsigned int CONSOLE_H; /**< Length of the console that will be blitted on tmpscreen */
/* Radar */
unsigned int RADAR_SIZE; /**< Height of the radar in pixels */
double RADAR_SCALE; /**< Will contain the scale of the 'real' field that will be used 
                         to create the radar */
unsigned int RADAR_W; /**< Will contain the width of the radar in pixels */
unsigned int RADAR_H; /**< Will contain the height of the radar in pixels */
unsigned int RADAR_X; /**< X position on tmpscreen where the radar will be blitted */ 
unsigned int RADAR_Y; /**< Y position on tmpscreen where the radar will be blitted */ 
unsigned int RADAR_SHOW; /**< if set to 1 radar will be shown on the screen.. */
unsigned int INDICATOR_WIDTH; /**< Width of indicators in pixels, height will be the same
                                   as the width of the radar */
unsigned int INDICATOR_DISTANCE_BETWEEN; /**< Space between each indicator in pixels */
unsigned int CSCREEN_X; /**< X position on tmpscreen where the console will be blitted */ 
unsigned int CSCREEN_Y; /**< Y position on tmpscreen where the console will be blitted */ 
unsigned int CSCREEN_H; /**< Height of the console, width will be the same as the 
                             width of the fields */ 
unsigned int FIELD_X; /**< X position on the fieldbuff which is currently visible */
unsigned int FIELD_Y; /**< Y position on the fieldbuff which is currently visible */
double ZOOM; /**< zoom size, can contain the following values:
                  0 = normal size, 1 = twice the size, -1 is half the size etc */
unsigned long zoom_time; /**< used to constrain the zooming in/out to time */
unsigned int MAX_HITS; /**< Contains the amount of bullet impacts a ship can have before it dies */
unsigned int guessed_power; /**< When we get hit at OUR side this instantly modifies 
                                 and yes, this indicator could be wrong, BUT
							     That doesn't matter, because we have 2 indicators
							     and one predicts our power and thats this one */
int talk; /**< Indicates whether you wish to talk, so keystrokes will be added to message 
               in console window */
unsigned int MAX_CHARS_IN_SENTENCE; /**< Maximum amount of characters possible in one sentance. */
char msg[80]; /**< used to store the message in */
int active_o_item; /**< Active item in the options menu */
int field_mode; /**< alpha: 2 = continues (endless map illusion) */

LINK head = NULL; /**< Pointer to the first node in the linked list */
LINK new_node = NULL; /**< Pointer used for new nodes in the linked list */
LINK current = NULL; /**< Pointer to the current selected node in the linked list */
LINK our_node = NULL; /**< Pointer that always points to our node in the linked list */
LINK2 e_head = NULL; /**< Pointer to the first node in the explosion linked list */
LINK2 e_new_node = NULL; /**< Pointer used for new nodes in the explosion linked list */
LINK2 e_current = NULL; /**< Pointer to the current selected node in the 
                             explosion linked list */
char nickname[64]; /**< Will contain our nickname */
char taddr[512]; /**< Will contain the host/ip of the server we will connect to */
int tport; /**< Will contain the port of the server we will connect to */
int grid; /**< true means a grid will be drawn on the field */
int direction_is_up; /**< indicates whether we are moving upwards or not */
struct option_ option[3]; /**< Will contain menu items in option menu */
struct bg_imgs bg_imgs_data[32];
int bg_imgs_data_index;

char *buffer;
int inlen; /**< input buffer length */

int can_spawn;
int requested_spawn;

// large_text(), large_text_draw();
char large_text_msg[64];
unsigned long large_text_time;
unsigned int large_text_display_time = 5; // secs

/********************************************
*  THE MAIN PROGRAM
****************************************************/

// int main(char *argv[], int argc)
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
	// char apppath[_MAX_PATH]; /* _MAX_PATH defined in windows.h */
	char *ptr, *ip, *port;
	int ret, skip_options = 0;

	while (!key[KEY_ESC])
	{
#if DEBUG == 1
		if ((dbug = fopen("debug.log", "w+")) == NULL)
		{	perror("SLDKFj");
			terminate();
		}
#endif
		initialize_vars();

		init(); /* initialize stuff */

		config( (strlen(lpszArgument) == 0 ? 0 : 1 ) );
		/* TEMPORARY: */
		//FULLSCREEN = 0;

		ptr = NULL; port = NULL;
		ptr = lpszArgument;
		/* 11 = strlen of: fighterz:// */
		if (strlen(ptr) >= 11)
		{
			ptr += 11;
			if (ptr != NULL)
			{
				/* ptr now contains ip :) */
				ip = ptr;
				ptr = NULL;
				ptr = strchr(ip, ':');
				if (ptr != NULL)
				{
					/* er zou nog een port moeten zijn */
					*ptr = '\0';
					ptr++;
					if (ptr != NULL)
					{
						/* er zit hier iets */
						port = ptr;
						ptr = strchr(ptr, '/');
						if (ptr != NULL)
						{
							*ptr = '\0'; /* weg met die crap */
						}
						tport = atoi(port);
					}
				}
				strcpy(taddr, ip);
				skip_options = 0;
			}
		}

		if (skip_options != 1)
		{
			play_midi(intro_music, TRUE);

			clear_to_color(screen, makecol(0,0,0));
			textprintf_centre(screen, (FONT *)dataf[ARCADE].dat, SCREEN_X / 2, 
				SCREEN_Y / 2, makecol(192, 192, 192), "Tachyon Fighterz");
			
			if (FULLSCREEN == 1)
				Sleep(2000); // extra delay for switching resolution..

			Sleep(1000);

			clear_to_color(screen, makecol(0,0,0));
			textprintf_centre(screen, (FONT *)dataf[ARCADE].dat, SCREEN_X / 2, 
				SCREEN_Y / 2, makecol(192, 192, 192), "www.fighterz.net");
			Sleep(1000);

		}

while (!key[KEY_ESC])
{
	//alert("in while!", "", "", "OK", NULL, KEY_ENTER, 1);
		if (skip_options != 1)
		{
			ret = getoptions();
		}
		else
		{
			ret = 0;
		}

		TXTPTR = 0;

		switch (ret)
		{
			case 0:
				if (-1 == start() ||
					-1 == mainloop())
				{
					WSACleanup();
#if DEBUG == 1
					fclose(dbug);
#endif DEBUG
					initialize_vars();
					play_midi(intro_music, TRUE);
				}
				break;
			case 1:
				alert("Singleplayer does not yet exist", 
					  "you are now being redirect to multiplayer", "", 
					  "Thanks for the info", NULL, KEY_ENTER, 0);
				if (-1 == start() ||
					-1 == mainloop())
				{
					WSACleanup();
#if DEBUG == 1
					fclose(dbug);
#endif DEBUG
					initialize_vars();
					play_midi(intro_music, TRUE);
				}
				break;
			case 2:
				getsettings();
				break;
			default:
				break;
		}
}
/*
			WSACleanup();
#if DEBUG == 1
			fclose(dbug);
#endif DEBUG
*/
		// we need a de_initiatlize_vars here
			// to free linked lists etc,
			// if we let the while continue, the program crashes, so:
			break;
	}

	destroy_midi(bg_music);
	destroy_midi(intro_music);

	/* exit program */
	allegro_exit(); 
	return 0; 
}	 
// END_OF_MAIN();