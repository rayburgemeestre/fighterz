#include "common.h"

static char xpacket[8194], *packet = xpacket + 2;

void send_packet(char *pktin, unsigned short len)
{
	char *pkt = pktin - 2;
	int n;

    /* Build length field */
    put_length(pkt, len);

    n = send(theSocket, pkt, len + 2, 0);
    if (n != len+2)
    {
            if (ERRNO == P_EAGAIN)
            {
                    printf("SendQ exceeded!\n");
                    /* TODO: handle :PPP */
                    CLOSE_SOCK(theSocket);
                    return;
            }
            CLOSE_SOCK(theSocket);
            /* TODO: handle :PPP */
            printf("Send error: %m\n");
            return;
    }
}


void send_version()
{
	char *p = packet;
	unsigned short len = 0;
	verbose("CMSG_VERSION: %d", CLIENT_VERSION);

	put_u16(CMSG_VERSION, &p, &len);
	put_u32(CLIENT_VERSION, &p, &len);
	send_packet(packet, len);
}

void send_ispawn()
{
	char *p = packet;
	unsigned short len = 0;
	requested_spawn = 1;
	verbose("CMSG_SPAWN -- requested spawn");
	put_u16(CMSG_SPAWN, &p, &len);	
	send_packet(packet, len);
}

void send_nickname()
{
	char *p = packet;
	unsigned short len = 0;	

	if (strlen(nickname) < 1)
		sprintf(nickname, "Unit%d", our_id);

	verbose("CMSG_NICK: %s", nickname);
	put_u16(CMSG_NICK, &p, &len);
	put_str(nickname, &p, &len);
	send_packet(packet, len);
}

void send_pong(unsigned int time)
{
	char *p = packet;
	unsigned short len = 0;
	verbose("CMSG_PONG: %d", time);
	
	put_u16(CMSG_PONG, &p, &len);
	put_u32(time, &p, &len);
	send_packet(packet, len);
}

void send_accel(int from)
{
	char *p = packet;
	unsigned short len = 0;
	verbose("CMSG_ACCEL(%d): %d", from, time);
	
	put_u16(CMSG_ACCEL, &p, &len);
	put_dbl(our_node->x, &p, &len);
	put_dbl(our_node->y, &p, &len);
	put_s8(our_node->velocity, &p, &len);
	put_dbl(our_node->speed, &p, &len);

	send_packet(packet, len);
}
//sockwrite("3 %.2f %.2f %.2f %d\n", our_node->x, our_node->y, 
	//our_node->deg, our_node->turn);
void send_turn()
{
	char *p = packet;
	unsigned short len = 0;
	verbose("CMSG_TURN(%d): %d", (signed int)our_node->turn, (signed int)our_node->deg);

// TURN <x (dbl)> <y (dbl)> <turn (s8)> <deg (dbl)>

	put_u16(CMSG_TURN, &p, &len);
	put_dbl(our_node->x, &p, &len);
	put_dbl(our_node->y, &p, &len);
	put_s8(our_node->turn, &p, &len);
	put_dbl(our_node->deg, &p, &len);	

	send_packet(packet, len);
}

void send_newbullet(double x, double y, double deg)
{
char *p = packet;
unsigned short len = 0;
	verbose("CMSG_NEWBULLET");

	put_u16(CMSG_NEWBULLET, &p, &len);
	put_dbl(x, &p, &len);
	put_dbl(y, &p, &len);
	put_dbl(deg, &p, &len);

	send_packet(packet, len);
}
