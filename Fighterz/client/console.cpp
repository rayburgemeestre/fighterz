#include "common.h"

/********************************************
*  CONSOLE FUNCTIONS
****************************************************/

void drawconsole()
{
 	int i = 0;
	int cur_pos = 0;
	int temp;

	clear_to_color(CONSOLE, 0);

	while (i < MAX_C_LINES)
	{
		temp = 0;
		
		if (!strncmp(console[i], "***", 3))
			temp = 1;
		
		if (console[i][0] == '[')
			temp = 2;

		textprintf
		(
			CONSOLE, 
			(FONT *)dataf[NOKIA].dat, 
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

	if (strlen(buf) > MAX_C_LLENGTH)
		buf[MAX_C_LLENGTH - 1] = '\0';
	
	while (i < (MAX_C_LINES - 1))
	{
		strcpy(console[i], console[i+1]);
		i++;
	}

	strcpy(console[i], buf);
#if DEBUG == 1
	fprintf(dbug, "%s\n", buf);
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
	if ( ( ourtime - large_text_time ) > ( large_text_display_time * 1000 ) )
		large_text_msg[0] = '\0';
	
	if (strlen(large_text_msg) == 0)
		return;

	textprintf(
		tmpscreen, 
		(FONT *)dataf[ARCADE].dat, 
		50, 
		400, 
		makecol(255, 0, 0), 
		large_text_msg	
	);
}