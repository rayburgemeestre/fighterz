/********************************************
*  PREDEFINES
****************************************************/

#define PORT 8099 /*    port we're listening on */

#ifdef _WIN32
 #define WINDOWS 1	/* Use winsock */
#else
 #define WINDOWS 0
#endif
#define DEBUG 0   /*    log ship positions etc to debug.log */
#define DEBUG2 1  /*    1= SHOW FIELD(console msgs are logged to a file)
					    0= CONSOLE ONLY

						if chosen 1: replace /subsystem:console with 
							/subsystem:windows in VC++'s project options
						if chosen 0: visa versa.
				   */
#ifdef _WIN32
  #if (DEBUG2 == 1)
      #pragma comment( linker, "/subsystem:\"windows\"" )
  #else
      #pragma comment( linker, "/subsystem:\"console\"" )
  #endif
#else
  #if (DEBUG2 == 1)
	  #define DEBUG2 0
  #endif
#endif

#define PI 3.14159265358979
#define DEF_BLOCKSIZE 20
#define DEBUG_FILE "output.txt"
#define IGNORE_LAG 0
#define PATH_MAX_ 513
#define BULLET_RE 600

#define SERVER_VERSION 0x000001			/**< Current server version (sent on connect) */
#define MIN_CLIENT_VERSION 0x00000000	/**< Min. client version required (older=kick) */

#define MAX_C_LINES 12
#define MAX_C_LLENGTH 80

#define NICKLEN 63

/********************************************
*  INCLUDES
****************************************************/

/* general */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
// #include <memory.h>
// #include <malloc.h>
#include "ctype.h"

/* time */
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

#if DEBUG2 == 1
/* allegro */
#include <allegro.h>
#include <winalleg.h>
#endif

#if WINDOWS == 1 
/* winsock */
#include <winsock.h>
#include <errno.h>
#include <fcntl.h>
#else
/* unix sockets */
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#ifdef S_SPLINT_S
/* CRAP! */
typedef unsigned char u_int8_t;
typedef unsigned short int u_int16_t;
typedef unsigned int u_int32_t;
typedef uint32_t in_addr_t;
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>
#endif

#include <fcntl.h>

#define FIGHTERZ_SERVER

#include "../common/sys.h"
#include "struct.h"
#include "server.h"
#include "variables.h"
#include "../common/proto.h"

/* Define stuff */

#define STATE_DEADSEND		-201	/* Dead because of a send() problem, send quit msg! */
#define STATE_DEAD			-200	/* Dead, should be closed&freed in mainloop, quit ALREADY sent. */
#define STATE_NEW			-100	/* Just connected */
#define STATE_GOTVERSION	-99		/* CMSG_VERSION received */
#define STATE_LAG			-98		/* CMSG_NICK received and succesfull, now calculating lag */
#define STATE_SYNCHED		-97		/* lag check done */
#define STATE_OK			-96		/* Map sent, ready for spawnage */

#define READY_INT	-96

#define IsRegistered(obj)	(obj->enabled >= STATE_OK)
#define IsDead(obj)			(obj->bullet != 1 && obj->enabled <= STATE_DEAD)

#define SetDeadSend(obj)	do { obj->enabled = STATE_DEADSEND; } while(0);
#define SetDead(obj)		do { obj->enabled = STATE_DEAD; } while(0);
#define SetSynched(obj)		do { obj->enabled = STATE_SYNCHED; } while(0);
#define SetLag(obj)			do { obj->enabled = STATE_LAG; } while(0);
#define SetGotField(obj)	do { obj->enabled = STATE_OK; } while(0);
#define SetVersion(obj)		do { obj->enabled = STATE_GOTVERSION; } while(0);
#define SetNew(obj)			do { obj->enabled = STATE_NEW; } while(0);

#define EVERYONE			NULL

#define T_PLAYER			0
#define T_BOT				1
#define T_BULLET			2

#define IsPlayer(obj)		(obj->type == T_PLAYER)
#define IsBot(obj)			(obj->type == T_BOT)
#define IsBullet(obj)		(obj->type == T_BULLET)
