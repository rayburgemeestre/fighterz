#include "common.h"

/********************************************
*  PARSE FUNCTIONS 
****************************************************/

int parse(char *stuff)
{   char *str, *tussen, *p1;
    static char tmpstr[1024] = "";
    int i = 0,j = 0, ret = 0;
	
    /* enough memory for stuff & tmpstr */
    if (!(str = (char *)calloc((strlen(stuff)+1)+strlen(tmpstr), sizeof(char) )))
    {   perror("Error allocating memory");
        exit(-1);
    }

    strcpy(str, stuff);

    /* remove \r from stuff */
    if (!(tussen = (char *)calloc( ( strlen(str) + 1 ) + strlen(tmpstr), sizeof(char) )))
    {   perror("Error allocating memory");
        exit(-1);
    }
    p1 = tussen; /* because tussen gets fragged */
    
    while (i < (int)(strlen(str) + 1) )
    {   
		if (str[i] != '\r') /* frag 'm */
            tussen[j++] = str[i];
        i++;
    }
    strcpy(str, tussen); 

    /* str is now \r-free */

    /* Old stuff needs to be inserted? */
    if (strlen(tmpstr)>0)
    {  
        strcat(tmpstr, str); /* put stuff from str behind tmpstr */
        strcpy(str, tmpstr); /* then put tmpstr back in str */
        tmpstr[0] = 0; /* 'emtpy' tmpstr */
    }

	ret = 0;
    while (strchr(str, '\n'))
    {  
        /* Keep copy */
        strcpy(tussen, str);
        /* Show line */
		if (strlen(tussen) > 0)
		{
			if (parse2(strtok(str,"\n")) == 1)
				ret = 1;

		} 

        tussen = strchr(tussen, '\n'); /* Screw that line */
        tussen++; /* We don't need the leading \n */
        /* Restore backup */
        strcpy(str, tussen);
    }

    if (strlen(str)>0) strcpy(tmpstr, str);
    
    free(str);
    free(p1);

	return ret;
}

#define LAG_CHECKS 5
// i know the protocol is pretty stupid but i will when the game is finished
// replace it with something better..
int parse2(char *stuff)

{   
	char *ptr, *bptr; /* backup ptr*/
	static int lag_cnt = 0;
	static int lag_cnt2 = 0;
	int ret = 0;

	if (stuff == NULL)
		return 0;
	
	// addtext("<- %s", stuff);
	printff_direct("  Received: %s", stuff);


#if DEBUG == 1
	if (strlen(stuff) > 0)
	{
		fprintf(dbug, "Rcvd: %s\n", stuff);
		fflush(dbug);
	}
#endif

	if (strchr(stuff, ' '))
	{
		ptr = strchr(stuff, ' ');
		*ptr = '\0';
		ptr++;
		bptr = ptr;
		/* ****************************************
		   0 <msg> - SERVER message (before game started)
		   **************************************** */
		if (!strcmp(stuff, "0"))
		{
			printff_direct("# %s", ptr);
		} 
		/* ****************************************
		   1 <servertime> - PING to calculate lag
		   **************************************** */
		if (!strcmp(stuff, "1"))
		{
			/* lag check */
			if (lag_cnt < LAG_CHECKS)
			{
				//if (lag_cnt <= LAG_CHECKS)
				lag_cnt++; 
				// // so it will go to LAG_CHECKS+1 and keep that value

				printff_direct("  Calculating lag.. [%d/%d]", lag_cnt, LAG_CHECKS);

			}
			sockwrite("1 %s\n", ptr);
		} 
		/* ****************************************
		   2 <servercalculatedlag> <servertime> - SERVER calculated lag
		   (PING -> PONG / 2), and its time
		   **************************************** */
		if (!strcmp(stuff, "2"))
		{
			char *ptr2 = ptr;
			ptr = strchr(ptr, ' ');
			
			if (ptr == NULL) 
			{
				printff_direct("Got illegal data.. reconn for now disconnect :P"); /* TODO: change */
				terminate();
			}

			*ptr = '\0';
			ptr++;
			
			if (lag_cnt2 < LAG_CHECKS) 
				lag_cnt2++;
			if (lag_cnt2 == LAG_CHECKS)
				ret = 0; // was: ret = 1; but i changed my mind! :)

			lag_proc(ptr2, ptr);
			// printff_direct("lag 1:%2.2f 2:%2.2f 3:%2.2f 4:%2.2f 5:%2.2f", lag[0] , lag[1] , lag[2] , lag[3] , lag[4]);
			// addtext("Calculated lag - now: %2.2f", current_lag());
#if DEBUG == 1
			//fprintf(dbug, "Recalculated lag: %2.2f\n", current_lag());
#endif
		}
		/* ****************************************
		   3 <msg> - SERVER message (when in game)
		   **************************************** */
		if (!strcmp(stuff, "3"))
		{
			char *ptr2 = ptr;

			if (ptr != NULL)
			{
				if (strlen(ptr) > MAX_C_LLENGTH)
					*(ptr+(MAX_C_LLENGTH - 1)) = '\0';
				addtext("%s", ptr);
			}
		}
		/* ****************************************
		   4 <id> <x> <y> <deg> <move> <velocity> <alive> <kills> <killavg> <dead> <nick>
		   our initialization info from the server so we can add ourself to db.
		   5 <id> <x> <y> <deg> <move> <velocity> <alive> <kills> <killavg> <dead> <nick>
		   already connected player's information so we can add them too
		   R <id> <x> <y> <deg> <move> <velocity> <alive> <kills> <killavg> <dead> <nick>
		   to correct stuff 
		   **************************************** */
		if (!strcmp(stuff, "4") || !strcmp(stuff, "5") || !strcmp(stuff, "R") )
		{
			LINK tmp;
			double deg, speed;
			int velocity, turn, bot, dead;
			unsigned int id, move;
			unsigned long alive;
			long int kills, killavg;
			double x, y;

			char *nick, *ptr2 = ptr;

			/* extract some stuff + check for some errors */
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				id = atoi(ptr);
			} else { alert("q", "","","","",1,1); terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				x = atof(ptr2);
			} else { alert("qwf", "","","","",1,1); terminate(); }

			ptr++;
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				y = atof(ptr);
			} else { alert("dffd", "","","","",1,1); terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				deg = atof(ptr2);
			} else { alert("qqqq", "","","","",1,1); terminate(); }
			ptr++;
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				move = atoi(ptr);
			} else { alert("fdffdf", "","","","",1,1); terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				velocity = atoi(ptr2);
			} else { alert("aaaaaa", "","","","",1,1); terminate(); }
			ptr++;
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				alive = atoi(ptr);
			} else { alert("aaaaaaa", "","","","",1,1); terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				kills = atol(ptr2);
			} else { alert("a", "","","","",1,1); terminate(); }
			ptr++;
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				killavg = atol(ptr);
			} else { alert("b", "","","","",1,1); terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				turn = atoi(ptr2);
			} else { alert("c", "","","","",1,1); terminate(); }
			ptr++;
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				bot = atoi(ptr);
			} else { alert("zxcc", "","","","",1,1); terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				speed = atof(ptr2);
			} else { alert("fc", "","","","",1,1); terminate(); }
			ptr++;
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				dead = atof(ptr);
			} else { alert("fc", "","","","",1,1); terminate(); }

			ptr2++;
			if (ptr2)
				nick = ptr2;
			else 
				terminate();

			if (strcmp(stuff, "R"))
			{
    			add_player(id, x, y, deg, move, turn, velocity, alive, kills, killavg, bot, speed, dead, nick);
			} else {
				// (only correct)
				tmp = head;
				while (tmp->id != id)
					tmp = tmp->next;
				if (tmp == NULL)
					return ret;
				
				tmp->x = x;
				tmp->y = y;
				tmp->deg = deg;
				tmp->move = move;
				tmp->turn = velocity;
				tmp->alive = alive;
				tmp->kills = kills;
				tmp->kills_avg = killavg;
				tmp->speed = speed;
				tmp->dead = dead;
				strcpy(tmp->nick, nick);
				return ret;
			}

			// addtext("id:%u x:%2.2f y:%2.2f deg:%d turn:%d", id, x, y, deg, turn);
			
			if (!strcmp(stuff, "4"))
			{
					printff_direct("   id:%u x:%f y:%f deg:%.2f move:%u turn:%d speed:%.2f", id, x, y, deg, move, turn, speed);
					printff_direct("   vel:%d alive:%lu kills:%ld killavg:%ld nick:%s", velocity, alive, kills, killavg, nick);

				if (our_id == -1)
					our_id = id;

				our_node = head;
				while (our_node->id != our_id)
					our_node = our_node->next;

				// TODO: remove these 3 lines: 			
				// printff_direct("Press enter to proceed..");
				//showscr = 1;
				// while(! key[KEY_ENTER]);
				
					addtext("*** Game started ***");
				
				bullet_time = 0;
				
				// STARTED = 1;
				
				/* Send our nickname */
				if (strlen(nickname) < 1)
					sprintf(nickname, "Unit%d", our_id);

				strcpy(our_node->nick, nickname);
				sockwrite("N %s\n", nickname);
			}
		}
		/* ****************************************
		   N <id> <nick> - a player's nickchange
		   **************************************** */
		if (!strcmp(stuff, "N"))
		{
			char *ptr2;

			if (!(ptr2 = strchr(ptr, ' ')))
			{
				printff_direct("bla bla dood");
				terminate();
			}
			*ptr2 = '\0';
			ptr2++;

			/* ptr contains id, ptr2 nick */

			if (strlen(ptr2) > 64)
				*(ptr2+63) = '\0';

			current = head;
			while (current != NULL)
			{
				if (current->id == (unsigned int)atoi(ptr))
					break;
				current = current->next;
			}

			if (current != NULL)
				strcpy(current->nick, ptr2);
		}
        /* ****************************************
		   K <id> <id> - id killed id :)
		   **************************************** */
		if (!strcmp(stuff, "K"))
		{
			LINK c2;
			int id1, id2;
			char *ptr2;

			if (!(ptr2 = strchr(ptr, ' ')))
			{
				printff_direct("bla bla dood");
				terminate();
			}
			*ptr2 = '\0';
			ptr2++;

			id1 = atoi(ptr);
			id2 = atoi(ptr2);

			current = head;
			while (current != NULL)
			{
				if (current->id == (unsigned)id1)
					break;
				current = current->next;
			}
			if (current != NULL)
			{
				/* current now points to the killer */
				c2 = head;
				while (c2 != NULL)
				{
					if (c2->id == (unsigned)id2)
						break;
					c2 = c2->next;
				}
				if (c2 != NULL)
				{
					/* c2 now points to the killed */
					c2->velocity = 0;
					c2->speed = 0.0;
					c2->dead = 1;
					c2->power = MAX_HITS;
					
					if (c2 == our_node)
						guessed_power = MAX_HITS;

					current->kills++;
					addtext("*** %s(%d) fragged %s(%d)", current->nick, current->id,
						c2->nick, c2->id);
					
					explosion(c2->x, c2->y, 250, 10, makecol(255,0,0));
				}
			}
		}
		/* ****************************************
		   T <id> <msg> - a player messages
		   **************************************** */
		if (!strcmp(stuff, "T"))
		{
			char *ptr2;

			if (!(ptr2 = strchr(ptr, ' ')))
			{
				printff_direct("bla bla dood");
				terminate();
			}
			*ptr2 = '\0';
			ptr2++;

			/* ptr contains id, ptr2 nick */

			if (strlen(ptr2) > 80)
				*(ptr2+79) = '\0';

			current = head;
			while (current != NULL)
			{
				if (current->id == (unsigned int)atoi(ptr))
					break;
				current = current->next;
			}

			if (current != NULL)
				addtext("<%s> %s", current->nick, ptr2);
		}
		/* ****************************************
		   6 <id> <x> <y> <vel> <speed> - Another player changed velocity
		   and it's starting coordinates.
		   **************************************** */
		if (!strcmp(stuff, "6"))
		{
			int id, vel;
			double x, y, spd;
			char *ptr2 = ptr;
			unsigned long clag;

			/* extract some stuff + check for some errors */
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				id = atoi(ptr);
			} else { terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				x = atof(ptr2);
			} else { terminate(); }

			ptr++;
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				y = atof(ptr);
			} else { terminate(); }
			ptr2++;

			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				vel = atoi(ptr2);
			} else { terminate(); }
			ptr++;

			if (ptr != NULL)
				spd = atof(ptr);
			else 
				terminate();

			current = head;
			while ( (current != NULL) && (current->id != (unsigned int)id) )
				current = current->next;

			if (current == NULL)
				return 0;

#if IGNORE_LAG == 1
			clag = 0;
#else
			clag = (unsigned long)current_lag();
#endif
			
			current->freeze = 0;
			current->t = ourtime - clag;
			current->x = x;
			current->y = y;
			
			setsec(current);

			current->velocity = vel;
			current->vel_time = ourtime - clag;
			current->speed = spd;
			//addtext("Velocity ajusted: U:%d X:%f Y:%f VEL:%d SPD:%.2f", id, x, y, vel, spd);
		}
		/* ****************************************
		   7 <id> <x> <y> <deg> <vel> - Another player changed direction
		   and it's starting coordinates so we stay in sync.
		   **************************************** */
		if (!strcmp(stuff, "7"))
		{
			int id, deg, turn;
			double x, y;
			char *ptr2 = ptr;
			unsigned long clag;

			/* extract some stuff + check for some errors */
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				id = atoi(ptr);
			} else { alert("dabbe", "","","","",1,1); terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				x = atof(ptr2);
			} else { alert("c", "","","","",1,1); terminate(); }
			ptr++;

			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				y = atof(ptr);
			} else { alert("b", "","","","",1,1); terminate(); }
			ptr2++;

			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				deg = atof(ptr2);
			} else { alert("c222", "","","","",1,1); terminate(); }
			ptr++;

			if (ptr != NULL)
			{	turn = atoi(ptr);
			} else { alert("a", "","","","",1,1); terminate(); }

			current = head;
			while ( (current != NULL) && (current->id != (unsigned int)id) )
				current = current->next;
			
			if (current == NULL)
				return 0;

#if IGNORE_LAG == 1
			clag = 0;
#else
			clag = (unsigned long)current_lag();
#endif
			
			current->freeze = 0;
			current->t = ourtime - clag;
			current->turn = turn;
			current->turn_t = ourtime - clag;
			current->deg = deg;
			current->x = x;
			current->y = y;
		
			setsec(current);

			// addtext("Turn ajusted: U:%d X:%f Y:%f deg:%d Turn:%d", id, x, y, deg, turn);
		}
		/* ****************************************
		   8 <owner_id> <x> <y> <deg> - Bullit entered game
		   **************************************** */
		if (!strcmp(stuff, "8"))
		{
			int owner_id, deg;
			double x, y;
			char *ptr2 = ptr;
			unsigned long clag;

			/* extract some stuff + check for some errors */
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				owner_id = atoi(ptr);
			} else { terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				x = atof(ptr2);
			} else { terminate(); }

			ptr++;
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				y = atof(ptr);
			} else { terminate(); }
			ptr2++;
			if (ptr2 != NULL)
			{	deg = atof(ptr2);
			} else { terminate(); }

			current = head;
			while ( (current != NULL) && (current->id != (unsigned int)owner_id) )
				current = current->next;
			if (current == NULL)
			{	/* We think the owner of the bullit doesn't exist */
				//alert("glabba", "","","","",1,1);
				//terminate();
			}

#if IGNORE_LAG == 1
			clag = 0;
#else
			clag = (unsigned long)current_lag();
#endif
			
			// addtext("Creating bullit: Owner:%d X:%f Y:%f deg:%d", owner_id, x, y, deg);
			add_bullet(current, x, y, deg, ourtime - clag);
		}
		/* ****************************************
		   9 <id> <invincible> <duration> - Invincibility settings for player/bot
		   **************************************** */
		if (!strcmp(stuff, "9"))
		{
			int id, invincible, duration;
			char *ptr2 = ptr;
			double clag;

			/* extract some stuff + check for some errors */
			if (ptr2 = strchr(ptr, ' '))
			{	*ptr2 = '\0';
				id = atoi(ptr);
			} else { terminate(); }
			ptr2++;
			if (ptr = strchr(ptr2, ' '))
			{	*ptr = '\0';
				invincible = atoi(ptr2);
			} else { terminate(); }
			ptr++;
			if (ptr != NULL)
			{	
				duration = atoi(ptr);
			} else { terminate(); }

			current = head;

			while ( (current != NULL) && (current->id != (unsigned int)id) )
				current = current->next;

			if (current == NULL)
			{	
				return ret;
			}

#if IGNORE_LAG == 1
			clag = 0;
#else
			clag = current_lag();
#endif
			
			current->invincible = invincible;
			current->invincibility_t = duration;
			current->invincibility_t2 = ourtime + clag;
			
		}
		/* ****************************************
		   D <id> - Delete player from list
		   **************************************** */
		if (!strcmp(stuff, "D"))
		{
			unsigned int id;

			/* extract some stuff + check for some errors */
			if (ptr)
				id = atoi(ptr);
			else {
				alert("dabbe", "","","","",1,1); 
				terminate(); 
			}

			current = head;
			while ( (current != NULL) && (current->id != (unsigned int)id) )
				current = current->next;
			
			if (current != NULL)
			{
				explosion(current->x, current->y, 500, 10, makecol(255, 128, 255));
				del_player(id);
			}
		}
	} 
	else 
	{
		/* ****************************************
		   H (no args) - WE GOT HIT! :P
		   **************************************** */
		if (!strcmp(stuff, "H"))
		{
			if (our_node->power > 0)
				our_node->power--;
		}
		/* ****************************************
		   R - ready signal.. we are allowed to join now 
		   **************************************** */
		if (!strcmp(stuff, "R"))
		{
			printff_direct("You can now press enter to show the field");
			STARTED = 1;
			ret = 1;
		}
	}

	return ret;
}