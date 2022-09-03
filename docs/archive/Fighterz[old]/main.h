void printff(char *pattern, ...);
void terminate();
void delay(int secs);
void loadmap();
void drawmap();
void drawship();
void moveships();
void moveship(unsigned int id2, unsigned long int t2);
void mainloop();
void fps_proc();
void time_proc();
void init();
void printulist();
void printul(char *nick, long int kills, long int killavg);
void check_keys();
void check2_keys();

/* linked list */
struct data {
    unsigned int id; /* sock_nr */
    char nick[64];
	/* coords: */
	double x;
	double y;
	double deg; /* degrees (where it's heading towards) */
	unsigned long int t; /* our players time */
	unsigned int move; /* how many steps should this ship be moved */
	unsigned int velocity; /* can contain the following values:
	                           0 = no velocity
	                           1 = 50% velocity
	                           2 = 100% velocity
	                          maximum speed is defined in MOVE_INTERVAL */
	long int kills;
	long int kills_avg;
	long int score_update; /* contains time(NULL); used for: */
	unsigned long int alive; /* amount of seconds that this player is online */
	
    struct data *next;
};

typedef struct data user;
typedef user *LINK;

LINK head = NULL;
LINK new_node = NULL;
LINK current = NULL;
LINK our_node = NULL;