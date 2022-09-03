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
 * plays3m.c - Simple program to play S3M files.    / / \  \
 *                                                 | <  /   \_
 * By entheh.                                      |  \/ /\   /
 *                                                  \_  /  > /
 *                                                    | \ / /
 *                                                    |  ' /
 *                                                     \__/
 */

#include <stdlib.h>
#include <allegro.h>

/* Note that your own programs should use <aldumb.h> not "aldumb.h". <> tells
 * the compiler to look in the compiler's default header directory, which is
 * where DUMB should be installed before you use it (make install does this).
 * Use "" when it is your own header file. This example uses "" because DUMB
 * might not have been installed yet when the makefile builds it.
 */
#include "aldumb.h"



#ifdef ALLEGRO_WINDOWS
static int closed = 0;

static void closehook(void)
{
	closed = 1;
}
#else
#define closed 0
#endif



static void usage(void)
{
	allegro_message(
		"Usage: plays3m file.s3m\n"
		"This will play the .s3m file specified.\n"
		"You can control playback quality by editing dumb.ini.\n"
	);

	exit(1);
}



int main(int argc, char *argv[])
{
	DUH *duh;
	AL_DUH_PLAYER *dp;

	if (allegro_init())
		return 1;

	if (argc != 2)
		usage();

	set_config_file("dumb.ini");

	if (install_keyboard()) {
		allegro_message("Failed to initialise keyboard driver!\n");
		return 1;
	}

	set_volume_per_voice(0);

	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL)) {
		allegro_message("Failed to initialise sound driver!\n%s\n", allegro_error);
		return 1;
	}

	atexit(&dumb_exit);

	dumb_register_packfiles();

	duh = dumb_load_s3m(argv[1]);
	if (!duh) {
		allegro_message("Failed to load %s!\n", argv[1]);
		return 1;
	}

	dumb_resampling_quality = get_config_int("sound", "dumb_resampling_quality", 4);
	dumb_it_max_to_mix = get_config_int("sound", "dumb_it_max_to_mix", 128);

#ifdef ALLEGRO_WINDOWS
	{
		const char *fn = get_filename(argv[1]);
		int w = strlen(fn);
		if (w < 22) w = 22;
		w = (w + 2) * 4;

		set_window_title("DUMB - S3M player");

		if (set_gfx_mode(GFX_GDI, w*2, 32, 0, 0) == 0) {
			acquire_screen();
			textout_centre(screen, font, fn, w, 8, 14);
			textout_centre(screen, font, "Press any key to exit.", w, 16, 11);
			release_screen();
		}
	}

	set_window_close_hook(&closehook);
#endif

	set_display_switch_mode(SWITCH_BACKGROUND);

	dp = al_start_duh(duh, 2, 0, 1.0,
		get_config_int("sound", "buffer_size", 4096),
		get_config_int("sound", "sound_freq", 44100));

	for (;;) {
		if (keypressed()) {
			readkey();
			break;
		}

		if (al_poll_duh(dp) || closed)
			break;

		yield_timeslice();
	}

	al_stop_duh(dp);

	unload_duh(duh);

	return 0;
}
END_OF_MAIN();

