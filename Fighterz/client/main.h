/********************************************
*  FUNCTION PROTOTYPES
****************************************************/

extern void draw_ships();
extern void draw_ship();
extern void draw_bullets();
extern void draw_fps();
extern void draw_explosions();
extern void draw_map();
extern void draw_radar();
extern void draw_talk_box();
extern void draw_console();

extern void move_ships();
extern void move_ship(struct data *client);
extern void move_bullets();
extern int move_bullet(struct data *ptr, unsigned long t2);

extern struct data  *add_player(int _id, int _x, int _y, int _deg, int _move, int _turn,  
					   int _velocity, int _alive, int _kills, int _killavg, 
					   int _bot, double _speed, int _dead, char *_nick);
extern struct data  *add_bullet(struct data *owner, int x, int y, double deg, unsigned long time);
extern struct data2 *add_explosion(double x, double y, int maxrad, int speed, int color);
extern void del_player(int sock_fd);
extern void del_bullet(struct data *ptr);
extern void del_explosion(struct data2 *ptr);




extern void printff(char *pattern, ...);
extern void printff_direct(char *pattern, ...);
extern void terminate();
extern void delay(int secs);

extern void set_map_coords();

extern int mainloop();
extern void center_window();
extern void fps_proc();
extern void time_proc();
extern void init();
extern int start();
extern void init_screen();
extern void printulist();
extern void printul(struct data *node);
extern int connect_socket();
extern void sockread();
extern int sockwrite(char *pattern, ...);
extern void large_text(char *pattern, ...);
extern void large_text_draw();
extern void lag_proc(char *lag_char, char *servertime_char);
extern double current_lag();


extern void config(int ignore);
extern void debug();
extern void addtext(char *pattern, ...);
extern void collidecheck();
extern int collidecheck2(unsigned int id, int variation, int nobounce);
extern double futureX(struct data *ptr, double angle, double speed);
extern double futureY(struct data *ptr, double angle, double speed);
extern double fbX(unsigned int id2);
extern double fbY(unsigned int id2);



extern void process_bounce(struct data *ptr, int *retval, double speed);
extern int collidecheck2b(struct data *ptr);
extern int bs_collidecheck(struct data *ptr);
extern double getdistance(double x1, double y1, double x2, double y2);
extern double getangle(double x1, double y1, double x2, double y2);
extern int getoptions();
extern void getsettings();
extern double dabs(double x);


extern void initialize_vars();
extern void die(char *s);
extern void parse_input();
extern void show_graphics();
extern struct data *getplayer_byid(unsigned int id);
extern void play_rand_bg_music();

//proto.cpp
extern void dopacket(int xtype, unsigned short len, char *dta);
extern void verbose(char *pattern, ...);
extern void m_hi(unsigned int sver, unsigned int oid);
extern void m_shiptype(unsigned int id, int newshiptype);
extern void m_ping(unsigned int servertime);
extern void m_lag(double diff);
extern void m_nickreply(unsigned char acceptance, char *msg);
extern void m_flagcarrier(unsigned int carrier_id, unsigned int code);
extern void m_clearfield();
extern void m_fieldline(unsigned int index, char *fieldline);
extern void m_blockinfo(int w, int h, int size);
extern void m_background(int df_id, int pos_x, int pos_y, char *datfile);
extern void m_fieldend();
extern void m_spawnready();
extern void m_kill(unsigned int victimid, unsigned int evilid, char *killstr);
extern void m_nick(unsigned int id, char *nick);
extern void m_say(unsigned int id, char *msg);
extern void m_newbullet(unsigned int id, unsigned int ownerid, double x, double y, double deg);
extern void m_kick( unsigned int id, char *reason );
extern void m_quit(unsigned int id, char *quit_msg);
extern void m_spawn(unsigned int id, double x, double y, double deg, signed char accel, 
					unsigned int alive, short frags, signed char turn, unsigned char type, 
					double speed);
extern void m_respawn(unsigned int id);
extern void m_newuser(int id, double x, double y, double deg,
					  signed char accel, unsigned int alive, signed short frags,
					  signed char team, signed char turn, unsigned char type, 
					  double speed, int shiptype, char *nick);
extern void m_hit();
extern void m_accel(unsigned int id, double x, double y, signed char accel, double speed);
extern void m_invincible(unsigned int id, unsigned char yesno, unsigned int t);
extern void m_turn(unsigned int id, double x, double y, signed char turn, double deg);

//send.cpp
extern void send_version();
extern void send_nickname();
extern void send_pong(unsigned int time);
extern void send_ispawn();
extern void send_accel(int from);
extern void send_say(char *msg);
extern void send_cmd(char *cmd);
extern void send_turn();
extern void send_newbullet(double, double, double);
extern void send_newship();
