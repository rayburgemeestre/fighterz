#include "common.h"

/********************************************
*  CONSOLE FUNCTIONS
****************************************************/

void drawconsole()
{
 	int i = 0;
	int cur_pos = 0;
	int temp;
	
	while (i < MAX_C_LINES)
	{
		temp = 0;
		if (console[i][0] != '\0' && console[i][0] == '*')
			if (console[i][1] != '\0' && console[i][1] == '*')
				if (console[i][2] != '\0' && console[i][2] == '*')
					temp = 1;

		textprintf(CONSOLE, font, 1, cur_pos + 1, (temp==1?makecol(0,128,255):makecol(255,128,0)) , console[i]);
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