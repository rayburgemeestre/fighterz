#include "header.h"

void init()
{
 // Initialize Allegro.        
 allegro_init();      

 // Set the resolution to 640 by 480 with SAFE autodetection.
 set_gfx_mode(GFX_SAFE, 800, 600, 0, 0);

 // Installing the keyboard handler.
 install_keyboard();
 install_mouse();

// while(! key[KEY_F12])
//   poll_keyboard(); // This shouldn't be necessary in Windows.
}
