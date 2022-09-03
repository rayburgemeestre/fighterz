/********************************************
 *   This will become the remake of my VB version,
 *
 *								Started: 07.08.02
 ****************************************************/

#include "common.h"

// general
int screensize_y = 600; /**< screen height size in pixels */
int screensize_x = 800; /**< screen width size in pixels */
int desktop_y = 600;
int desktop_x = 800;
int fullscreen = 0;     /**< boolean: 0=windowed, 1=fullscreen */
int enable_vsync = 0;   /**< boolean: 1=rest()+vsync(),0=not */
int enable_stretch = 0; /**< boolean: 1=yes,0=no */
int game_started;       /**< boolean: 0=no,1=yes */
// settings
unsigned int ship_maxpower; /**< bullets count after which a ship dies */
// sound
MIDI *bg_music;    /**< used to play background midi ingame */
MIDI *intro_music; /**< used to play background midi in options */
// data
DATAFILE *dat_base;  /** datafile: contains all fonts & graphics used in the game */
DATAFILE *dat_sound; /** datafile: sound container (midi,wav,etc..) */
// modifiers
int mod_bounce;                  /**< boolean: 0=bullets die at walls,1=bullets bounce off walls [alpha] */
int ship_movespeed_multiplier;   /**< multiplies this value with the ships moving speed */
int bullet_movespeed_multiplier; /**< multiplies this value with the bullet moving speed */
int mod_grid;                    /**< boolean: 0=no,1=yes, show grid instead of normal graphics */
// fakeconsole (the console like output at startup and connect)
int fakeconsole_y; /**< contains text y-pos on screen for use with textprintf() */
// Local info
unsigned int our_id;    /**< our client_id at the server */
char our_nickname[64];  /**< Will contain our nickname */
int our_direction;      /**< boolean: our_direction is 0=down,1=up */
int our_spawnstatus;    /**< boolean: 0=we cannot spawn,1=we may spawn */
int our_spawnrequested; /**< boolean: spawn request 0=not sent,1=sent */
// Controls, Keyboard
unsigned long bullet_fire_time; /**< used in calculating the automatic fire on keydown delay */
int our_bullet_count;           /**< number of bullets currently exist in the field */
int our_bullet_max;             /**< maximum number of bullets we can have in the field */
int our_bullet_ttl;             /**< our bullet's time-to-live in milliseconds*/
int our_bullet_autofiredelay;   /**< millisecs delay between bullets in autofire */
int talk_mode;                  /**< boolean: 0=not in talk mode,1=keystrokes are typed into the
                                                                          message/command console */
char msg[MSG_LINE_LENGTH];      /**< when talking characters are stored in this array */
// Large text large_text(), large_text_draw();
char large_text_msg[64];
unsigned long large_text_time;
unsigned int large_text_display_time = 5;  // secs
// Debug:
#if DEBUG == 1
FILE *debugfile; /**< debug file descriptor: used for logging some data */
#endif

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
#else
int main(int argc, char *argv[])
#endif
{
  if (access("resources", F_OK) == 0) {
    chdir("resources");
  }

  // char apppath[_MAX_PATH]; /* _MAX_PATH defined in windows.h */
  char *ptr, *ip, *port;
  int ret, skip_options = 0;

  while (!key[KEY_ESC]) {
#if DEBUG == 1
    if ((debugfile = fopen("debug.log", "w+")) == NULL) {
      terminate();
    }
#endif
    initialize_vars();
    init_gui();

    init(); /* initialize stuff */

#ifdef _WIN32
    config((strlen(lpszArgument) == 0 ? 0 : 1));
    /* TEMPORARY: */
    // fullscreen = 0;

    ptr = NULL;
    port = NULL;
    ptr = lpszArgument;
    /* 11 = strlen of: fighterz:// */
    if (strlen(ptr) >= 11) {
      ptr += 11;
      if (ptr != NULL) {
        /* ptr now contains ip :) */
        ip = ptr;
        ptr = NULL;
        ptr = strchr(ip, ':');
        if (ptr != NULL) {
          /* er zou nog een port moeten zijn */
          *ptr = '\0';
          ptr++;
          if (ptr != NULL) {
            /* er zit hier iets */
            port = ptr;
            ptr = strchr(ptr, '/');
            if (ptr != NULL) {
              *ptr = '\0'; /* weg met die crap */
            }
            serv_port = atoi(port);
          }
        }
        strcpy(serv_addr, ip);
        skip_options = 0;
      }
    }
    // skip_options = 1;
#else
    config(0);
#endif

    if (enable_stretch == 0) {
      printf("enabling stretch was disabled, so resetting\n");
      desktop_x = screensize_x;
      desktop_y = screensize_y;
    } else {
      if (get_desktop_resolution(&desktop_x, &desktop_y) == 0) {
        printf("Read desktop resolution!\n");
        init_screen();
      }
    }

    if (skip_options != 1) {
      set_volume(255, 255);
      play_midi(intro_music, TRUE);

      clear_to_color(screen, makecol(0, 0, 0));
      textprintf_centre(screen,
                        (FONT *)dat_base[ARCADE].dat,
                        screensize_x / 2,
                        screensize_y / 2,
                        makecol(192, 192, 192),
                        "Tachyon Fighterz");

      if (fullscreen == 1) {
#ifdef _WIN32
        Sleep(2000);  // extra delay for switching resolution..
#else
        sleep(2);
#endif
      }

      // Sleep(1000);

      clear_to_color(screen, makecol(0, 0, 0));
      textprintf_centre(screen,
                        (FONT *)dat_base[ARCADE].dat,
                        screensize_x / 2,
                        screensize_y / 2,
                        makecol(192, 192, 192),
                        "www.fighterz.net");
      // Sleep(1000);
    }

    while (!key[KEY_ESC]) {
      // alert("in while!", "", "", "OK", NULL, KEY_ENTER, 1);
      if (skip_options != 1)
        ret = get_options();
      else
        ret = 0;

      fakeconsole_y = 0;

      switch (ret) {
        case 1:
          alert("Singleplayer does not yet exist",
                "you are now being redirect to multiplayer",
                "",
                "Thanks for the info",
                NULL,
                KEY_ENTER,
                0);
        case 0:
          if (-1 == start() || -1 == mainloop()) {
#ifdef _WIN32
            WSACleanup();
#endif

#if DEBUG == 1
            fclose(debugfile);
#endif
            initialize_vars();
            play_midi(intro_music, TRUE);
          }
          break;
        case 2:
          get_settings();
          break;
        default:
          break;
      }
    }
    /*
                            WSACleanup();
    #if DEBUG == 1
                            fclose(debugfile);
    #endif DEBUG
    */
    // we need a de_initiatlize_vars here
    // to free linked lists etc,
    // if we let the while continue, the program crashes, so:
    break;
  }

  destroy_midi(bg_music);
  destroy_midi(intro_music);

  /* exit program */
  alfont_destroy_font(lcdfont);
  alfont_destroy_font(tccmfont);
  alfont_exit();
  allegro_exit();
  return 0;
}
// END_OF_MAIN();
