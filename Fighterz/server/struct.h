/* Some defines */
#define PATH_MAX_ 513 /**< Maximum amount of coordinates that can be stored
						 for a bots path calculation. The higher this number,
						 the longer the path length can be at its maximum.
						 If there are to more coordinates necessary then this,
						 the path will terminate earlier. */

#ifndef TF_TIME_DEFINED
typedef signed long TIME;
#define TF_TIME_DEFINED
#endif
typedef unsigned int ID;

/** Object class
 * This struct contains data for every player, bot and bullet.
 * It will be used as a linked list.
 */
struct data {
    ID id; /**< unique object id, filedescriptor of socket or number >1000 for bots */
	int enabled; /**< Object enabled? 1=yes, 0=no */
	int type;	/**< Object type (T_BOT, T_PLAYER, T_BULLET) */
    char nick[NICKLEN]; /**< Nickname */
	/* coords: */
	double x; /**< X position of object */
	double y; /**< Y position of object */
	unsigned int power; /**< contains the amount of impacts the object can withstand */
	double deg; /**< degrees (0 to 360), where it's heading towards */
	TIME t; /**< contains the time of the object (used for movement calculations) */
	unsigned int move; /**< pending amount of times this ship should be moved */
	// unsigned int direction = 1; | direction, 0 = back, 1 = forward
	// ^ now we use velocity for this
	double speed; /**< speed of object (between -0.20 and 0.20 (0.20 defined in max_speed!!!!) */
	double max_speed;
	TIME vel_time; /**< used to calculate the thrust speed etc, 
								 this is (just as 't') all relative to time ever 
	                             X millisecs increase speed with X etc.. */
	int velocity;
	/**< Velocity of object.
	 * Can contain the following values:<br>
	 * -1 = direction is backwards<br>
	 *  0 = no direction (still) <br>
	 *  1 = direction is forward<br>
	 *  <br>
	 *  velocity used to be what speed is now.. and should better be named
	 *  direction istead of velocity.
	 */
	int turn;
	/**< Turn direction.
	 * -4 = Turn left fast<br>
	 * -2 = Turn left 'slow'<br>
	 *  0 = Don't turn<br>
	 *  2 = Turn right 'slow'<br>
	 *  4 = Turn right fast<br>
	 */
	TIME turn_t; /**< last time spinning, used to calculate turning speed
							   relative to time.. */
#if 0
	long int kills; /**< amount of fraggs the player/bot has  */
	long int kills_avg; /**< accuracy/whatever relative to the other players..
	                         i haven't what i want exactly :) */
#endif
	signed short frags; /**< amount of fraggs the player/bot has */
	unsigned int alive; /**< Amount of seconds that this player is online (not used yet) */

	unsigned int init_lag; /**< Amount of lag checks to go until player can join the party */
	TIME last_lag_check; /**< time when last lag check was done */
	
	double lag[5]; /**< Used for calculating lag: will keep the last 5 lag indications
	                    and throughout the program the average of all 5 indications will be
	                    considered as the players lag */
	double path[PATH_MAX_][2]; /**< When a pathcalculation is made, all coordinates are stored in here.
									[PATH_MAX_-1][0] is used to store the amount of coordinates stored and 
									[PATH_MAX_-1][1] contains the current coordinate the bot is heading towards */
	double bot_x; /**< used for calculations on the map the bots use */
	double bot_y; /**< used for calculations on the map the bots use */
	double diff_x; /**< used for calculations on the map the bots use */
	double diff_y; /**< used for calculations on the map the bots use */

	/* after every move a bot makes when flying to a coordinate in its find path routine, 
	   the distance between himself & the target is measured. If it somehow flew by his
	   target the bot's previous diff_x & diff_y will be smaller then the step it is about 
	   to make, and it will stop and no longer fly to that point (because it will not get
	   any closer) but go to its next coordinate (if any). */


/* Bot additions */
	int bot; /**< indicates whether or not this object is a bot (1 means it is) */
	double target_x; /**< the bot its current target, where it should fly to now according
	                      to the calculations it made in the findpath routine */
	double target_y; /**< the bot its current target, where it should fly to now according
	                      to the calculations it made in the findpath routine */

/* Bullet additions: */
	int bullet; /**< indicates whether or not this object is a bullet (1 means it is) */
	struct data *owner; /**< a link to the struct of the owner of this bullet */
	TIME TTL; /**< Bullet's Time To Live in milliseconds */
	
	unsigned int bulletcnt; /**< A player/bot its current amount of bullets in the field
	                             (so when this is 5, 5 bullets are already floating. When these 
								 stop bulletcount will be decreased again) */
	unsigned long bullet_time;
	
	char *buffer;	/**< Client input buffer (used to store partially received data ) */
	int inlen;		/**< Client input buffer length */

    int invincible; /**< if this is set 1, the ship cannot fire and is shielded. if 2, also shielded but yellow (godmode) */
	unsigned int invincibility_t; /**< amount of milliseconds to stay invincible */
	TIME invincibility_t2; /**< timer that comes along with it.. <br>
							   if ( (servertime - invincibility_t2) > invincibility_t ) then
	                           invincibility drops. */

	int dead; /**< 0=no, 1 = yes, 2=yes and ready to respawn */
	int freeze; /**< 1= using the servers calculations this user has collided, 
					 the user won't be moved any longer, but we do not change velocity/speed
					 variables */
	TIME died; /**< time player/bot died, used for the respawn delay */
	char *errmsg; /**< Error msg to be sent */
	int shiptype;
	signed char team; /**< team: 0=blue,1=red */

    struct data *next; /**< next object in list */
};

typedef struct data user; /**< typedef for the structure data */
typedef user *LINK; /**< pointer to { typedef for the struct data } */

/** flag positions */
struct flagpositions {
	int current; /**< Current path we're calculating, because there are 
	                  always 4 directions there should be calculated (example:
					  because flying an entire different direction might be faster..
	                  if there is an opening you can warp..) */
	int length1; /**< will contain the length of path calculation 1 */
	int length2; /**< will contain the length of path calculation 2 */
	int length3; /**< will contain the length of path calculation 3 */
	int length4; /**< will contain the length of path calculation 4 */
};

/**
 *	Struct to store locations (on map not coordinates)
 *  used for flags/spawnlocatioins etc
 */
struct location 
{
	int x; /* x-blocks on map */
	int y; /* y-blocks on map */
};


/* Lol, ja nu weet ik het echt niet meer, document jij maar ;)) -- Syz
   
   heheh die ELIST bagger etc is ook van die goochem die dat dijkstra algoritme
   prog gemaakt had :) moet ofzo een priority queue voorstellen.. volgens mij gewoon 
   een soort linked list ofzo ;) -- Trig */

/* -snip- */

// struct to store bg images etc.......
struct bg_imgs {
	// df_id, pos_x, pos_y, datafile
	int df_id;
	int pos_x;
	int pos_y;
	char datfile[128];
};

struct bg_imgs bg_imgs_data[32];