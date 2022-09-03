#include "common.h"
// refactored 12 dec 2004

#include <sys/types.h>
#include <sys/socket.h>

// functions:
//--
// void send_packet(char *pktin, unsigned short len);
// void send_version();
// void send_ispawn();
// void send_nickname();
// void send_pong(unsigned int time);
// void send_accel(int from);
// void send_say(char *msg)
// void send_cmd(char *cmd);
// void send_turn();
// void send_newbullet(double x, double y, double deg);
// void send_newship();

// variables:
//--
static char xpacket[8194], *packet = xpacket + 2;

void send_packet(char *pktin, unsigned short len) {
  char *pkt = pktin - 2;
  int n;

  /* Build length field */
  put_length(pkt, len);

  n = send(sock_to_serv, pkt, len + 2, 0);
  if (n != len + 2) {
    if (ERRNO == P_EAGAIN) {
      printf("SendQ exceeded!\n");
      /* TODO: handle :PPP */
      CLOSE_SOCK(sock_to_serv);
      return;
    }
    CLOSE_SOCK(sock_to_serv);
    /* TODO: handle :PPP */
    printf("Send error: %m\n");
    return;
  }
}

void send_version() {
  char *p = packet;
  unsigned short len = 0;
  put_u16(CMSG_VERSION, &p, &len);
  put_u32(CLIENT_VERSION, &p, &len);
  send_packet(packet, len);
}

void send_ispawn() {
  char *p = packet;
  unsigned short len = 0;
  our_spawnrequested = 1;
  put_u16(CMSG_SPAWN, &p, &len);
  send_packet(packet, len);
}

void send_nickname() {
  char *p = packet;
  unsigned short len = 0;
  if (strlen(our_nickname) < 1) sprintf(our_nickname, "Unit%d", our_id);
  put_u16(CMSG_NICK, &p, &len);
  put_str(our_nickname, &p, &len);
  send_packet(packet, len);
}

void send_pong(unsigned int time) {
  char *p = packet;
  unsigned short len = 0;
  put_u16(CMSG_PONG, &p, &len);
  put_u32(time, &p, &len);
  send_packet(packet, len);
}

void send_accel(int from) {
  char *p = packet;
  unsigned short len = 0;
  if (!our_node) return;
  put_u16(CMSG_ACCEL, &p, &len);
  put_dbl(our_node->x, &p, &len);
  put_dbl(our_node->y, &p, &len);
  put_s8(our_node->velocity, &p, &len);
  put_dbl(our_node->speed, &p, &len);
  send_packet(packet, len);
}

void send_say(char *msg) {
  char *p = packet;
  unsigned short len = 0;
  if (strlen(msg) > (unsigned)CON_LINE_LENGTH) *(msg + CON_LINE_LENGTH - 1) = '\0';
  put_u16(CMSG_SAY, &p, &len);
  put_str(msg, &p, &len);
  send_packet(packet, len);
}

void send_cmd(char *cmd) {
  char *p = packet;
  unsigned short len = 0;
  if (strlen(cmd) > (unsigned)CON_LINE_LENGTH) *(msg + CON_LINE_LENGTH - 1) = '\0';
  put_u16(CMSG_CMD, &p, &len);
  put_str(cmd, &p, &len);
  send_packet(packet, len);
}

void send_turn() {
  char *p = packet;
  unsigned short len = 0;
  put_u16(CMSG_TURN, &p, &len);
  put_dbl(our_node->x, &p, &len);
  put_dbl(our_node->y, &p, &len);
  put_s8(our_node->turn, &p, &len);
  put_dbl(our_node->deg, &p, &len);
  send_packet(packet, len);
}

void send_newbullet(double x, double y, double deg) {
  char *p = packet;
  unsigned short len = 0;
  put_u16(CMSG_NEWBULLET, &p, &len);
  put_dbl(x, &p, &len);
  put_dbl(y, &p, &len);
  put_dbl(deg, &p, &len);
  send_packet(packet, len);
}

void send_newship() {
  char *p = packet;
  unsigned short len = 0;
  put_u16(CMSG_NEWSHIP, &p, &len);
  put_s32(our_node->shiptype, &p, &len);
  send_packet(packet, len);
}
