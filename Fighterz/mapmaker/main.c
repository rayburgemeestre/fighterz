/*
    MapMaker for Fighterz

    Build Using Dev-C++ 4 & Allegro for graphics.
*/

#include "header.h"

char mapfile[512] = "";
char mapdata[512][512];
char edit_x_width[64];
char edit_y_width[64];

void die(char *diemsg)
{
    alert("Fatal error:", diemsg, "shutting down..", "OK", NULL, 0, 0);
    exit(-1);
}

int main()
{
	if (access("resources", F_OK) == 0) {
		chdir("resources");
	}

    init();
    // init_map(0);
    init_datafile(0, DEFAULT_DATA_FILE);
    init_help();
    mainloop();
    return 0;
}     

// Some Allegro magic to deal with WinMain().
END_OF_MAIN();
