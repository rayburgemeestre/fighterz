#include "header.h"

char mapfile[512] = "";
char mapdata[512][512];

DIALOG the_dialog[] =
{
    /* (dialog proc)     (x)   (y)   (w)   (h)
                         (fg)  (bg)  (key) (flags)
                         (d1)  (d2)  (dp) */

    { d_clear_proc,      0,    0,    0,    0,
                         255,  0,    0,    0,
                         0,    0,    NULL },

    { d_ctext_proc,       80,   32,   512,  48,
                         255,  0,    0,    0,
                         512,   0,    mapfile },

    { NULL,              0,    0,    0,    0,
                         0,    0,    0,    0,
                         0,    0,    NULL }
};

void die(char *diemsg)
{
    alert("Fatal error:", diemsg, "shutting down..", "OK", NULL, 0, 0);
    terminate();
}

int main()
{ 
    init();
    loadmap();
    terminate();

    return 0;
}     

// Some Allegro magic to deal with WinMain().
END_OF_MAIN();

/*
char *xprintf(char *pattern, ...)
{
	static char buf[512];
    char *ptr = buf;
	int total = 0;
	int bytesleft;
	int n;

	va_list ap;
	va_start (ap, pattern);
	vsprintf(buf, pattern, ap);
	va_end(ap);

	return buf;
}
*/
