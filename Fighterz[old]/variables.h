#define MAX_F_LEN 81 /* max length, used when reading files (fgets buffer) */
#define PORT 8099
#define PI 3.14159265358979

/* Variables used in the entire program */
//int SCREEN_Y = 480;
int SCREEN_Y = 600;
//int SCREEN_X = 640;
int SCREEN_X = 800;

double MOVE_INTERVAL = 0.8; /* move every ?? millisecs */
double KEY_INTERVAL = 1; /* move every ?? millisecs */

int TXTPTR = 0; /* Used for textprintf(); */
int TXTPTR2 = 0; /* Used for printul(); */

char map[64]; /* Will contain the current map's filename + path */
//char field[21][31]; 
char field[76][126]; /* Think this max. values are fine. */
// BITMAP *fieldbuff; 
				     /* Buffer to store field in, once created
                        so constantly rebuilding it won't be necessary */ 

int Y_BLOCKS = 0; /* Amount of blocks vertically */
int X_BLOCKS = 0; /* Amount of blocks vertically */
int BLOCKSIZE = 0; /* in pixels */
unsigned long int field_width, field_height; /* will contain field width/height */

int LEFT = 20;
int TOP = 50;


/* will contain frames per second */
int fps = 0; 
/* Used to calculate fps */
int fps_count = 0;
long int fps_oldtime, fps_newtime;
/* Show fps ? */
int show_fps = 1;

/* Userlist vars */
int LEFT_2; /* Where the playerlist should be situated */
/* Show playerlist*/
int show_ulist = 1;

struct timeb t; 
unsigned long int ourtime = 0; /* will contain ((t.time * 100) + (t.millitm / 10)) */

/* keys */
int f2_pressed = 0; /* Playerlist */
int f6_pressed = 0; /* Frames per second */
int right_arrow = 0;
int check_option_keys = 0; /* every 50 frames i want 
	the function keys to be checked */
unsigned long int arrow_keys = 0;
unsigned long int thrust = 0;

unsigned int our_id = 0; /* TODO will be modifyed */