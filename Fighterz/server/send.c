#include "common.h"

static char xpacket[8194], *packet = xpacket + 2;

/*
 * NOTE: You can safely ignore this return value. Maybe this should be type void.
 */
void send_packet(struct data *client, struct data *except, char *pktin, unsigned short len)
{
char *pkt = pktin - 2;
int n;
LINK current;

	/* Build length field */
	put_length(pkt, len);

	if (client)
	{
		/* Single client */
		if (IsDead(client))
		{
			/* Is this a debug message or a real problem? */
			printf("Trying to write to already-dead client: %p [id %u] ['%s']\n",
				client, (unsigned int)client->id, client->nick);
			return;
		}
		printf("send_packet -> %u xtype: 0x%.2X%.2X\n",
			(unsigned int)client->id, (unsigned char)*pktin, (unsigned char)*(pktin+1));
		n = send(client->id, pkt, len + 2, MSG_NOSIGNAL);
		if (n != len+2)
		{
			SetDeadSend(client);
			if (ERRNO == P_EAGAIN)
				client->errmsg = strdup("SendQ exceeded");
			else
				client->errmsg = strdup("Send error"); /* TODO: improve */
			return;
		}
	} else {
		/* Broadcast! */
		for (current = head; current; current = current->next)
			if (IsPlayer(current) && (current != except) && !IsDead(current))
			{
				printf("send_packet BC! -> %u xtype: 0x%.2X%.2X\n",
					(unsigned int)current->id, (unsigned char)*pktin, (unsigned char)*(pktin+1));
				n = send(current->id, pkt, len + 2, MSG_NOSIGNAL);
				if (n != len+2)
				{
					SetDeadSend(current);
					if (ERRNO == P_EAGAIN)
						current->errmsg = strdup("SendQ exceeded");
					else
						current->errmsg = strdup("Send error"); /* TODO: improve */
				}
			}
	}
}


/* Insert send functions here :P */
void send_hi(struct data *client)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_HI: %d %d", SERVER_VERSION, client->id);

	put_u16(SMSG_HI, &p, &len);
	put_u32(SERVER_VERSION, &p, &len);
	put_u32(client->id, &p, &len);
	send_packet(client, NULL, packet, len);
}
void send_newship(struct data *client)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_NEWSHIP: <args>");

	put_u16(SMSG_NEWSHIP, &p, &len);
	
	put_u32(client->id, &p, &len);
	put_s32(client->shiptype, &p, &len);

	send_packet(NULL, NULL, packet, len);
}
void send_servertime(struct data *client)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_PING: %d", servertime);

	put_u16(SMSG_PING, &p, &len);
	put_time(servertime, &p, &len);
	send_packet(client, NULL, packet, len);
}

// code 1=red flag 2=blue flag.
void send_flagcarrier(ID carrier_id, unsigned int code)
{
char *p = packet;
unsigned short len = 0;

	put_u16(SMSG_FLAGCARR, &p, &len);
	put_u32(carrier_id, &p, &len);
	put_u32(code, &p, &len);

	send_packet(NULL, NULL, packet, len);
}

void send_clearfield(struct data *client)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_CLEARFIELD");

	put_u16(SMSG_CLEARFIELD, &p, &len);
	send_packet(client, NULL, packet, len);
}

void send_blockinfo(struct data *client, int w, int h, int size)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_BLOCKINFO");

	put_u16(SMSG_BLOCKINFO, &p, &len);
	
	put_s32(w, &p, &len);
	put_s32(h, &p, &len);
	put_s32(size, &p, &len);

	send_packet(client, NULL, packet, len);
}
void send_fieldend(struct data *client)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_FIELDEND");

	put_u16(SMSG_FIELDEND, &p, &len);
	send_packet(client, NULL, packet, len);
}
void send_background(struct data *client, int df_id, 
	int pos_x, int pos_y, char *datfile)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_BACKGROUND");

	put_u16(SMSG_BACKGROUND, &p, &len);
	put_s32(df_id, &p, &len);
	put_s32(pos_x, &p, &len);
	put_s32(pos_y, &p, &len);
	put_str(datfile, &p, &len);

	send_packet(client, NULL, packet, len);
}

void send_fieldline(struct data *client, unsigned int index, char *line)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_FIELDLINE: %s", line);

	put_u16(SMSG_FIELDLINE, &p, &len);
	put_u32(index, &p, &len);
	put_str(line, &p, &len);
	send_packet(client, NULL, packet, len);
}

void send_lag(struct data *client, int diff)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_LAG: %d", diff);
	
	put_u16(SMSG_LAG, &p, &len);
	put_s32(diff, &p, &len);
	send_packet(client, NULL, packet, len);
}

void send_spawnready(struct data *client)
{
char *p = packet;
unsigned short len = 0;
	verbose("SMSG_SPAWNREADY");
	put_u16(SMSG_SPAWNREADY, &p, &len);
	send_packet(client, NULL, packet, len);
}

void send_accel(struct data *client, struct data *except,
	ID id, double x, double y, signed char accel, double speed)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_ACCEL <args>");
	put_u16(SMSG_ACCEL, &p, &len);
	put_u32(id, &p, &len);
	put_dbl(x, &p, &len);
	put_dbl(y, &p, &len);
	put_s8(accel, &p, &len);
	put_dbl(speed, &p, &len);
	send_packet(client, except, packet, len);
}

void send_turn(struct data *client, struct data *except,
	ID id, double x, double y, signed char turn, double deg)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_TURN %d %2.2f %2.2f %d %2.2f", id, x, y, (int)turn, deg);

	put_u16(SMSG_TURN, &p, &len);
	put_u32(id, &p, &len);
	put_dbl(x, &p, &len);
	put_dbl(y, &p, &len);
	put_s8(turn, &p, &len);
	put_dbl(deg, &p, &len);
	send_packet(client, except, packet, len);
}

void send_invincible(struct data *client, struct data *except,
	ID id, unsigned char yesno, unsigned int t)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_INVINCIBLE <args>");
	put_u16(SMSG_INVINCIBLE, &p, &len);
	put_u32(id, &p, &len);
	put_u8(yesno, &p, &len);
	put_u32(t, &p, &len);
	send_packet(client, except, packet, len);
}

void send_hit(struct data *client, struct data *except,
	ID victimid, ID evilid, unsigned char newpower)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_HIT <args>");
	put_u16(SMSG_HIT, &p, &len);
	put_u32(victimid, &p, &len);
	put_u32(evilid, &p, &len);
	put_u8(newpower, &p, &len);
	send_packet(client, except, packet, len);
}

void send_newuser(struct data *client, struct data *except,
	ID id, double x, double y, double deg,	signed char accel, 
	unsigned int alive, signed short frags, unsigned int pending_moves,
	signed char turn, unsigned char type, double speed, int shiptype,
	char *nick)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_NEWUSER <args> nick: %s", nick);

	put_u16(SMSG_NEWUSER, &p, &len);
	put_u32(id, &p, &len);
	put_dbl(x, &p, &len);
	put_dbl(y, &p, &len);
	put_dbl(deg, &p, &len);
	put_s8(accel, &p, &len);
	put_u32(alive, &p, &len);
	put_s16(frags, &p, &len);
	put_u32(pending_moves, &p, &len);
	put_s8(turn, &p, &len);
	put_u8(type, &p, &len);
	put_dbl(speed, &p, &len);
	put_s32(shiptype, &p, &len);
	put_str(nick, &p, &len);
	send_packet(client, except, packet, len);
}

void send_delobj(struct data *client, struct data *except, ID id)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_DELOBJ <args>");

	put_u16(SMSG_DELOBJ, &p, &len);
	put_u32(id, &p, &len);
	send_packet(client, except, packet, len);
}

void send_kill(struct data *client, struct data *except,
	ID victimid, ID evilid, char *killstr)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_KILL <args>");
	put_u16(SMSG_KILL, &p, &len);
	put_u32(victimid, &p, &len);
	put_u32(evilid, &p, &len);
	put_str(killstr, &p, &len);
	send_packet(client, except, packet, len);
}

void send_newbullet(struct data *client, struct data *except,
	ID id, ID ownerid, double x, double y, double deg)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_NEWBULLET <args> [bullet-id:%u]", id);
	put_u16(SMSG_NEWBULLET, &p, &len);
	put_u32(id, &p, &len);
	put_u32(ownerid, &p, &len);
	put_dbl(x, &p, &len);
	put_dbl(y, &p, &len);
	put_dbl(deg, &p, &len);
	send_packet(client, except, packet, len);
}

void send_nick(struct data *client, struct data *except, ID id, char *newnick)
{
char *p = packet;
unsigned short len = 0;
	// verbose("SMSG_NICK <args>");
	put_u16(SMSG_NICK, &p, &len);
	put_u32(id, &p, &len);
	put_str(newnick, &p, &len);
	send_packet(client, except, packet, len);
}

void send_spawn(struct data *client, struct data *except,
	ID id, double x, double y, double deg,
	signed char accel, unsigned int alive, signed short frags,
	signed char turn, unsigned char type, double speed)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_SPAWN <args>");
	put_u16(SMSG_SPAWN, &p, &len);
	put_u32(id, &p, &len);
	put_dbl(x, &p, &len);
	put_dbl(y, &p, &len);
	put_dbl(deg, &p, &len);
	put_s8(accel, &p, &len);
	put_u32(alive, &p, &len);
	put_s16(frags, &p, &len);
	put_s8(turn, &p, &len);
	put_u8(type, &p, &len);
	put_dbl(speed, &p, &len);
	send_packet(client, except, packet, len);
}

void send_quit(struct data *client, struct data *except, ID id, char *msg)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_QUIT <args>");

	put_u16(SMSG_QUIT, &p, &len);
	put_u32(id, &p, &len);
	put_str(msg, &p, &len);
	send_packet(client, except, packet, len);
}

void send_kick(struct data *client, struct data *except, ID id, char *msg)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_KICK %u %s", id, msg);
	put_u16(SMSG_KICK, &p, &len);
	put_u32(id, &p, &len);
	put_str(msg, &p, &len);
	send_packet(client, except, packet, len);
}

void send_nickreply(struct data *client, unsigned char goodorbad, char *txt)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_NICKREPLY: %d %s", (int)goodorbad, txt);
	put_u16(SMSG_NICKREPLY, &p, &len);
	put_u8(goodorbad, &p, &len);
	put_str(txt, &p, &len);
	send_packet(client, NULL, packet, len);
}

void send_say(struct data *client, struct data *except, ID id, char *msg)
{
char *p = packet;
unsigned short len = 0;
	//verbose("SMSG_SAY <args>");
	put_u16(SMSG_SAY, &p, &len);
	put_u32(id, &p, &len);
	put_str(msg, &p, &len);
	send_packet(client, except, packet, len);
}
