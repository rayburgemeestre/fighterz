/********************************************
*  COMPOUNDS
****************************************************/

/** 
 *   This struct contains data for every player, bot and bullet.
 *	 It will be used as a linked list.
 */
struct data {
    unsigned int id; /**< unique object id, filedescriptor of socket or number >1000 for bots */
	int bot; /**< indicates whether or not this object is a bot (1 means it is) */
    char nick[64]; /**< Nickname */
	double x; /**< X position of object */
	double y; /**< Y position of object */
	double x2; /**< X position used for coordinates on screen */
	double y2; /**< Y position used for coordinates on screen */
	unsigned int power; /**< contains the amount of impacts the object can withstand */
	double deg; /**< degrees (0 to 360), where it's heading towards */
	unsigned long t; /**< contains the time of the object (used for movement calculations) */
	unsigned int move; /**< pending amount of times this ship should be moved */
	double speed; /**< speed of object (between -0.20 and 0.20, 0.20 defined in max_speed!!!!!) */
	double max_speed;
	unsigned long vel_time; /**< used to calculate the thrust speed etc, 
								 this is (just as 't') all relative to time ever 
	                             X millisecs increase speed with X etc.. */
	int freeze; /**< Indicates whether the object can move or not (overrules speed),
	                 required to prevent some desync. */
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
	unsigned long turn_t; /**< last time spinning, used to calculate turning speed
							   relative to time.. */
	long int kills; /**< amount of fraggs the player/bot has  */
	long int kills_avg; /**< accuracy/whatever relative to the other players..
	                         i haven't what i want exactly :) */
	unsigned long alive; /**< Amount of seconds that this player is online (not used yet) */
	
/* Bullet additions: */
	int bullet; /**< indicates whether or not this object is a bullet (1 means it is) */
	double start_x; /* start_x & start_y nog ff checken waar 't gebruikt voor word */
	double start_y; /* en of het nodig is */
	struct data *owner; /**< A pointer to the struct of the ship who owns this bullet */
	unsigned int TTL; /**< Bullet's Time To Live in milliseconds */
	unsigned int hit_t; /**< time ship got impact from bullet.. used for impact effect */
	int impact; /**< if set to 1 the ship got impact */
    int invincible; /**< if this is set 1, the ship cannot fire and is shielded. if 2 also shielded but yellow (godmode) */
	int invincibility_t; /**< amount of milliseconds to stay invincible */
	int invincibility_t2; /**< timer that comes along with it.. <br>
							   if ( (servertime - invincibility_t2) > invincibility_t ) then
	                           invincibility drops. */
	int dead; /**< 1 = yes, and therefore invisible on map */
    struct data *next; /**< next object in list */
};

typedef struct data user; /**< typedef for the structure user */
typedef user *LINK; /**< pointer to { typedef for the struct user } */

/** 
 *   This struct will contain all the explosion circles used in the game
 *	 It will be used as a linked list.
 */
struct data2 {

	double x; /**< X position on screen for circle center */
	double y; /**< Y position on screen for circle center */
    int color; /**< Circles color */
	int radius; /**< The initial radius of the circle */
	int cradius; /**< The current radius of the circle */
	int maxradius; /**< The maximum radius of the circle */
	int speed; /**< The speed which the circle grows
					(increase size every _speed_ milliseconds) */
	unsigned long t; /**< Used to constrain speed of explosion to time */

    struct data2 *next; /**< Pointer to next circle in linked list */
};

typedef struct data2 _explosion; /**< typedef for the structure data */
typedef _explosion *LINK2; /**< pointer to { typedef for the struct data } */

/** 
 *   This struct contains the option menu's data
 *	 It will be used as a linked list.
 */
struct option_ 
{
	int x; /**< X position of item on the screen */
	int y; /**< Y position of item on the screen */
	int r, g, b; /**< Items colors */
	unsigned long time; /**< Used to constrain time to the highlight effect */
	int flicker; /**< Flicker on or off */
	char str[100]; /**< The caption of the item */
};


struct bg_imgs {
	// df_id, pos_x, pos_y, datafile
	int df_id;
	int pos_x;
	int pos_y;
	char datfile[128];
};
