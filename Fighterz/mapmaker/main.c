#include "header.h"

char mapfile[512] = "";
char mapdata[512][512];
char edit_x_width[64];
char edit_y_width[64];

void die(char *diemsg)
{
    alert("Fatal error:", diemsg, "shutting down..", "OK", NULL, 0, 0);
    terminate();
}

int main()
{ 
    init();
    init_map();
    init_help();
    mainloop();
    terminate();

    return 0;
}     

// Some Allegro magic to deal with WinMain().
END_OF_MAIN();
