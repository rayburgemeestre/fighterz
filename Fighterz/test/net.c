#include "common.h"

/** Packet reading routine.
 * This function reads packets from the network,
 * put them together if they were fragmented,
 * and pass every (good) packet on to @a dopacket()
 * @param s socket (file descriptor)
 * @see dopacket()
 */
void readpacket(struct data *client, int s)
{
char buf[8192], tmpbuf[4096], *p = buf;
int r, len;
unsigned short plen;

	r = read(s, tmpbuf, sizeof(tmpbuf));
	if (r <= 0)
	{
		if (errno == EAGAIN)
			return;
		SetDead(client);
		printf("Read error: %m\n");
		return;
	}
	memcpy(buf, client->buffer, client->inlen);
	memcpy(buf + client->inlen, tmpbuf, r);
	len = r + client->inlen;
	
	/* Now parse it ^_^ */
	while(len > 0)
	{
		char *p2 = p;
		unsigned short xlen = (unsigned short)len;
		unsigned short xtype;
		
		if (!get_u16(&plen, &p2, &xlen))
			break; /* Insufficient data (short header) */
		if (xlen < plen)
			break; /* Insufficient data (short type/data) */
		if (!get_u16(&xtype, &p2, &xlen))
			break; /* Impossible */
		dopacket(client, xtype, plen-2, p2);
		if (IsDead(client))
			return;
		p += (4 + xlen);
		len -= (4 + xlen);
	}
	
	client->inlen = len;
	if (len > 0)
		memcpy(client->buffer, p, len);
	return;
}

int set_sockopts(int s)
{
int enabled = 1;
	if (fcntl(s, F_SETFL, O_NDELAY) < 0)
		return -1;
	if (setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char *)&enabled, sizeof(enabled)) < 0)
		return -1;
	return 1;
}
