/*  _______         ____    __         ___    ___
 * \    _  \       \    /  \  /       \   \  /   /     '   '  '
 *  |  | \  \       |  |    ||         |   \/   |       .      .
 *  |  |  |  |      |  |    ||         ||\  /|  |
 *  |  |  |  |      |  |    ||         || \/ |  |       '  '  '
 *  |  |  |  |      |  |    ||         ||    |  |       .      .
 *  |  |_/  /        \  \__//          ||    |  |
 * /_______/edicated  \____/niversal  /__\  /____\usic /|  .  . astardisation
 *                                                    /  \
 *                                                   / .  \
 * aldumb.h - The user header file for DUMB with    / / \  \
 *            Allegro.                             | <  /   \_
 *                                                 |  \/ /\   /
 * Include this file if you wish to use DUMB        \_  /  > /
 * with Allegro. It will include dumb.h for you,      | \ / /
 * and provide extra functionality such as audio      |  ' /
 * stream and datafile integration.                    \__/
 */

#ifndef ALDUMB_H
#define ALDUMB_H


#include <allegro.h>

#include "dumb.h"


#ifdef __cplusplus
	extern "C" {
#endif


/* Packfile Support */

void dumb_register_packfiles(void);

DUMBFILE *dumbfile_open_packfile(PACKFILE *p);


/* Datafile Registration Functions */

#define DUMB_DAT_DUH DAT_ID('D','U','H',' ')
#define DUMB_DAT_IT  DAT_ID('I','T',' ',' ')
#define DUMB_DAT_XM  DAT_ID('X','M',' ',' ')
#define DUMB_DAT_S3M DAT_ID('S','3','M',' ')

void dumb_register_dat_duh(long type);
void dumb_register_dat_it(long type);
void dumb_register_dat_xm(long type);
void dumb_register_dat_s3m(long type);


/* DUH Playing Functions */

typedef struct AL_DUH_PLAYER AL_DUH_PLAYER;

AL_DUH_PLAYER *al_start_duh(DUH *duh, int n_channels, long pos, float volume, long bufsize, int freq);
void al_stop_duh(AL_DUH_PLAYER *dp);
void al_pause_duh(AL_DUH_PLAYER *dp);
void al_resume_duh(AL_DUH_PLAYER *dp);
void al_duh_set_volume(AL_DUH_PLAYER *dp, float volume);
int al_poll_duh(AL_DUH_PLAYER *dp);
long al_duh_get_position(AL_DUH_PLAYER *dp);


#ifdef __cplusplus
	}
#endif


#endif /* ALDUMB_H */
