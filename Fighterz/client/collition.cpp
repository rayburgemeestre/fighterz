#include "common.h"

/********************************************
*  COLLISION CHECKS
****************************************************/


void collidecheck()
{
	current = head;
	while (current!= NULL)
	{
		if (current->bullet == 1)
		{
			/* collide check for bullets ;) */
			/* TODO: */
			// collidecheck2b(current);
		} else {
			collidecheck2(current->id, 0);
		}
		current = current->next;
	}
}

int collidecheck2(unsigned int id, int variation) /* variation = only 1 when 
								called in mainloop() at UP/DOWN stuff */
{
	LINK current2;
	int collided = 0; /* 0=no, 1=yes */
	double x, y;
	int px, py, qx, qy;
	/* the fucked up stuff */
	double direction;
	int step = 1; /* Will only use this nonsense temporary */

	current2 = head;
	while (current2 != NULL)
	{
		if (current2->id == id)
			break;
		current2 = current2->next;
	}

	/* TODO: This is weird, change this */
    if (current2->speed != 0.00)
        if (current2->speed > -0.01)
            direction = step;
        else
            direction = step * -1;
    else
        if (direction_is_up)
            direction = step;
        else
            direction = step * -1;

	x = futureX(current2, direction);
	y = futureY(current2, direction);

//	addtext("After x:%.2f y:%.2f", x, y);

	px = (int)(x / BLOCKSIZE);
	py = (int)(y / BLOCKSIZE);

	if ( !(px > X_BLOCKS) && !(px < 0) && 
		!(py > Y_BLOCKS) && !(py < 0) )
	{

		/* center of the ship */
		if (field[py][px] == '1')
			collided = 1;

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
			if (field[py][qx] == '1') {								// TODO; find out why :)
				collided = 1;
			}
		}
	}

	if (collided == 1)
	{
		if (current2->id == our_id)
		{	/* our ship */
			if (current2->speed != 0.0)
			{	
				//addtext("C: collided at velocity: %d", our_node->velocity);
				//addtext("Set To 0");
				current2->velocity = 0;
				current2->speed = 0.0;
				// current2->freeze = 1;
				if (variation == 0)
					send_accel(4);
			}			
		} else {
			/* other ship */
			/* mark as "do not move" */
			current2->freeze = 1;
			/* but don't modify velocity */
		}		
	}
	return collided;
}


double futureX(struct data *ptr, double speed)
{
    int radius;
    double ret;
    double pos_x;


    radius = BLOCKSIZE / 2;
    ret = (PI / 180) * (ptr->deg - 90);

    pos_x = cos(ret);
    pos_x = pos_x * speed;
    pos_x = pos_x + radius;
    pos_x = pos_x + ptr->x - (BLOCKSIZE / 2);

    /* Hmzz */
    if (pos_x < 0) {
        pos_x = field_width - 1;
    } else if (pos_x > field_width) {
        pos_x = 1;
    }

    return pos_x;
}

double futureY(struct data *ptr, double speed)
{
	int radius;
	double ret;
	double pos_y;

	radius = BLOCKSIZE / 2;
	ret = (PI / 180) * (ptr->deg - 90);

	pos_y = sin(ret);
	pos_y = pos_y * speed;
	pos_y = pos_y + radius;
	pos_y = pos_y + ptr->y - (BLOCKSIZE / 2);

	/* .. :) */
	if (pos_y < 0) 
	{	// *warp*
		pos_y = field_height - 1;
	} else if (pos_y > field_height) {
		pos_y = 1;
	} 

	return pos_y;
}




// bullet

int collidecheck2b(struct data *ptr)
{
	LINK current2;
	int collided = 0; /* 0=no, 1=yes */
	int err = 0;
	double x, y;
	// int qx, qy;
	int px, py;
	int old_px, old_py;
	int retval = 0;
	int a,b,c,d;
	int direction = -1; /* 1 is degrees ++, 0 --
	                  -1 = not set :) */

	current2 = head;
	while (current2 != NULL)
	{
		if ((struct data *)current2 == ptr)
			break;
		current2 = current2->next;
	}

	do
	{
		err = 0;

		x = futureX(current2, B_SPEED);
		y = futureY(current2, B_SPEED);
		/* was: 

        x = futureB_X(current2);
        y = futureB_Y(current2);

		*/
		
		old_px = (int)(current2->x / BLOCKSIZE);
		old_py = (int)(current2->y / BLOCKSIZE);
		px = (int)(x / BLOCKSIZE);
		py = (int)(y / BLOCKSIZE);

		

		if ( !(px > X_BLOCKS) && !(px < 0) && 
			!(py > Y_BLOCKS) && !(py < 0) )
		{
			/* center of the bullet */
			if (field[py][px] == '1')
				collided = 1;

	//		/* BLOCKSIZE / 4 down the ship */
	//		if ((py - 1) > 0)
	//		{
	//			qy = (int) ((y - (BLOCKSIZE / 4)) / BLOCKSIZE);
	//			if (field[qy][px] == '1') collided = 1;
	//		}
	//		/* BLOCKSIZE / 4 above the ship */
	//		if ( (py + 1) <= Y_BLOCKS )
	//		{
	//			qy = (int)( (y + (BLOCKSIZE / 4) ) / BLOCKSIZE);
	//			if (field[qy][px] == '1') collided = 1;
	//		}
	//		/* BLOCKSIZE / 4 on the right of the ship */
	//		if ( (px + 1) <= Y_BLOCKS )
	//		{
	//			qx = (int)( (x + (BLOCKSIZE / 4) ) / BLOCKSIZE);
	//			if (field[py][qx] == '1') collided = 1;
	//		} 
	//		/* BLOCKSIZE / 4 on the left of the ship */
	//		if ( (px - 1) >= 0 )
	//		{
	//			qx = (int)( ( (x + (BLOCKSIZE / 4) ) / BLOCKSIZE) - 0.5); // dunno why i had to add the - 0.5
	//																	// in the VB version it wasn't necessary
	//			if (field[py][qx] == '1') {								// TODO; find out why :)
	//				collided = 1;
	//			}
	//		}

		}

		//if ( (collided == 1 && dabs(ourtime - current2->vel_time) > 100) )
		if ( collided == 1 )
		{
			current2->vel_time = ourtime;

			if (BOUNCING_BULLETS == 1)
			{
				int done_move = 0;
				char tmp[512];

				static FILE *fd;
				if (!fd)
					fd = fopen("WTF.txt", "w");

				sprintf(tmp, "%d %d COLLITION AT DEG: %f X: %f Y: %f", 
					current2->move, ourtime, current2->deg, current2->x, current2->y);
				
				// !!!!!!!! addtext("%s", tmp);
				fprintf(fd, "%s\n", tmp);

				// fclose(fd);
				// alert(tmp, "","","","", 1,1);
				//del_bullet(current2);
				//retval = 1;
				//return 1;
				//alert("COLLIDE", "", "", "", "", 1, 1);
				//addtext("COLLIDE");

				// (int)(current2->start_x / BLOCKSIZE)
				// (int)(current2->start_y / BLOCKSIZE)
				a = (px > old_px?1:0);
				b = (py > old_py?1:0);
				c = (px < old_px?1:0);
				d = (py < old_py?1:0);
	//			a = (px > (int)(current2->start_x / BLOCKSIZE)?1:0);
	//			b = (py > (int)(current2->start_y / BLOCKSIZE)?1:0);
	//			c = (px < (int)(current2->start_x / BLOCKSIZE)?1:0);
	//			d = (py < (int)(current2->start_y / BLOCKSIZE)?1:0);

				// !!!!!!!!! addtext("%d %d %d %d", a, b, c, d);

				if (a || b || c || d)
				{
					current2->start_x = current2->x;
					current2->start_y = current2->y;
				}

				/* Correct degrees if needed */
				if (current2->deg < 0)
				{
					sprintf(tmp, "wtf: %2.2f", current2->deg);
					alert(tmp, "", "", "", "", 1, 1);
					current2->deg += 90;
				}

				//if (current2->deg > 360)
			//		current2->deg -= 90;

				/* Lets bounce this bullet~~ */
			
	/*			if (a && 0 == 1)
				{
					addtext("MUUR = RECHTS");
					del_bullet(current2);
					retval = 1;
				}
				else if (b && 0 == 1)
				{
					addtext("MUUR = BENEDEN");
					del_bullet(current2);
					retval = 1; 
				} */
		/*		if (c && b)
				{	
					addtext("MUUR = LINKS / BENEDEN");
					addtext("status: %d %d %d %d", a, b, c, d);
					current2->deg = 45;
					retval = 1;
				}
				else if (a && b)
				{
					addtext("MUUR = RECHTS / BENEDEN");
					addtext("status: %d %d %d %d", a, b, c, d);
					current2->deg = 315;
					retval = 1;
				}
				else if (a && d)
				{
					addtext("MUUR = RECHTS / BOVEN");
					addtext("status: %d %d %d %d", a, b, c, d);
					current2->deg = 225;
					retval = 1;
				}
				else if (c && d)
				{
					addtext("MUUR = LINKS / BOVEN");
					addtext("status: %d %d %d %d", a, b, c, d);
					current2->deg = 135;
					retval = 1;
				} */
				if ( (c && d) ||
					 (a && b) ||
					 (a && d) ||
					 (c && d) )
				{
					err = 1;

					addtext("OLE");

					if (direction == -1)
						if (current2->deg >= 180)
							direction = 0;
						else
							direction = 1;

					if (direction == 1)
					{
						addtext("INCREASING");
						current2->deg++;
					}
					else
					{
						addtext("DECREASING");
						current2->deg--;
					}
				}
				else if (a || c)
				{
				//	if (c) 
				//		addtext("MUUR = LINKS");
				//	else 
				//		addtext("MUUR = RECHTS");

	//addtext("status: %d %d %d %d", a, b, c, d);
	//addtext("xbefore deg: %2.2f", current2->deg);
					if (current2->deg > 0 && current2->deg < 90)
						current2->deg = 360.0 - current2->deg;
					else if (current2->deg == 0)
						current2->deg = 180;
					else if (current2->deg == 90)
						current2->deg = 270;
					else if (current2->deg > 90 && current2->deg < 180)
						current2->deg = 360 - current2->deg;
					else if (current2->deg == 180)
						current2->deg = 0;
					else if (current2->deg > 180 && current2->deg < 270)
						current2->deg = 360 - current2->deg;
					else if (current2->deg == 270)
						current2->deg = 90;
					else if (current2->deg > 270 && current2->deg < 360)
						current2->deg = 360.0 - current2->deg;
					else if (current2->deg == 360)
						current2->deg = 180;

					if (current2->deg < 0)
						current2->deg += 360;
					if (current2->deg > 360)
						current2->deg -= 360;
	//addtext("xafter deg: %f x:%f y:%f", current2->deg, current2->x, current2->y);
					retval = 1;
				}
				else if (b || d)
				{
				//	if (b) 
				//		addtext("MUUR = BENEDEN");
				//	else
				//		addtext("MUUR = BOVEN");
	//addtext("status: %d %d %d %d", a, b, c, d);
	//addtext("abefore deg: %2.2f", current2->deg);
					if (current2->deg > 0 && current2->deg < 90)
						current2->deg = 180.0 - current2->deg;
					else if (current2->deg == 0)
						current2->deg = 180;
					else if (current2->deg == 90)
						current2->deg = 270;
					else if (current2->deg > 90 && current2->deg < 180)
						current2->deg = 180 - current2->deg;
					else if (current2->deg == 180)
						current2->deg = 0;
					else if (current2->deg > 180 && current2->deg < 270)
						current2->deg = 180 - current2->deg;
					else if (current2->deg == 270)
						current2->deg = 90;
					else if (current2->deg > 270 && current2->deg < 360)
						current2->deg = 180 - current2->deg;
					else if (current2->deg == 360)
						current2->deg = 180;

					if (current2->deg < 0)
						current2->deg += 360;
					if (current2->deg > 360)
						current2->deg -= 360;
	//addtext("aafter deg: %f x:%f y:%f", current2->deg, current2->x, current2->y);
					retval = 1;
				}
			


				/* */
				//del_bullet(current2);
				//retval = 1;
			} else {
				/* A non bouncing bullet would die here */
				explosion(current2->x, current2->y, 15, 10, makecol(0, 128, 255));
				del_bullet(current2);				
				retval = 1;
			}
		}
	} while (err == 1);

	return retval;
}

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

			if ( (ourtime - tmp->hit_t) >= 200)
				tmp->impact = 0;

			LINK tmp2 = head;
			LINK old = NULL;
			/* Check if this ship hit a bullet */
			while (tmp2 != NULL)
			{
				i = 0;
				if ( (tmp2->bullet != 1) && (tmp->owner != tmp2) && 
					 (tmp2->dead != 1) && (tmp2->invincible == 0) )
				{
					/* Compare coordinates with this ship */
					if ( (dabs(tmp2->x - (tmp->x) ) <= (BLOCKSIZE / 4)) &&
						 (dabs(tmp2->y - (tmp->y) ) <= (BLOCKSIZE / 4)) )
					{
						// addtext("del bullet at addr: %d", tmp);
						explosion(tmp->x, tmp->y, 30, 30, makecol(255, 255, 255));
						verbose("ID %u was hit by bullet owned by ID: %u", tmp2->id, tmp->owner);
						del_bullet(tmp);
						
						retval = 1;

						if (our_node == tmp2)
							guessed_power--;

						tmp2->impact = 1;
						tmp2->hit_t = ourtime;

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
		alert("error 1", "","","","",1,1);
		terminate();
	}

	return retval;
}