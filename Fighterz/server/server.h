extern void die(char *s);
extern void lps_proc();
extern void settime();
extern void parse(int sock_id, char *stuff, int nbytes);
extern void parse2(int sock_id, char *stuff);
extern void lag_check_proc();
extern double current_lag(ID sockid);
extern int sockwrite(int sockfd, char *pattern, ...);
extern int sockwrite_all(char *pattern, ...);
extern int sockwrite_notall(int except_id, char *pattern, ...);
extern struct data *add_player(ID sock_fd);
extern struct data *add_bot();
extern double getangle(double x1, double y1, double x2, double y2);
extern void del_player(ID sock_fd);
extern void del_bullets(ID sock_fd);
extern void printplayerlist();
extern int sendall(int s, char *buf, int *len);
extern void sendulist(struct data *client);
extern void sendulist2(unsigned int id);
extern void flyto(int id, double x, double y);
extern void moveships();
extern void moveship(unsigned int id2, TIME t2);
extern double dabs(double x);
extern void collidecheck();
extern int collidecheck2(struct data *ptr, int nobounce);
extern double futureX(struct data *ptr);
extern double futureY(struct data *ptr);
extern double fbX(unsigned int id2);
extern double fbY(unsigned int id2);
extern void loadmap();
extern void fillmap();
extern void findpath(struct data *ptr, double x, double y);
extern int printf_(char *pattern, ...);
extern int valid_target(double x1, double y1, double x2, double y2, double precision);
extern double vtX(double x, double deg);
extern double vtY(double y, double deg);
#if DEBUG2 == 1
extern void init();
extern void init_screen();
extern void drawgraphics();
extern void set_map_coords();

extern void fps_proc();
extern void drawmap();
extern void drawconsole();
extern void addtext(char *pattern, ...);
extern void drawships();
extern void printulist();
extern void printul(char *nick, long int one);
extern void parse_input();
#endif
extern void initialize_vars();
extern void initialize_debug();

extern int bs_collidecheck(struct data *ptr);
extern void notify_of_newuser(struct data *client, struct data *except, struct data *current);

extern void drawmap_features();
extern struct data *add_bullet(struct data *_owner, double _x, double _y, double _deg, TIME _time);
extern void del_bullet(struct data *ptr);
extern int collidecheck2b(struct data *ptr);
extern void movebullets();
extern int movebullet(struct data *ptr);
extern void drawbullets();

#if WINDOWS == 1
/* Fuck it then i'll make it myself ;)  */
//struct timeval {
	//long tv_sec;        /* seconds */
	//long tv_usec;  /* microseconds */
//};
extern struct timeval timevl;
#endif

/* FIND PATH */

#ifndef __SP_H__
#define __SP_H__

//#define MAX_R		30
//#define MAX_C		30
//#define INFINITY	(1 << 14) **moved**

// typedef int MAP[MAX_R][MAX_C];

//int  map_read(char * fname);
extern void map_read();
extern int map_draw_path(void);
extern void map_draw_path2(void);
extern void map_create_path(struct data *ptr);
extern void build_path(void);
extern void limit_path(void);
#endif

extern struct data *getplayer_byid(ID);
extern void dopacket(struct data *client, unsigned short xtype, unsigned short len, char *dta);
extern void readpacket(struct data *client, int s);
extern void kick_player(struct data *client, char *reason);
extern void softkick_player(struct data *client, char *reason);
extern void quit_player(struct data *client, char *reason);

extern void del_tha_player(struct data *client);
extern void notify_of_respawn(struct data *client, struct data *except, struct data *current);
extern struct data *getplayer_bynick(char *nick);

extern void cleanstr(char *str);

extern int set_sockopts(int s);

/* send.c externs */
extern void send_hi(struct data *client);
extern void send_servertime(struct data *client);
extern void send_clearfield(struct data *client);
extern void send_fieldline(struct data *client, unsigned int index, char *line);
extern void send_background(struct data *client, int df_id, 
	int pos_x, int pos_y, char *datfile);
extern void send_fieldend(struct data *client);
extern void send_blockinfo(struct data *client, int w, int h, int size);
extern void send_lag(struct data *client, int diff);
extern void send_accel(struct data *client, struct data *except,
    ID id, double x, double y, signed char accel, double speed);
extern void send_turn(struct data *client, struct data *except,
    ID id, double x, double y, signed char turn, double deg);
extern void send_invincible(struct data *client, struct data *except,
    ID id, unsigned char yesno, unsigned int t);
extern void send_hit(struct data *client, struct data *except,
    ID victimid, ID evilid, unsigned char newpower);
extern void send_newuser(struct data *client, struct data *except,
    ID id, double x, double y, double deg, signed char accel, 
	unsigned int alive, signed short frags, unsigned int pending_moves,
    signed char turn, unsigned char type, double speed,
    char *nick);
extern void send_delobj(struct data *client, struct data *except, ID id);
extern void send_kill(struct data *client, struct data *except,
    ID victimid, ID evilid, char *killstr);
extern void send_newbullet(struct data *client, struct data *except,
    ID id, ID ownerid, double x, double y, double deg);
extern void send_nick(struct data *client, struct data *except, ID id, char *newnick);
extern void send_spawn(struct data *client, struct data *except,
    ID id, double x, double y, double deg,
    signed char accel, unsigned int alive, signed short frags,
    signed char turn, unsigned char type, double speed);
extern void send_quit(struct data *client, struct data *except, ID id, char *msg);
extern void send_kick(struct data *client, struct data *except, ID id, char *msg);
extern void send_nickreply(struct data *client, unsigned char goodorbad, char *txt);
extern void send_spawnready(struct data *client);
extern void send_say(struct data *client, struct data *except, ID id, char *msg);
extern void verbose(char *pattern, ...);
