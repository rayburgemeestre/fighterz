#include "common.h"

static char xpacket[8194], *packet = xpacket + 2;

void send_packet(char *pktin, unsigned short len)
{
char *pkt = pktin - 2;
int n;
LINK current;

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
	put_u16(CMSG_VERSION, &p, &len);
	put_u32(CLIENT_VERSION, &p, &len);
	send_packet(packet, len);
}

void send_nick(char *nick)
{
char *p = packet;
unsigned short len = 0;
	put_u16(CMSG_NICK, &p, &len);
	put_str(nick, &p, &len);
	send_packet(packet, len);
}

void send_lag(unsigned int t)
{
char *p = packet;
unsigned short len = 0;
	put_u16(CMSG_LAG, &p, &len);
	put_u32(t, &p, &len);
	send_packet(packet, len);
}
