/** @file proto.c
 * Protocol helper functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "proto.h"

#ifndef _WIN32
 #include <unistd.h>
#endif

/** @defgroup extractpacket Routines to extract variables from packets
 */

/** Extract a unsigned integer (32bit) from a packet.
 * The result is stored in @a val, @a p is updated to point
 * after the value just read, @a len will also be updated (decreased).
 * @ingroup extractpacket
 * @param val Reference to var in which the result is stored
 * @param p Reference to packet pointer
 * @param len Reference to packet length
 * @retval 0 Failed - insufficient room
 * @retval 1 Succeeded
 * @par Example:
 * @code
 * if (!get_XXX(&v, &p, &len))
 *     goto fatal;
 * @endcode
 */
int get_u32(unsigned int *val, char **p, unsigned short *len)
{
	if (*len < 4)
		return 0;
	memcpy(val, *p, 4);
	*p += 4;
	*len -= 4;
	return 1;
}

/** Extract a signed integer (32bit) from a packet.
 * @ingroup extractpacket
 * @copydoc get_u32()
 */
int get_s32(signed int *val, char **p, unsigned short *len)
{
	if (*len < 4)
		return 0;
	memcpy(val, *p, 4);
	*p += 4;
	*len -= 4;
	return 1;
}

/** Extract a double (packed as *100 signed integer) from a packet.
 * @ingroup extractpacket
 * @copydoc get_u32()
 */
int get_dbl(double *val, char **p, unsigned short *len)
{
int tmp;
	if (*len < 4)
		return 0;
	memcpy(&tmp, *p, 4);
	*p += 4;
	*len -= 4;
	*val = ((double)tmp) / 100.0;
	return 1;
}

/** Extract a unsigned short (16bit) from a packet.
 * @ingroup extractpacket
 * @copydoc get_u32()
 */
int get_u16(unsigned short *val, char **p, unsigned short *len)
{
	if (*len < 2)
		return 0;
	memcpy(val, *p, 2);
	*p += 2;
	*len -= 2;
	return 1;
}

/** Extract a signed short (16bit) from a packet.
 * @ingroup extractpacket
 * @copydoc get_u32()
 */
int get_s16(signed short *val, char **p, unsigned short *len)
{
	if (*len < 2)
		return 0;
	memcpy(val, *p, 2);
	*p += 2;
	*len -= 2;
	return 1;
}

/** Extract a unsigned char (8bit) from a packet.
 * @ingroup extractpacket
 * @copydoc get_u32()
 */
int get_u8(unsigned char *val, char **p, unsigned short *len)
{
	if (!*len)
		return 0;
	*val = **p;
	*p += 1;
	*len -= 1;
	return 1;
}

/** Extract a signed char (8bit) from a packet.
 * @ingroup extractpacket
 * @copydoc get_u32()
 */
int get_s8(signed char *val, char **p, unsigned short *len)
{
	if (!*len)
		return 0;
	*val = **p;
	*p += 1;
	*len -= 1;
	return 1;
}

/** Extract a string from a packet.
 * The result is stored in @a s, @a p is updated to point
 * after the value just read, @a len will also be updated (decreased).
 * @ingroup extractpacket
 * @param s Pointer to destination string
 * @param p Reference to packet pointer
 * @param len Reference to packet length
 * @param max Max size of string
 * @retval 0 Failed - insufficient room
 * @retval len Succeeded - length of string
 * @par Example:
 * @code
 * if (!get_str(s, &p, &len, sizeof(s)))
 *     goto fatal;
 * @endcode
 */
int get_str(char *s, char **p, unsigned short *len, size_t max)
{
register size_t i;

	if (!*len)
		return 0;
	for (i=0; (i < max - 1) && (*len > 0); i++)
	{
		*s = **p;
		*len -= 1;
		if (**p == '\0')
		{
			*p += 1;
			return (int)(i + 1);
		}
		*p += 1;
		s++;
	}
	*s = '\0';
	return 0;
}

/** Extract a TIME variable (currently 32bit signed long) from a packet.
 * @ingroup extractpacket
 * @copydoc get_u32()
 */
int get_time(TIME *val, char **p, unsigned short *len)
{
	if (*len < (unsigned short)sizeof(TIME))
		return 0;
	memcpy(val, *p, sizeof(TIME));
	*p += sizeof(TIME);
	*len -= sizeof(TIME);
	return 1;
}

/** @defgroup buildpacket Routines to build packets
 */

/** Puts an unsigned integer (32bit) in a packet.
 * val is stored in @a p, @a p is updated to point after the
 * value just written, @a len will also be updated (increased).
 * @ingroup buildpacket
 * @param val Value to be stored
 * @param p Reference to packet pointer
 * @param len Reference to packet length
 * @par Example:
 * @code
 * put_XXX(val, &p, &len);
 * @endcode
 */
void put_u32(unsigned int val, char **p, unsigned short *len)
{
	memcpy(*p, &val, 4);
	*p += 4;
	*len += 4;
}

/** Puts a signed integer (32bit) in a packet.
 * @ingroup buildpacket
 * @copydoc put_u32()
 */
void put_s32(signed int val, char **p, unsigned short *len)
{
	memcpy(*p, &val, 4);
	*p += 4;
	*len += 4;
}

/** Puts a double (packed as *100 signed integer) in a packet.
 * @ingroup buildpacket
 * @copydoc put_u32()
 */
void put_dbl(double val, char **p, unsigned short *len)
{
int tmp;

	tmp = (int)(val * 100.0);
	memcpy(*p, &tmp, 4);
	*p += 4;
	*len += 4;
}

/** Puts an unsigned short (16bit) in a packet.
 * @ingroup buildpacket
 * @copydoc put_u32()
 */
void put_u16(unsigned short val, char **p, unsigned short *len)
{
	memcpy(*p, &val, 2);
	*p += 2;
	*len += 2;
}

/** Puts a signed short (16bit) in a packet.
 * @ingroup buildpacket
 * @copydoc put_u32()
 */
void put_s16(signed short val, char **p, unsigned short *len)
{
	memcpy(*p, &val, 2);
	*p += 2;
	*len += 2;
}

/** Puts an unsigned char (8bit) in a packet.
 * @ingroup buildpacket
 * @copydoc put_u32()
 */
void put_u8(unsigned char val, char **p, unsigned short *len)
{
	**p = val;
	*p += 1;
	*len += 1;
}

/** Puts a signed char (8bit) in a packet.
 * @ingroup buildpacket
 * @copydoc put_u32()
 */
void put_s8(signed char val, char **p, unsigned short *len)
{
	**p = val;
	*p += 1;
	*len += 1;
}

/** Puts a string in a packet.
 * @a s is stored in @a p, @a p is updated to point after the
 * string just written, @a len is also updated (increased).
 * @ingroup buildpacket
 * @param s String to be stored
 * @param p Reference to packet pointer
 * @param len Reference to packet length
 * @par Example:
 * @code
 * put_str("This is a test", &p, &len);
 * @endcode
 * @attention Be carefull with buffer overflows, there's no size checking in this routine!
 */
void put_str(char *s, char **p, unsigned short *len)
{
register size_t l;
	l = strlen(s) + 1;
	memcpy(*p, s, l);
	*p += l;
	*len += l;
}

/** Fills in the length field in a packet.
 * @ingroup buildpacket
 * @param header Packet pointer
 * @param len Length of packet
 */
void put_length(char *header, unsigned short len)
{
	memcpy(header, &len, 2);
}

/** Puts a TIME value (currently 32bit signed long) in a packet.
 * @ingroup buildpacket
 * @copydoc put_u32()
 */
void put_time(TIME val, char **p, unsigned short *len)
{
	memcpy(*p, &val, sizeof(TIME));
	*p += sizeof(TIME);
	*len += sizeof(TIME);
}
