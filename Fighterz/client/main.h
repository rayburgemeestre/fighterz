/********************************************
*  FUNCTION PROTOTYPES
****************************************************/

extern void drawships();
extern void drawfps();
extern void drawexplosions();
extern void printff(char *pattern, ...);
extern void printff_direct(char *pattern, ...);
extern void terminate();
extern void delay(int secs);
extern void loadmap();
extern void drawmap();
extern void setrcoords();
extern void zoom_(int i);
extern void drawradar();
extern void set_map_coords();
extern void draw_talk_box();
extern void drawship();
extern void moveships();
extern void moveship(unsigned int id2, unsigned long t2);
extern void mainloop();
extern void center_window();
extern void fps_proc();
extern void time_proc();
extern void init();
extern void start();
extern void init_screen();
extern void printulist();
extern void printul(char *nick, long int two, unsigned int three);
extern void connect_socket();
extern void sockread();
extern int sockwrite(char *pattern, ...);
extern int parse(char *stuff);
extern int parse2(char *stuff);
extern void lag_proc(char *lag_char, char *servertime_char);
extern double current_lag();
extern void add_player(int _id, int _x, int _y, int _deg, 
				int _move, int _turn,  int _velocity, int _alive, 
				int _kills, int _killavg, int _bot, double _speed, int _dead, char *_nick);
extern void del_player(int sock_fd);
extern void drawconsole();
extern void config(int ignore);
extern void debug();
extern void addtext(char *pattern, ...);
extern void collidecheck();
extern int collidecheck2(unsigned int id, int variation);
extern double futureX(struct data *ptr, double speed);
extern double futureY(struct data *ptr, double speed);
extern double fbX(unsigned int id2);
extern double fbY(unsigned int id2);
extern struct data *add_bullet(struct data *_id, int _x, int _y, double _deg, unsigned long _time);
extern void del_bullet(struct data *ptr);
extern void movebullets();
extern void drawbullets();
extern int collidecheck2b(struct data *ptr);
extern int bs_collidecheck(struct data *ptr);
extern int getangle(double x1, double y1, double x2, double y2);
extern int getoptions();
extern double dabs(double x);
extern void setsec(struct data *ptr);
extern void explosion(double x, double y, int maxrad, int speed, int color);
extern int rexplosion(struct data2 *ptr);
extern int movebullet(struct data *ptr, unsigned long t2);
extern void initialize_vars();
extern void die(char *s);
extern void parse_input();
extern void show_graphics();
extern struct data *getplayer_byid(unsigned int id);

/* protocol */
extern void dopacket(int xtype, unsigned short len, char *dta);
extern void verbose(char *pattern, ...);
extern void m_hi(unsigned int sver, unsigned int oid);
extern void m_ping(unsigned int servertime);
extern void m_lag(double diff);
extern void m_nickreply(unsigned char acceptance, char *msg);
extern void m_spawnready();
extern void m_kill(unsigned int victimid, unsigned int evilid, char *killstr);
extern void m_nick(unsigned int id, char *nick);
extern void m_say(unsigned int id, char *msg);
extern void m_newbullet(unsigned int id, unsigned int ownerid, double x, double y, double deg);
extern void m_quit(unsigned int id, char *quit_msg);
extern void m_spawn(unsigned int id);
extern void m_respawn(unsigned int id);
extern void m_newuser(int id, double x, double y, double deg,
					  signed char accel, unsigned int alive, signed short frags,
					  unsigned int pending_moves, signed char turn, unsigned char type, 
					  double speed, char *nick);
extern void m_hit();
extern void m_accel(unsigned int id, double x, double y, signed char accel, double speed);
extern void m_turn(unsigned int id, double x, double y, signed char turn, double deg);
extern void send_version();
extern void send_nickname();
extern void send_pong(unsigned int time);
extern void send_ispawn();
extern void send_accel(int from);
extern void send_say(char *msg);
extern void send_cmd(char *cmd);
extern void send_turn();
extern void send_newbullet(double, double, double);
