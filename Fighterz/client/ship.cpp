#include "common.h"

/********************************************
*  SHIP FUNCTIONS
****************************************************/

void drawships() 
{
int color;
/* BLOCKSIZE = diameter */
double radius;	
double ret;
double pos_x, pos_y; /* ship's x,y */
double left_wing_deg, right_wing_deg; 
double lw_x, lw_y; /* leftwing x,y */
double rw_x, rw_y; /* rightwing x,y */

//	double fill_x, fill_y;

	radius = BLOCKSIZE_2 / 2;

	current = head;
	while (current != NULL)
	{
		if (current->bullet != 1 && current->dead != 1)
		{
			if (current->impact == 1)
				color = makecol(255, 0, 0); /* red */
			else if (current->dead == 2)
				color = makecol(128, 128, 128); /* grey */
			else if (current == our_node)
				color = makecol(255, 255, 255); /* white */
			else
				color = makecol(0, 255, 128); /* green */

			if (current->bot == 1)
			{
				ret = (PI / 180) * (current->deg - 90);
				pos_x = fbX(current->id);
				pos_y = fbY(current->id);

				circle(shipbuff, current->x, current->y, BLOCKSIZE / 4, color);
				line(shipbuff, current->x, current->y, pos_x, pos_y, color);

 				/* Also draw them on radar */
				circlefill(RADAR, (INDICATOR_WIDTH * 2) + (INDICATOR_DISTANCE_BETWEEN * 2) + current->x / RADAR_SCALE, current->y / RADAR_SCALE, 1, color);
			} else {

				/**********************
				 **   CALCULATIONS   **
				 **********************/

				/* ship coordinates */
				ret = (PI / 180) * (current->deg - 90);

				pos_x = cos(ret);
				pos_x = pos_x * (radius - (BLOCKSIZE_2 / 4));
				pos_x = pos_x + radius;
				pos_x = pos_x + current->x2 - (BLOCKSIZE_2 / 2);

				pos_y = sin(ret);
				pos_y = pos_y * (radius - (BLOCKSIZE_2 / 4));
				pos_y = pos_y + radius;
				pos_y = pos_y + current->y2 - (BLOCKSIZE_2 / 2);

		/*		fill_x = cos(ret);
				fill_x = fill_x * (radius - (BLOCKSIZE_2 / 2)+1);
				fill_x = fill_x + radius;
				fill_x = fill_x + current->x2 - (BLOCKSIZE_2 / 2);
				fill_y = sin(ret);
				fill_y = fill_y * (radius - (BLOCKSIZE_2 / 2)+1);
				fill_y = fill_y + radius;
				fill_y = fill_y + current->y2 - (BLOCKSIZE_2 / 2);				

				/* wing coordinates */
				left_wing_deg = current->deg + 120;
				right_wing_deg = current->deg - 120;

				/* left wing */
				ret = (PI / 180) * (left_wing_deg - 90);
				lw_x = cos(ret);
				lw_x = lw_x * radius;
				lw_x = lw_x + radius;
				lw_x = lw_x + current->x2 - (BLOCKSIZE_2 / 2);

				lw_y = sin(ret);
				lw_y = lw_y * radius;
				lw_y = lw_y + radius;
				lw_y = lw_y + current->y2 - (BLOCKSIZE_2 / 2);

				/* right wing */
				ret = (PI / 180) * (right_wing_deg - 90);
				rw_x = cos(ret);
				rw_x = rw_x * radius;
				rw_x = rw_x + radius;
				rw_x = rw_x + current->x2 - (BLOCKSIZE_2 / 2);

				rw_y = sin(ret);
				rw_y = rw_y * radius;
				rw_y = rw_y + radius;
				rw_y = rw_y + current->y2 - (BLOCKSIZE_2 / 2);

				/**********************
				 **     DRAW SHIP    **
				 **********************/
				
				// polygon test
				{
					int points[8] = {
						pos_x, pos_y,
						lw_x, lw_y,
						current->x2, current->y2,
						rw_x, rw_y
					};
					int *p = points;

					polygon(shipbuff, 4, p, makecol(0, 0, 0));
				}
				/* ship base */
				// line(shipbuff , current->x2, current->y2, pos_x, pos_y, color);
				/* left wing */
				line(shipbuff , pos_x, pos_y, lw_x, lw_y, color);
				line(shipbuff , current->x2, current->y2, lw_x, lw_y, color);
				/* right wing */
				line(shipbuff , pos_x, pos_y, rw_x, rw_y, color);
				line(shipbuff , current->x2, current->y2, rw_x, rw_y, color);

				if (RADAR_SHOW == 1)
					/* Also draw them on radar */
					circlefill(RADAR, (INDICATOR_WIDTH * 2) + (INDICATOR_DISTANCE_BETWEEN * 2) + current->x / RADAR_SCALE, current->y / RADAR_SCALE, 1, color);
				
			}
            if (current->invincible == 1)
			    rect(shipbuff, current->x - (BLOCKSIZE / 2), current->y - (BLOCKSIZE / 2), 
			    	current->x + (BLOCKSIZE / 2), current->y + (BLOCKSIZE / 2), makecol(255,0,0));				
            else if (current->invincible == 2)
			    rect(shipbuff, current->x - (BLOCKSIZE / 2), current->y - (BLOCKSIZE / 2), 
			    	current->x + (BLOCKSIZE / 2), current->y + (BLOCKSIZE / 2), makecol(255,128,0));				

			if (show_names == 1)
			{
				int _bullets = ((BULLET_MAX - BULLET_COUNT) * 100) / BULLET_MAX;
				char tmpstr[128];
				// sprintf(tmpstr, "%s [%d] [%2.2f] [%d]", current->nick, current->velocity, current->speed, current->freeze);
				sprintf(tmpstr, "%s", current->nick);
				textprintf_centre(shipbuff, font, current->x, 
					current->y + BLOCKSIZE, makecol(192,192,192), tmpstr);
			}

		}
		/* next ship */
		current = current->next;
	}
}

void moveships()
{
	current = head;
	while (current != NULL)
	{

		/* Nice moment to check if invinsibility should be turned off :) */
		if (current->invincible == 1)
		{
			int diff;
			diff = ourtime - current->invincibility_t2;
			if (diff > (signed)current->invincibility_t)
			{
				/* No longer invincible */
				current->invincible = 0;
			}
		}

		/* this function does not handle bullets 
		   i split ships/bullets in two seperate functions */
		if (current->bullet != 1)
		{
		//	if ( (current->velocity == 1) || (current->velocity == 2) ||
		//		 (current->velocity == -1) || (current->velocity == -2) || (current->velocity == 0) )
		//	{
				moveship(current->id, ourtime);
		//	}

			/* something to remove the 'red' glow of hit ships after 200 ms */
			if ( (ourtime - current->hit_t) >= 200)
			current->impact = 0;
		}
		current = current->next;
	}
}
void moveship(unsigned int id2, unsigned long t2)
{
	long int diff = 0, cnt;
	long int times = 0;
	double tmp = 0;
	int radius;
	double ret;
	double pos_x, pos_y;

	current = head;

	while (current != NULL)
	{
		if (current->id == id2)
		{
			if (current->bullet == 1)
				return;

			if (current == our_node)
			{
				diff = (ourtime - our_node->turn_t);
				tmp = 10; /* Turn speed */
				while (diff >= tmp)
				{
					diff -= tmp;
					our_node->turn_t += tmp;
					times += 1;
				}

				/* added: */
				/* for our ship */
				for (cnt=0; cnt<times; cnt++)
				{
					if (current->turn == 0) {
						our_node->turn_t = ourtime + diff;
						/* Todo? break; Nothing to do anyway */
					} else
					if (current->turn > 0) {
						our_node->deg += our_node->turn;
						if (our_node->deg > 360) {
							our_node->deg = our_node->turn;
						}
					} else
					if (current->turn < 0) {
						our_node->deg += our_node->turn;
						if (our_node->deg < 0) {
							our_node->deg = 358; /* todo: 360 - our_node->deg ? */
						}
					}
				}

			}

			diff = (ourtime - current->vel_time);
			/*if (current == our_node)
				addtext("dbg, diff: %d", diff); */
			tmp = 50;
			times = 0;
			while (diff >= tmp)
			{
				diff -= tmp;
				current->vel_time += tmp;
				times += 1;
			}

			for (cnt=0; cnt<times; cnt++)
			{
				if (current->velocity == 1)
				{ 
					if (current->speed < SPEED)
						current->speed += 0.01;
				}
				else if (current->velocity == 0.00)
				{
					if (current->speed < -0.001)  // we don't want -0.00 to valid this statement
					{
						current->speed += 0.01;
						if (current->speed == 0.00)
							current->speed = 0.00;
					}
					else if (current->speed > 0.001) // ***
						current->speed -= 0.01;
					/* 
						*** De doubles zijn nogal gaar, intern afgerond ofzo?
						0.0000 is tegenwoordig al groter dan 0.00 ofzo
						dus ehm, vandaar die vage extra 0.001 steeds.....
					*/
				}
				else if (current->velocity == -1)
				{
					if (current->speed > (-1 * SPEED))
						current->speed -= 0.01;
				}
				else
					; //our_node->vel_time += diff;
				/*
				if (current->velocity == 0)
				{
					if (current->speed == -0.01)
					{
						current->speed = 0;
					}
					else
					{
						if (current->speed > 0.00)
						{
							addtext("%.2f -> 1", current->speed);
							current->speed -= 0.01;
						}
						else if (current->speed < -0.01)
						{
							addtext("%.2f -> 2", current->speed);
							current->speed += 0.01;
						}
					}
				} */
			}

			times = 0;
			tmp = 0;
		
			/*
					HIER ZIT NOG EEN FOUT IN <-- niet meer denk :P  */


			diff = (ourtime - current->turn_t);
			tmp = 10;
			while (diff >= tmp)
			{
				diff -= tmp;
				current->turn_t += tmp;
				times += 1;
			}

			// for the other ships
			for (cnt=0; cnt<times; cnt++)
			{
				switch (current->turn)
				{
				case 2:
					current->deg += current->turn;
					if ( (current->deg == 362) || (current->deg == 364) )
						current->deg = current->turn;
					break;
				case 4:
					current->deg += current->turn;
					if ( (current->deg == 362) || (current->deg == 364) )
						current->deg = current->turn;
					break;
				case -2:
					current->deg += current->turn;
					if ( (current->deg == -2) || (current->deg == -4) )
						current->deg = 358;
					break;
				case -4:
					current->deg += current->turn;
					if ( (current->deg == -2) || (current->deg == -4) )
						current->deg = 356;
					break;
				case 0:
					current->turn_t = ourtime + diff;
					break;
				default:
					// shouldn't be here
					//alert("i must die", "","","","",1,1);
					//terminate();
					break;
				}
			}
			/*	*/
			/* end */


			times = 0;
			tmp = 0;

			/* move this ship*/
			if (current->t > t2)
			{	/* time is not synched correctly */
				/* todo: call function to sync again*/
				return;
			}
			diff = (ourtime - current->t);
			
			if (current->freeze == 1)
			{
				current->t = ourtime + diff;
				return;
				/* we don't move because we THINK (and are probably right)
				   that this person has collided with some wall.. */
			}

		
			/*
			if ( (current->velocity == 1) || (current->velocity == -1) )
			{
				//tmp = MOVE_INTERVAL * 2;
				tmp = 2;
			} else if ((current->velocity == 2) || (current->velocity == -2)) 
			{
				//tmp = MOVE_INTERVAL;
				tmp = 1;
			} else {
				current->t = ourtime + diff;
				return;
			} 
			
			  */
			//if (current->speed == -0.01 || current->speed == -0.00 || current->speed == -0.00 ) // whatever
			//{
			//	current->t = ourtime + diff;
			//	return;
			//}
			
			tmp = 1;

			while (diff >= tmp)
			{
				diff -= tmp;
				current->t += tmp;
				// times += ( MOVE_STEPW * ( BLOCKSIZE / DEF_BLOCKSIZE ) );
				times += MOVE_STEPW;
			}

			radius = BLOCKSIZE / 2;
			ret = (PI / 180) * (current->deg - 90);

			for (cnt=0; cnt<times; cnt++)
			{
				int cret = 0;
				/* modifyed this */

				//if (current->velocity == 0) break;

				//if ((current->velocity) < 0)
				//{
				//	direction = current->speed * -1; /* pixzls to move :) */
				//} else {
				//	direction = current->speed;
				//}

				pos_x = futureX(current, current->speed);
				pos_y = futureY(current, current->speed);

				current->x = pos_x;
				current->y = pos_y;
				
				//ret = collidecheck2(current->id, 1, 0); /* OLD: stond bovenin dit blok */
				
				/* did the ship fly out the field? */
				if (current->y < 0) 
				{	// *warp*
					current->y = field_height - 1;
				} else if (current->y > field_height) {
					current->y = 1;
				} else if (current->x < 0) {
					current->x = field_width - 1;
				} else if (current->x > field_width) {
					current->x = 1;
				}				
				
				setsec(current);

				//cret = 
				collidecheck2(current->id, 0, 0); 
				/*if (cret == 1)
				{
					if (our_node == current)
					{
						our_node->velocity = 0;
					}
				}*/
			}
			return;
		}
		current = current->next;
	}
}

void setrcoords()
{
	current = head;
	while (current != NULL)
	{
		if (ZOOM > 0.0)
		{
			current->x2 = current->x * (1.0 + ZOOM);
			current->y2 = current->y * (1.0 + ZOOM);
		}
		else if (ZOOM < 0.0)
		{
			current->x2 = current->x / (1.0 - ZOOM);
			current->y2 = current->y / (1.0 - ZOOM);
		}
		else 
		{
			current->x2 = current->x;
			current->y2 = current->y;
		}
		current = current->next;
	}
}


/* used in drawships() for bots cannon :) */
double fbX(unsigned int id2)
{
	int radius;
	double direction; 
	double ret;
	double pos_x;

	current = head;

	while (current != NULL)
	{
		if (current->id == id2)
		{
			radius = BLOCKSIZE / 2;
			ret = (PI / 180) * (current->deg - 90);

			direction = 7;

			pos_x = cos(ret);
			pos_x = pos_x * direction; /* ? */
			pos_x = pos_x + radius;
			pos_x = pos_x + current->x - (BLOCKSIZE / 2);

			return pos_x;
		}
		current = current->next;
	}
	return current->x;
}
/* idem dito */
double fbY(unsigned int id2)
{
	int radius;
	double direction; 
	double ret;
	double pos_y;

	current = head;

	while (current != NULL)
	{
		if (current->id == id2)
		{
			radius = BLOCKSIZE / 2.5;
			ret = (PI / 180) * (current->deg - 90);

			direction = 7;

			pos_y = sin(ret);
			pos_y = pos_y * direction; /* ? */
			pos_y = pos_y + radius;
			pos_y = pos_y + current->y - (BLOCKSIZE / 2);

			return pos_y;
		}
		current = current->next;
	}
	return current->y;
}


void explosion(double x, double y, int maxrad, int speed, int color)
{
	/* create first node in linked list */
	if (!(e_new_node = (LINK2)malloc(sizeof(_explosion))))
	{   printff_direct("Error allocating memory");
		exit(-1);
	}

	/* insert the created node */
	e_new_node->next = e_head;
	e_head = e_new_node;

	/* intiate some values */
	e_head->x = x;
	e_head->y = y;
	e_head->color = color;
	e_head->radius = 0;
	e_head->cradius = 0;
	e_head->maxradius = maxrad;
	e_head->speed = speed; /* increase every x millisecs */
	e_head->t = ourtime;

//	addtext("Created node addr: %d", e_head);

}

int rexplosion(struct data2 *ptr)
{
	LINK2 old = NULL;
	LINK2 future = NULL;
	LINK2 current = e_head;

	while (current != NULL)
	{
		future = current->next;
		if (current == ptr)
		{
			if (current == e_head)
			{
				/* first node */
				future = current->next;
				free(current);
				e_head = future;
				return 1;
			} else {
				if (current->next != NULL)
				{
					/* mid node */
					free(current);
					old->next = future;
					return 1;
				} else {
					/* last node */
					free(current);
					old->next = NULL;
					return 1;
				}
			}
		}
		old = current;
		current = current->next;
	}
	return 0;
}

void drawexplosions() 
{
	int diff, tmp, times, cnt;
	LINK2 current;
	current = e_head;

	while (current != NULL)
	{
		/* Also check here if radius should change */
			diff = (ourtime - current->t);
			
			tmp = current->speed;
			times = 0;
			while (diff >= tmp)
			{
				diff -= tmp;
				current->t += tmp;
				times++;
			}

			for (cnt=0; cnt<times; cnt++)
				current->radius++;

			if (current->radius >= current->maxradius)
			{
				rexplosion(current);
				current = e_head; /* again, easy easy.. TODO: change this 
								     pathatic solution */
				continue;
			}

		circle(shipbuff, current->x, current->y, current->radius, current->color);
		current = current->next;
	}
}