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
 * dumb.h - The user header file for DUMB.          / / \  \
 *                                                 | <  /   \_
 * Include this file in any of your files in       |  \/ /\   /
 * which you wish to use the DUMB functions         \_  /  > /
 * and variables.                                     | \ / /
 *                                                    |  ' /
 *                                                     \__/
 */

#ifndef DUMB_H
#define DUMB_H


#include <stdlib.h>
#include <stdio.h>


#ifdef __cplusplus
	extern "C" {
#endif


#define DUMB_MAJOR_VERSION 0
#define DUMB_MINOR_VERSION 8
#define DUMB_REVISION_VERSION 1

#define DUMB_VERSION (DUMB_MAJOR_VERSION*10000 + DUMB_MINOR_VERSION*100 + DUMB_REVISION_VERSION)

#if DUMB_REVISION_VERSION > 0
#define DUMB_VERSION_STR #DUMB_MAJOR_VERSION"."#DUMB_MINOR_VERSION"."#DUMB_REVISION_VERSION
#else
#define DUMB_VERSION_STR #DUMB_MAJOR_VERSION"."#DUMB_MINOR_VERSION
#endif

#define DUMB_NAME "DUMB v"DUMB_VERSION_STR

#define DUMB_YEAR  2002
#define DUMB_MONTH 8
#define DUMB_DAY   11

#define DUMB_YEAR_STR2 "02"

#define DUMB_YEAR_STR4  #DUMB_YEAR
#define DUMB_MONTH_STR1 #DUMB_MONTH
#define DUMB_DAY_STR1   #DUMB_DAY

#if DUMB_MONTH < 10
#define DUMB_MONTH_STR2 "0"DUMB_MONTH_STR1
#else
#define DUMB_MONTH_STR2 DUMB_MONTH_STR1
#endif

#if DUMB_DAY < 10
#define DUMB_DAY_STR2 "0"DUMB_DAY_STR1
#else
#define DUMB_DAY_STR2 DUMB_DAY_STR1
#endif


/* WARNING: The month and day were inadvertently swapped in the v0.8 release.
 *          Please do not compare this constant against any date in 2002. In
 *          any case, DUMB_VERSION is probably more useful for this purpose.
 */
#define DUMB_DATE (DUMB_YEAR*10000 + DUMB_MONTH*100 + DUMB_DAY)

#define DUMB_DATE_STR DUMB_DAY_STR1"."DUMB_MONTH_STR1"."DUMB_YEAR_STR4


#undef MIN
#undef MAX
#undef MID

#define MIN(x,y)   (((x) < (y)) ? (x) : (y))
#define MAX(x,y)   (((x) > (y)) ? (x) : (y))
#define MID(x,y,z) MAX((x), MIN((y), (z)))

#undef ABS
#define ABS(x) (((x) >= 0) ? (x) : (-(x)))


/** WARNING - TEMPORARY HACK */
#ifdef DEBUGMODE

#ifndef ASSERT
#include <assert.h>
#define ASSERT(n) assert(n)
#endif
#ifndef TRACE
#define TRACE 1 ? (void)0 : printf
#endif

#else

#ifndef ASSERT
#define ASSERT(n)
#endif
#ifndef TRACE
#define TRACE 1 ? (void)0 : printf
#endif

#endif


#define DUMB_ID(a,b,c,d) (((unsigned int)(a) << 24) | \
                          ((unsigned int)(b) << 16) | \
                          ((unsigned int)(c) <<  8) | \
                          ((unsigned int)(d)      ))



/** WARNING - THE FOLLOWING DEFINITIONS ARE NOT PORTABLE */

#ifndef LONG_LONG
#ifdef __GNUC__
#define LONG_LONG long long
#elif defined _MSC_VER
#define LONG_LONG __int64
#endif
#endif


/* Basic Sample Type */

typedef int sample_t;


/* Library Clean-up Management */

int dumb_atexit(void (*proc)(void));

void dumb_exit(void);


/* File Input Functions */

typedef struct DUMBFILE_SYSTEM
{
	void *(*open)(const char *filename);
	int (*skip)(void *f, long n);
	int (*getc)(void *f);
	long (*getnc)(char *ptr, long n, void *f);
	void (*close)(void *f);
}
DUMBFILE_SYSTEM;

typedef struct DUMBFILE DUMBFILE;

void register_dumbfile_system(DUMBFILE_SYSTEM *dfs);

DUMBFILE *dumbfile_open(const char *filename);
DUMBFILE *dumbfile_open_ex(void *file, DUMBFILE_SYSTEM *dfs);

long dumbfile_pos(DUMBFILE *f);
int dumbfile_skip(DUMBFILE *f, long n);

int dumbfile_getc(DUMBFILE *f);

int dumbfile_igetw(DUMBFILE *f);
int dumbfile_mgetw(DUMBFILE *f);

long dumbfile_igetl(DUMBFILE *f);
long dumbfile_mgetl(DUMBFILE *f);

unsigned long dumbfile_cgetul(DUMBFILE *f);
signed long dumbfile_cgetsl(DUMBFILE *f);

long dumbfile_getnc(char *ptr, long n, DUMBFILE *f);

int dumbfile_error(DUMBFILE *f);
int dumbfile_close(DUMBFILE *f);


/* stdio File Input Module */

void dumb_register_stdfiles(void);

DUMBFILE *dumbfile_open_stdfile(FILE *p);


/* DUH Management */

typedef struct DUH DUH;

#define DUH_SIGNATURE DUMB_ID('D','U','H','!')

DUH *load_duh(const char *filename);
DUH *read_duh(DUMBFILE *f);
void unload_duh(DUH *duh);

/* PLEASE DO NOT USE THIS FUNCTION - IT IS NOT IMPLEMENTED PROPERLY. */
long duh_get_length(DUH *duh);


/* Impulse Tracker Support */

extern int dumb_it_max_to_mix;

DUH *dumb_load_it(const char *filename);
DUH *dumb_load_s3m(const char *filename);
DUH *dumb_load_xm(const char *filename);

DUH *dumb_read_it(DUMBFILE *f);
DUH *dumb_read_s3m(DUMBFILE *f);
DUH *dumb_read_xm(DUMBFILE *f);


/* DUH Rendering Functions */

typedef struct DUH_RENDERER DUH_RENDERER;

DUH_RENDERER *duh_start_renderer(DUH *duh, int n_channels, long pos);

long duh_render(
	DUH_RENDERER *dr,
	int bits, int unsign,
	float volume, float delta,
	long size, void *sptr
);

long duh_renderer_get_position(DUH_RENDERER *dr);

void duh_end_renderer(DUH_RENDERER *dr);


/* Signal Design Helper Values */

/* Use pow(DUMB_SEMITONE_BASE, n) to get the 'delta' value to transpose up by
 * n semitones. To transpose down, use negative n.
 */
#define DUMB_SEMITONE_BASE 1.059463094359295309843105314939748495817

/* Use pow(DUMB_QUARTERTONE_BASE, n) to get the 'delta' value to transpose up
 * by n quartertones. To transpose down, use negative n.
 */
#define DUMB_QUARTERTONE_BASE 1.029302236643492074463779317738953977823

/* Use pow(DUMB_PITCH_BASE, n) to get the 'delta' value to transpose up by n
 * units. In this case, 256 units represent one semitone; 3072 units
 * represent one octave. These units are used by the sequence signal (SEQU).
 */
#define DUMB_PITCH_BASE 1.000225659305069791926712241547647863626


/* Signal Design Function Types */

typedef void *(*DUH_LOAD_SIGNAL)(DUH *duh, DUMBFILE *file);

typedef void *(*DUH_START_SAMPLES)(
	DUH *duh,
	void *signal,
	int n_channels,
	long pos
);

typedef void (*DUH_SET_PARAMETER)(
	void *sampinfo,
	unsigned char id, long value
);

typedef long (*DUH_RENDER_SAMPLES)(
	void *sampinfo,
	float volume, float delta,
	long size, sample_t **samples
);

typedef void (*DUH_END_SAMPLES)(void *sampinfo);

typedef void (*DUH_UNLOAD_SIGNAL)(void *signal);


/* Signal Design Function Registration */

typedef struct DUH_SIGTYPE_DESC
{
	long type;
	DUH_LOAD_SIGNAL    load_signal;
	DUH_START_SAMPLES  start_samples;
	DUH_SET_PARAMETER  set_parameter;
	DUH_RENDER_SAMPLES render_samples;
	DUH_END_SAMPLES    end_samples;
	DUH_UNLOAD_SIGNAL  unload_signal;
}
DUH_SIGTYPE_DESC;

void dumb_register_sigtype(DUH_SIGTYPE_DESC *desc);


/* Signal Rendering Functions */

typedef struct DUH_SIGNAL_SAMPINFO DUH_SIGNAL_SAMPINFO;

DUH_SIGNAL_SAMPINFO *duh_signal_start_samples(
	DUH *duh,
	int sig,
	int n_channels,
	long pos
);

void duh_signal_set_parameter(
	DUH_SIGNAL_SAMPINFO *signal_sampinfo,
	unsigned char id, long value
);

long duh_signal_render_samples(
	DUH_SIGNAL_SAMPINFO *signal_sampinfo,
	float volume, float delta,
	long size, sample_t **samples
);

void duh_signal_end_samples(DUH_SIGNAL_SAMPINFO *signal_sampinfo);


/* Silencing Helper */

void dumb_silence(sample_t *samples, long length);


/* Resampling Helpers */

/* DUMB_EXTRA_SAMPLES is the maximum number of extra samples that might be
 * required after the end by the resampling algorithms. For a looping sample,
 * the extra samples should be set the same as the samples at the loop-back
 * point. For a non-looping sample, they should be set to zero.
 */
#define DUMB_EXTRA_SAMPLES 3

extern int dumb_resampling_quality;

typedef int (*DUMB_RESAMPLE_PICKUP)(
	sample_t *src, long *_src_pos, int *_src_subpos,
	long *_src_start, long *_src_end,
	int dir,
	void *data
);

long dumb_resample(
	sample_t *src, long *_src_pos, int *_src_subpos,
	long src_start, long src_end,
	sample_t *dst, long dst_size,
	float volume, float delta, int *_dir,
	DUMB_RESAMPLE_PICKUP pickup, void *pickup_data
);


/* DUH Construction */

DUH *make_duh(
	long length,
	int n_signals,
	DUH_SIGTYPE_DESC *desc[],
	void *signal[]
);


#ifdef __cplusplus
	}
#endif


#endif /* DUMB_H */

