/********************************************
*  COMMON DEFINES / HEADER FILES
****************************************************/

#define OPTION_COUNT 2
#define ASTEROIDS_MODE 1 // asteroids like flying?
#define DONT_LOSE_VELOCITY_AT_COLLISION false
#define DEBUG 1
#define MAX_F_LEN 81 /* max length, used when reading files (fgets buffer) */
#define PORT 8099
#define PI 3.14159265358979
#define IGNORE_LAG 0
#define NOSOUND 0
#define CONFIG_FILE "fighterz.cfg"
/* #define ALLEGRO_STATICLINK
   #define USE_CONSOLE   */
#define SERVER_VERSION 0x000001			/**< Current server version (sent on connect) */
#define MIN_CLIENT_VERSION 0x00000000	/**< Min. client version required (older=kick) */
#define CLIENT_VERSION 0x00000001
#define BASE_DATAFILE "system\\base.dat"
#define BASE_SOUND_DATAFILE "system\\base_snd.dat"

#define T_PLAYER			0
#define T_BOT				1
#define T_BULLET			2

#define IsPlayer(obj)		(obj->type == T_PLAYER)
#define IsBot(obj)			(obj->type == T_BOT)
#define IsBullet(obj)		(obj->type == T_BULLET)

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <allegro.h>
#include <math.h>
#include <winalleg.h>
#include <winsock.h>
/* Time */
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

#include <fcntl.h> 

#include "data.h"
#include "sound.h"
#include "main.h"
#include "../common/proto.h"
#include "../common/sys.h"
#include "struct.h"
#include "variables.h"
/* needed for current working directory: 
   #include <direct.h> */

// not using: #include <aldumb.h> // for XM etc