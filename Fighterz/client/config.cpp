#include "common.h"

/********************************************
*  CONFIG PROC
****************************************************/


void config(int ignore) /* ignore @ 1 = don't use host/port in config */
{
	FILE *fi;
#define MAX_B_L 500
	char buff[MAX_B_L];
	char *p;
	char *configfile;

						// 512 for extra path
	configfile = (char *)malloc((512+strlen(CONFIG_FILE)) * sizeof(char));
	strcpy(configfile, CONFIG_FILE);

	if ((fi = fopen(configfile, "r")) == NULL)
	{
		printff_direct("Error loading config file");
		printff_direct(configfile);
		terminate();
	}
	
	while (fgets(buff, (MAX_B_L - 1), fi) != NULL) 
	{
		if (buff[0] == ';' || buff[0] == 13 || buff[0] == 10)
			continue;
		else
		{
			if (!(p = strchr(buff, ':')))
			{
				printff_direct("Illegal data in config file");
				terminate();
			}

			*p = '\0';
			if (!strcmp(buff, "nick"))
			{
				int i = 0;

				p++;
				if (!p)
				{
					printff_direct("Illegal data in config file");
					terminate(); 	
				}
				if (strlen(p) > 20)
					*(p + 20) = '\0';
				
				while (*(p + i) != 10 && *(p + i) != 13)
					i++;
				*(p + i) = '\0';

				strcpy(nickname, p);
				printff_direct("  nick:%s", nickname);
				// delay(10);
			}
			if (!strcmp(buff, "addr") && ignore != 1)
			{
				int i = 0;

				p++;
				if (!p)
				{
					printff_direct("Illegal data in config file");
					terminate(); 	
				}
				if (strlen(p) > 512)
					*(p + 512) = '\0';
				
				while (*(p + i) != 10 && *(p + i) != 13)
					i++;
				*(p + i) = '\0';

				strcpy(taddr, p);

				printff_direct("  taddr:%s", taddr);
			}
			if (!strcmp(buff, "port") && ignore != 1)
			{
				int i;

				p++;
				if (!p)
				{
					printff_direct("Illegal data in config file");
					terminate(); 	
				}

				if (strlen(p) > 5)
					*(p + 5) = '\0';
				
				i = atoi(p);
				tport = i;

				printff_direct("  tport:%d", tport);
			}
			if (!strcmp(buff, "fullscreen"))
			{
				int i;

				p++;
				if (!p)
				{
					printff_direct("Illegal data in config file");
					terminate(); 	
				}

				if (strlen(p) > 1)
					*(p + 1) = '\0';
				
				i = atoi(p);
				if (FULLSCREEN != i)
				{
				    FULLSCREEN = i;
					init_screen();
				}

				//printff_direct("  fullscreen:%s", (FULLSCREEN == 1?"yes":"no"));
			}
			if (!strcmp(buff, "grid"))
			{
				int i;

				p++;
				if (!p)
				{
					printff_direct("Illegal data in config file");
					terminate(); 	
				}

				if (strlen(p) > 1)
					*(p + 1) = '\0';
				
				i = atoi(p);
				grid = (i == 1?1:0);
				printff_direct("  grid:%s", (grid == 1?"on":"off"));
			}
			if (!strcmp(buff, "high_graphics"))
			{
				int i;

				p++;
				if (!p)
				{
					printff_direct("Illegal data in config file");
					terminate(); 	
				}

				if (strlen(p) > 1)
					*(p + 1) = '\0';
				
				i = atoi(p);
				HIGH_GRAPHICS = i;
				printff_direct("  high graphics:%s", (grid == 1?"yes":"no"));
			}
		}
	}
}