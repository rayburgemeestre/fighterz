#include "common.h"

/********************************************
*  PARSE FUNCTIONS
****************************************************/

/* TODO: rewrite ( al een keer in een bot ofzo gunstige parse functie gecood ) */
/** The packet handling routine.
 * This function does the packet handling, it puts the data
 * together and hands it over to parse2.
 * @see parse2()
 * @param sock_id the socket filedescriptor
 * @param stuff the bytes received
 * @param nbytes length of bytes received
 */
#if 1 == 2
void parse(int sock_id, char *stuff, int nbytes)
{   char *str, *tussen, *p1;
	int i=0,j=0;
	LINK current = head;
	stuff[sizeof(char) * nbytes] = '\0';

	while ( (current != NULL) && ((signed)current->id != sock_id))
		current = current->next;
	if (current == NULL)
	{
		printf("beetje gaar!~~\n");
		exit(-1);
	}
	
	/* enough memory for stuff & tmpstr */
	if (!(str = (char *)calloc((strlen(stuff)+1)+strlen(current->tmpstr), sizeof(char) )))
	{   perror("Error allocating memory");
		exit(-1);
	}

	strcpy(str, stuff);

	/* remove \r from stuff */
	if (!(tussen = (char *)calloc( ( strlen(str) + 1 ) + strlen(current->tmpstr), sizeof(char) )))
	{   perror("Error allocating memory");
		exit(-1);
	}
	p1 = tussen; /* because tussen gets fragged */
	
	while(i < (signed)(strlen(str)+1))
	{   if (str[i] != '\r') /* frag 'm */
			tussen[j++] = str[i];
		i++;
	}
	strcpy(str, tussen); 

	/* str is now \r-free */

	/* Old stuff needs to be inserted? */
	if (strlen(current->tmpstr)>0)
	{  
		strcat(current->tmpstr, str); /* put stuff from str behind tmpstr */
		strcpy(str, current->tmpstr); /* then put tmpstr back in str */
		current->tmpstr[0] = 0; /* 'emtpy' tmpstr */
	}

	while (strchr(str, '\n'))
	{  
		/* Keep copy */
		strcpy(tussen, str);
		/* Show line */
		parse2(sock_id, strtok(str,"\n"));
		tussen = strchr(tussen, '\n'); /* Screw that line */
		tussen++; /* We don't need the leading \n */
		/* Restore backup */
		strcpy(str, tussen);
	}

	if (strlen(str)>0) strcpy(current->tmpstr, str);
	
	free(str);
	free(p1);

	/* Nice place to check if somebody is ready! :) */
	if (current->enabled == READY_INT)
	{
		current->invincible = 1;
		current->invincibility_t = 5000;
		current->invincibility_t2 = servertime;

		sockwrite(current->id, "9 %u %d %d\n", current->id, current->invincible, current->invincibility_t);

		sockwrite(current->id, "R\n"); /* R means ready */
		current->enabled = 1;
		// send other users info on new player
		
		sendulist2(current->id);
		sockwrite_all("9 %u %d %d\n", current->id, current->invincible, current->invincibility_t);
	}

}
#endif

// i know the protocol is pretty stupid but i will when the game is finished
// replace it with something better..
void parse2(int sock_id, char *stuff)
{   
	LINK tmp, current;
	char *ptr, *ptr2;
	unsigned long tussen = 0;
	int diff = 0, i = 0;

	if (stuff == NULL)
		return;

	printf_("%lu RCVD[%d]: %s\n", servertime, sock_id, stuff);
//	addtext("%lu RCVD[%d]: %s\n", servertime, sock_id, stuff);
	fflush(stdout);


	/* sync db with this user */
	tmp = head;
	while (tmp != NULL && tmp->id != sock_id)
		tmp = tmp->next;

	if (strchr(stuff, ' '))
	{
		ptr = strchr(stuff, ' ');
		*ptr = '\0';

		/* ****************************************
		   1 <servertime> - PONG to calculate lag
		   **************************************** */
		if (!strcmp(stuff, "1"))
		{
			*ptr = ' ';
			ptr++;
			if (ptr == NULL)
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			/* calculate lag */
			sscanf(stuff, "1 %lu", &tussen);
			diff = (servertime - tussen); 
			diff = (int)( ( (float)diff / 2.0) + 0.5);
			
			if (diff < 0)
			{
				printf_("ILLEGAL 2\n");
				/* TODO: destroy player here (illegal data) */
			}

			sockwrite(sock_id, "2 %d %lu\n", diff, servertime);

			current = head;
			while (current != NULL && current->id != sock_id)
				current = current->next;

			while (i < 4)
			{
				current->lag[i] = current->lag[i+1];
				i++;
			}

			current->lag[i] = (double)diff;
		}

		/* ****************************************
		   N <nickname> - Clients request for a nickname
		   **************************************** */
		if (!strcmp(stuff, "N"))
		{
			ptr++;

			if (ptr == NULL)
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}

			if (strlen(ptr) > 64)
				*(ptr + 63) = '\0';
			
			/* ptr now contains the nick */

			/* update it in our db */
			current = head;
			while (current != NULL)
			{
				if (current->id == sock_id)
					break;
				current = current->next;
			}
			strcpy(current->nick, ptr);

			if (current->enabled == 1)
			{
				/* Send to all connected users */
				sockwrite_all("N %u %s\n", sock_id, ptr);
			} else {
				/* Only to the connecting user, because
				   other users don't know this one exists */
				sockwrite(sock_id, "N %u %s\n", sock_id, ptr);
				current->enabled++;
			}				
		}

		/* ****************************************
		   T <msg> - Message to all users :)
		   **************************************** */
		if (!strcmp(stuff, "T"))
		{
			char *t1, *t2;
			*ptr = ' ';
			ptr++;
			t1 = strchr(ptr, ' ');

			if (ptr == NULL)
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}

			if (!strcmp(ptr, ".god"))
			{
				tmp->invincible = 2;
				sockwrite_all("9 %u %d %d", 
					tmp->id, tmp->invincible, tmp->invincibility_t);

				return;
			}
			if (!strcmp(ptr, ".human"))
			{
				tmp->invincible = 0;
				sockwrite_all("9 %u %d %d", 
					tmp->id, tmp->invincible, tmp->invincibility_t);

				return;
			}
			if (!strcmp(ptr, ".addbot"))
			{
/*				add_bot();
				//asdfasdfvoid notify_of_newuser(struct data *client, struct data *except, struct data *current)
				LINK our_bot = getplayer_byid(bot_id - 1);
				notify_of_newuser(EVERYONE, our_bot);

				flyto((bot_id - 1), REDFLAG.x, REDFLAG.y); *//* hehe */
				return;
			}
			if (!strcmp(ptr, ".delall"))
			{
				LINK x;
again:
				x = head;
				while (x != NULL)
				{
					if (x->bot == 1)
					{
						sockwrite_all("D %d\n", x->id);
						del_player(x->id);
						goto again;
					}

					x = x->next;
				}
				return;
			}
			if (t1 != NULL)
			{
				*t1 = '\0';
				t2 = (t1 + 1);
				if (t2 != NULL)
				{
					if (!strcmp(ptr, ".nick"))
					{
						if (strlen(t2) > 1 && strlen(t2) < 64)
						{
							/* TODO: valid nick characters etc.. ;) */
							/* Change nickname */
							strcpy(tmp->nick, t2);
							sockwrite_all("N %d %s\n", tmp->id, t2);
							return;
						}
					}
				}
				*t1 = ' ';
			}

			if ( (strlen(ptr) > 0) && (strlen(ptr) <= MAX_CHARS_IN_SENTENCE))
				sockwrite_notall(sock_id, "T %u %s\n", sock_id, ptr);

		}

		/* ****************************************
		   2 <x> <y> <velocity> <speed> - Client's new velocity
		   with starting coordinates 
		   **************************************** */
		if (!strcmp(stuff, "2"))
		{
			double _x, _y, spd;
			int vel;
			unsigned long clag;

			if (tmp->dead == 1)
				return;

			*ptr = ' ';
			ptr++;

			if (ptr == NULL)
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			
			if (!(ptr2 = strchr(ptr, ' ')))
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			*ptr2 = '\0';
			ptr2++;
			_x = atof(ptr);
			
			if (!(ptr = strchr(ptr2, ' ')))
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			*ptr = '\0';
			ptr++;
			_y = atof(ptr2);

			if (!(ptr2 = strchr(ptr, ' ')))
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			*ptr2 = '\0';
			ptr2++;
			vel = atoi(ptr);

			if (ptr2 == NULL)
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			spd = atof(ptr2);

			printf_("Got x:%f y:%f vel:%d\n", _x, _y, vel, spd);
		//	addtext("Got x:%f y:%f vel:%d spd:%.2f\n", _x, _y, vel, spd);

#if IGNORE_LAG == 1
			clag = 0;
#else
			clag = (unsigned long)current_lag(sock_id);
#endif
			/* ajust our db */
			tmp->x = _x;
			tmp->y = _y;
			tmp->velocity = vel;
			tmp->vel_time = (servertime - clag);
			tmp->speed = spd;
			tmp->t = (servertime - clag);
			
			/* move ship to the position we think it SHOULD be */
			// printf_("Got x:%f y:%f \n", tmp->x, tmp->y);

#if IGNORE_LAG != 1
			moveship(tmp->id, servertime);
#endif
			// printf_("Sending x:%f y:%f \n", tmp->x, tmp->y);

			/* "6 <id> <x> <y> <velocity> \n"*/
			sockwrite_notall(sock_id, "6 %d %.2f %.2f %d %.2f\n", sock_id, tmp->x, tmp->y, vel, spd);
		}
		
		/* ****************************************
		   3 <x> <y> <deg> <direction> - Client's new direction
		   (left/right) with starting coordinates x and y
		   **************************************** */
		if (!strcmp(stuff, "3"))
		{
			double _x, _y;
			double _deg;
			int _turn;
			unsigned long clag;

			if (tmp->dead == 1)
				return;

			*ptr = ' ';
			ptr++;

			if (ptr == NULL)
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			
			if (!(ptr2 = strchr(ptr, ' ')))
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			*ptr2 = '\0';
			ptr2++;
			_x = atof(ptr);
			
			if (!(ptr = strchr(ptr2, ' ')))
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			*ptr = '\0';
			ptr++;
			_y = atof(ptr2);

			if (!(ptr2 = strchr(ptr, ' ')))
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			*ptr2 = '\0';
			ptr2++;
			_deg = atof(ptr);			
			
			if (ptr2 == NULL)
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}

			_turn = atoi(ptr2);

			// printf_("Got x:%f y:%f deg:%d turn:%d\n", _x, _y, _deg, _turn);
			
#if IGNORE_LAG == 1
			clag = 0;
#else
			clag = (unsigned long)current_lag(sock_id);
#endif			

			/* ajust our db */
			tmp->x = _x;
			tmp->y = _y;
			tmp->deg = _deg;
			tmp->turn = _turn;
			tmp->t = (servertime - clag);
			tmp->turn_t = (servertime - clag);

			
			/* "7 <id> <x> <y> <deg> <turn> \n"*/
#if IGNORE_LAG != 1
			moveship(tmp->id, servertime);
#endif
			sockwrite_notall(sock_id, "7 %d %.2f %.2f %.2f %d\n", sock_id, tmp->x, tmp->y, tmp->deg, tmp->turn);
		}
		/* ****************************************
		   4 <x> <y> <deg> <direction> - Bullet created :)
		   **************************************** */
		if (!strcmp(stuff, "4"))
		{
			double _x, _y;
			double _deg;
			unsigned long clag;
			struct data *_ptr;

			if (tmp->dead == 1)
				return;
			if (tmp->invincible == 1) /* 1= invincible at start */
				return;

			*ptr = ' ';
			ptr++;

			if (ptr == NULL)
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			
			if (!(ptr2 = strchr(ptr, ' ')))
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			*ptr2 = '\0';
			ptr2++;
			_x = atof(ptr);
			
			if (!(ptr = strchr(ptr2, ' ')))
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			*ptr = '\0';
			ptr++;
			_y = atof(ptr2);

			if (ptr == NULL)
			{
				printf_("ILLEGAL 1\n");
				/* TODO: destroy player here (illegal data) */
			}
			_deg = atof(ptr);

			// printf_("Got2 x:%f y:%f deg:%d owner:%d\n", _x, _y, _deg, sock_id);
			
#if IGNORE_LAG == 1
			clag = 0;
#else
			clag = (unsigned long)current_lag(sock_id);
#endif			
			
			current = head;
			while ( (current != NULL) && (current->id != sock_id) )
				current = current->next;

			
			if (current == NULL)
			{
				printf_("DKFJSLKDJFLKSJDFKSDF\n");
				exit(-1);
			}
			
			if (current->bulletcnt == (unsigned)BULLET_MAX)
			{
				LINK oldestbullet = NULL;
				LINK current2;
				/* remove oldest bullet */
				current2 = head;
				while (current2 != NULL)
				{
					if (current2->bullet == 1)
						if (current2->owner == current)
							oldestbullet = current2;
					
					current2 = current2->next;
				}
				if (oldestbullet == NULL)
				{
					printf_("dit kan niet\n");
					exit(-1);
				}
			
				oldestbullet->owner->bulletcnt--;
#if DEBUG == 1
 				fprintf(outp, "deletingasdf bullet(%d) at addr: %d\n", oldestbullet->id, oldestbullet);fflush(stdout);
#endif
				del_bullet(oldestbullet);
			}

    		
			
			_ptr = add_bullet(current, _x, _y, _deg, servertime - clag);

			/* "8 <id> <x> <y> <deg>\n"*/

#if IGNORE_LAG != 1
			// moveship(tmp->id, servertime);
			if (!movebullet(_ptr, servertime)) /* if movebullet returned 1 then bullet is already gone */
#endif
				sockwrite_notall(sock_id, "8 %d %.2f %.2f %.2f\n", sock_id, _ptr->x, _ptr->y, _ptr->deg);
		}
	}
}
