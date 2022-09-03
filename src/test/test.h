extern int theSocket;
extern int myid;

#define CLIENT_VERSION 0

extern void send_version();
extern void send_nick(char *nick);
extern void dopacket(struct data *client, int xtype, unsigned short len, char *dta);
extern void send_lag(unsigned int t);
