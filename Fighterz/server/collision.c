#include "common.h"

/********************************************
*  COLLISION CHECKS
****************************************************/


void collidecheck()
{
	LINK current;
	current = head;
	while (current!= NULL)
	{
		collidecheck2(current);
		current = current->next;
	}
}

void collidecheck2(struct data *ptr)
{
	LINK current = head;
	int collided = 0; /* 0=no, 1=yes */
	double x, y;
	int px, py, qx, qy;

	while (current != NULL)
	{
		if ((struct data *)current == ptr)
			break;
		current = current->next;
	}

	x = futureX(current);
	y = futureY(current);	

	px = (int)(x / BLOCKSIZE);
	py = (int)(y / BLOCKSIZE);

	if ( !(px > X_BLOCKS) && !(px < 0) && 
		!(py > Y_BLOCKS) && !(py < 0) )
	{

		/* center of the ship */
		//if (field[py][px] == 1)
		//	collided = true;

		/* BLOCKSIZE / 4 down the ship */
		if ((py - 1) > 0)
		{
			qy = (int) ((y - (BLOCKSIZE / 4)) / BLOCKSIZE);
			if (field[qy][px] == '1') collided = 1;
		}
		/* BLOCKSIZE / 4 above the ship */
		if ( (py + 1) <= Y_BLOCKS )
		{
			qy = (int)( (y + (BLOCKSIZE / 4) ) / BLOCKSIZE);
			if (field[qy][px] == '1') collided = 1;
		}
		/* BLOCKSIZE / 4 on the right of the ship */
		if ( (px + 1) <= Y_BLOCKS )
		{
			qx = (int)( (x + (BLOCKSIZE / 4) ) / BLOCKSIZE);
			if (field[py][qx] == '1') collided = 1;
		} 
		/* BLOCKSIZE / 4 on the left of the ship */
		if ( (px - 1) >= 0 )
		{
			qx = (int)( ( (x + (BLOCKSIZE / 4) ) / BLOCKSIZE) - 0.5); // dunno why i had to add the - 0.5
																	// in the VB version it wasn't necessary
			if (field[py][qx] == '1') {
				collided = 1;
			}
		}
	}

	if (collided == 1)
	{
		current->velocity = 0;
		// current->speed = 0.0;
	}
}

double futureX(struct data *ptr)
{
	int radius;
	double ret;
	double pos_x;

	radius = BLOCKSIZE / 2;
	ret = (PI / 180) * (ptr->deg - 90);

	pos_x = cos(ret);
	pos_x = pos_x * ptr->speed;
	pos_x = pos_x + radius;
	pos_x = pos_x + ptr->x - (BLOCKSIZE / 2);

	return pos_x;
}

double futureY(struct data *ptr)
{
	int radius;
	double ret;
	double pos_y;

	radius = BLOCKSIZE / 2;
	ret = (PI / 180) * (ptr->deg - 90);

	pos_y = sin(ret);
	pos_y = pos_y * ptr->speed;
	pos_y = pos_y + radius;
	pos_y = pos_y + ptr->y - (BLOCKSIZE / 2);

	return pos_y;
}

int collidecheck2b(struct data *ptr)
{
	LINK current;
	int collided = 0; /* 0=no, 1=yes */
	double x, y;
	int px, py;
	int retval = 0;

	current = head;
	while (current != NULL)
	{
		if ((struct data *)current == ptr)
			break;
		current = current->next;
	}

	x = futureX(current);
	y = futureY(current);	

	px = (int)(x / BLOCKSIZE);
	py = (int)(y / BLOCKSIZE);

	if ( !(px > X_BLOCKS) && !(px < 0) && 
		!(py > Y_BLOCKS) && !(py < 0) )
	{
		/* center of the bullet */
		if (field[py][px] == '1')
			collided = 1;
	}

	if (collided == 1)
	{
#if DEBUG == 1
		fprintf(outp, "deletingc bullet(%d) at addr: %d\n", current->id, current);fflush(stdout);
#endif
		if (current->owner != NULL)
			current->owner->bulletcnt--;
		del_bullet(current);

		retval = 1;
	}

	return retval;
}

#if 0 == 1
// bullet/ship collide check
void bs_collidecheck()
{
	LINK tmp = head;

	while (tmp != NULL)
	{
		if (tmp->bullet != 1)
		{	
			int i;

			LINK tmp2 = head;
			LINK old = NULL;
			/* Check if this ship hit a bullet */
			while (tmp2 != NULL)
			{
				i = 0;
				if ( (tmp2->bullet == 1) && (tmp2->owner != tmp) )
				{
					/* Compare coordinates with this ship */
					if ( (dabs(tmp2->x - (tmp->x) ) <= (BLOCKSIZE / 4)) &&
						 (dabs(tmp2->y - (tmp->y) ) <= (BLOCKSIZE / 4)) )
					{
						tmp2->owner->bulletcnt--;
						/* Inform this user that he got hit ;) */
						if (tmp->bot != 1)
							sock__NOTUSED__write(tmp->id, "H\n");

						fprintf(outp, "deletingd bullet(%d) at addr: %d\n", tmp2->id, tmp2);fflush(stdout);
						del_bullet(tmp2);

						tmp->power = tmp->power - 1;
						if (old == NULL)
							tmp2 = head;
						else
							tmp2 = old;
						old = NULL;
						i = 1;
					}
				}
				if (i == 0)
				{
					old = tmp2;
					tmp2 = tmp2->next;
				}
			}
		}
		tmp = tmp->next;
	}

}
#endif

// bullet/ship collide check
int bs_collidecheck(struct data *ptr) 
{
	//LINK tmp = head;
	LINK tmp = ptr;
	int retval = 0;
	// while (tmp != NULL)
	if (tmp != NULL)
	{
		if (tmp->bullet == 1)
		{	
			int i;

			//if ( (ourtime - tmp->hit_t) >= 200)
			//	tmp->impact = 0;

			LINK tmp2 = head;
			/* Check if this ship hit a bullet */
			while (tmp2 != NULL)
			{
				i = 0;
				if ( (tmp2->bullet != 1) && (tmp->owner != tmp2) && (tmp->owner->bullet != 1) && 
					 (tmp2->dead != 1) && (tmp2->invincible == 0) )
				{
					/* Compare coordinates with this ship */
					if ( (dabs(tmp2->x - (tmp->x) ) <= (double)(BLOCKSIZE / 4)) &&
						 (dabs(tmp2->y - (tmp->y) ) <= (double)(BLOCKSIZE / 4)) )
					{
//						addtext("del bullet at addr: %d P:%s hit P:%s", tmp, tmp->owner->nick, tmp2->nick);
						//sock__NOTUSED__write(tmp2->id, "H\n");
						send_hit(EVERYONE, NULL, tmp2->id, tmp->id, (unsigned char)tmp2->power);
						/* Ajust scores */
						tmp2->power--;
						tmp->owner->bulletcnt--;
						if (tmp2->power <= 0)
						{
							/* tmp2 is dood */
							tmp2->velocity = 0;
					        tmp2->speed = 0.0;
							tmp2->died = servertime;
							tmp2->dead = 1;
							tmp2->power = MAX_HITS;
							tmp2->x = -100.0;
							tmp2->y = -100.0;

							tmp->owner->frags++;
							printf_("*** %s(%d)(%d) got killed by: %s(%d)\n", tmp->owner->nick, tmp->owner->id, tmp->owner->bullet, tmp2->nick, tmp2->id);
							//soc__NOTUSED__kwrite_all("K %d %d\n", tmp->owner->id, tmp2->id);
							send_kill(EVERYONE, NULL, tmp2->id, tmp->owner->id, ""); /* "" = '\0' = default kill text */
						}

						del_bullet(tmp);
						
						retval = 1;

						/* if (our_node == tmp2)
							guessed_power--; 

						tmp2->impact = 1;
						tmp2->hit_t = ourtime; */

						i = 1;
					}
				}
				if (i == 0)
					tmp2 = tmp2->next;
				else
					return retval;
			}
		}
		//tmp = tmp->next;
	} else {
		/* alert("error 1", "","","","",1,1);
		terminate(); */
	}

	return retval;
}

/* used in drawships() for bots cannon :) */
double fbX(unsigned int id2)
{
	int radius;
	double direction; 
	double ret;
	double pos_x;
	LINK current = head;

	while (current != NULL)
	{
		if ((unsigned int)current->id == id2)
		{
			radius = BLOCKSIZE / 2;
			ret = (PI / 180) * (current->deg - 90);

			direction = 7; /* pixzls to move :) */

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

double fbY(unsigned int id2)
{
	int radius;
	double direction; 
	double ret;
	double pos_y;
	LINK current = head;

	while (current != NULL)
	{
		if ((unsigned)current->id == id2)
		{
			radius = BLOCKSIZE / 2;
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


/* 2 following functions are Used in valid_target() */
double vtX(double x, double deg)
{
	int radius;
	double direction; 
	double ret;
	double pos_x;

	radius = BLOCKSIZE / 2;
	ret = (PI / 180) * (deg - 90);

	direction = (double)((BLOCKSIZE - 1) / 4);
	//direction = 1;

	pos_x = cos(ret);
	pos_x = pos_x * direction;
	pos_x = pos_x + radius;
	pos_x = pos_x + x - (BLOCKSIZE / 2);

	return pos_x;

}
/* idem dito */
double vtY(double y, double deg)
{
	int radius;
	double direction; 
	double ret;
	double pos_y;

	radius = BLOCKSIZE / 2;
	ret = (PI / 180) * (deg - 90);

	direction = (double)((BLOCKSIZE - 1) / 4);
	//direction = 1;

	pos_y = sin(ret);
	pos_y = pos_y * direction; /* ? */
	pos_y = pos_y + radius;
	pos_y = pos_y + y - (BLOCKSIZE / 2);

	return pos_y;
}
