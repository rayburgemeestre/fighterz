/** @file Protocol defines */

#ifndef TF_TIME_DEFINED
typedef signed long TIME;
#define TF_TIME_DEFINED
#endif

/* PROTOCOL DEFINES HERE... */

/* server->client msgs: SMSG_* */
#define SMSG_HI 0x0000
#define SMSG_MOTD 0x0001
#define SMSG_PING 0x0002
#define SMSG_LAG 0x0003
#define SMSG_NEWSHIP 0x0004

#define SMSG_SPAWNREADY 0x0200

#define SMSG_ACCEL 0x0400
#define SMSG_TURN 0x0401
#define SMSG_INVINCIBLE 0x0402
#define SMSG_HIT 0x0403

#define SMSG_SAY 0x0500
#define SMSG_TEAMSAY 0x0501

#define SMSG_NEWUSER 0x0600
#define SMSG_DELOBJ 0x0601
#define SMSG_KILL 0x0602
#define SMSG_NEWBULLET 0x0603
#define SMSG_NICK 0x0604
#define SMSG_SPAWN 0x0605
#define SMSG_QUIT 0x0606
#define SMSG_KICK 0x0607
#define SMSG_RESPAWN 0x0608

#define SMSG_NICKREPLY 0x0700

#define SMSG_FLAGCARR 0x8000 /* Flag Carrier */
#define SMSG_FLAGCAPT 0x8001 /* Flag Capturer */
#define SMSG_FLAGREST 0x8002 /* Flag restorer */

#define SMSG_CLEARFIELD 0x8100
#define SMSG_FIELDLINE 0x8101
#define SMSG_BLOCKINFO 0x8102
#define SMSG_BACKGROUND 0x8103
#define SMSG_FIELDEND 0x8104

/* client->server msgs: CMSG_* */
#define CMSG_VERSION 0x8000
#define CMSG_PASS 0x8001
#define CMSG_NICK 0x8002
#define CMSG_PONG 0x8003

#define CMSG_SPAWN 0x8200

#define CMSG_ACCEL 0x8300
#define CMSG_TURN 0x8301
#define CMSG_NEWBULLET 0x8302
#define CMSG_NEWSHIP 0x8303

#define CMSG_CMD 0x8500
#define CMSG_SAY 0x8501
#define CMSG_TEAMSAY 0x8502

#if defined(__cplusplus)
extern "C" {
#endif

/* Extern stuff */
extern int get_u32(unsigned int *val, char **p, unsigned short *len);
extern int get_s32(signed int *val, char **p, unsigned short *len);
extern int get_u16(unsigned short *val, char **p, unsigned short *len);
extern int get_s16(signed short *val, char **p, unsigned short *len);
extern int get_u8(unsigned char *val, char **p, unsigned short *len);
extern int get_s8(signed char *val, char **p, unsigned short *len);
extern int get_dbl(double *val, char **p, unsigned short *len);
extern int get_str(char *s, char **p, unsigned short *len, size_t max);
extern int get_time(TIME *val, char **p, unsigned short *len);
extern void put_u32(unsigned int val, char **p, unsigned short *len);
extern void put_s32(signed int val, char **p, unsigned short *len);
extern void put_u16(unsigned short val, char **p, unsigned short *len);
extern void put_s16(signed short val, char **p, unsigned short *len);
extern void put_u8(unsigned char val, char **p, unsigned short *len);
extern void put_s8(signed char val, char **p, unsigned short *len);
extern void put_str(char *s, char **p, unsigned short *len);
extern void put_dbl(double val, char **p, unsigned short *len);
extern void put_length(char *header, unsigned short len);
extern void put_time(TIME val, char **p, unsigned short *len);
#if defined(__cplusplus)
}
#endif