#include "common.h"

/********************************************
*  CONSOLE FUNCTIONS
****************************************************/

void draw_console()
{
 	int i = 0;
	int cur_pos = 0;
	int temp;

	clear_to_color(bmp_console, makecol(0,0,0));

	while (i < CON_LINES)
	{
		temp = 0;
		
		if (!strncmp(console[i], "***", 3))
			temp = 1;
		
		if (console[i][0] == '[')
			temp = 2;

		textprintf
		(
			bmp_console, 
			(FONT *)dat_base[NOKIA].dat, 
			1, 
			cur_pos + 1, 
			(
				temp == 1
			?
				makecol(0,128,255)
			:
				(
					temp == 2
				?
					makecol(0, 64, 128)
				:
					makecol(192, 192, 192)
				)
			),
			console[i]
		);

		cur_pos += 10;
		i++;
	}
}

void addtext(char *pattern, ...)
{
	char buf[512];
	int i = 0;

	va_list ap;
	va_start (ap, pattern);
	vsprintf(buf, pattern, ap);
	va_end(ap);

	if (strlen(buf) > CON_LINE_LENGTH)
		buf[CON_LINE_LENGTH - 1] = '\0';
	
	while (i < (CON_LINES - 1))
	{
		strcpy(console[i], console[i+1]);
		i++;
	}

	strcpy(console[i], buf);
#if DEBUG == 1
	fprintf(debugfile, "%s\n", buf);
#endif

}

void large_text(char *pattern, ...)
{
	char buf[512];
	int total = 0;

	va_list ap;
	va_start (ap, pattern);
	vsprintf(buf, pattern, ap);
	va_end(ap);

	if (strlen(buf) > 64)
		*(buf + 63) = '\0';
	
	strcpy(large_text_msg, buf);
	
	large_text_time = ourtime;

	return;
}

void large_text_draw()
{
int i;

	if ( ( ourtime - large_text_time ) > ( large_text_display_time * 1000 ) )
		large_text_msg[0] = '\0';
	
	if (strlen(large_text_msg) == 0)
		return;

//	textprintf(
//		tmpscreen, 
//		(FONT *)dat_base[ARCADE].dat, 
//		50, 
//		400, 
//		makecol(255, 0, 0), 
//		large_text_msg	
//	);

	// make upper case
	for (i=0; large_text_msg[i] != '\0';)
	{
		if (large_text_msg[i] >= 'a' && large_text_msg[i] <= 'z')
			large_text_msg[i] -= 32;
		i++;
	}

	alfont_textout_centre_aa_ex(tmpscreen, tccmfont, large_text_msg, SCREEN_W / 2, SCREEN_H / 2 - 28, makecol(85, 239, 111), -1);

}