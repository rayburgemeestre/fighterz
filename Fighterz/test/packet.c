#include "common.h"

void m_hi(unsigned int ver, int xmyid)
{
	printf("m_version: server version is 0x%x and my id is %d\n", ver, xmyid);
	myid = xmyid;
	printf("Sending back version & nick\n");
	send_version();
	send_nick("Test");
}

void m_time(unsigned int t)
{
	send_lag(t);
}


/** Packet parse routine.
 * This routine parses every packet and hands it over to the m_* functions.
 */
void dopacket(struct data *client, int xtype, unsigned short len, char *dta)
{
	printf("dopacket: type=%u (0x%.4x), len=%hu\n", xtype, xtype, len);

	switch(xtype)
	{
		case SMSG_HI:
		{
		unsigned int ver;
		unsigned int myid;
			if (!get_u32(&ver, &dta, &len))
				goto fatal;
			if (!get_u32(&myid, &dta, &len))
				goto fatal;
			return m_hi(ver, myid);
		}
		case SMSG_TIME:
		{
		unsigned int xtime;
			if (!get_u32(&xtime, &dta, &len))
				goto fatal;
			return m_time(xtime);
		}
	}


	/* This point should never be reached */
	return;
fatal:
	printf("Trouble :(((\n");
	SetDead(client);
	return;
}
