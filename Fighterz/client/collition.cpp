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
			collidecheck2(current->id, 0, 0);
		}
		current = current->next;
	}
}

int collidecheck2(unsigned int id, int variation, int nobounce) /* variation = only 1 when 
								called in mainloop() at UP/DOWN stuff */
{
LINK current2;
int collided = 0; /* 0=no, 1=yes */
int cool = 1;
int clr = makecol(128, 128, 128);
double x, y;
int px, py, qx, qy;
/* the fucked up stuff */
double direction, old_deg;
int step = 1; /* Will only use this nonsense temporary */
int deg_diff_max = 50, 
	demp_min = 10,
	demp_max = 25,
	rdemp;

	current2 = head;
	while (current2 != NULL)
	{
		if (current2->id == id)
			break;
		current2 = current2->next;
	}

old_deg = current2->deg;
#define BOUNCE_POSSIBLE_OR_RESTORE if (collidecheck2(current2->id, 0, 1) != 1) { collided = 0; } else { current2->deg = old_deg;}

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
	rdemp = (dabs(current2->speed) / SPEED) * (demp_max - demp_min);
	rdemp = (demp_max - demp_min) - rdemp;
	rdemp = (rdemp > 0 ? rdemp : 1) + demp_min;

		/* center of the ship */
		if (field[py][px] == '1')
		{
			collided = 1;
		}

		/* BLOCKSIZE / 4 above the ship - tested */
		if ((py - 1) >= 0)
		{
			qy = (int) ((y - (BLOCKSIZE / 4)) / BLOCKSIZE);
			if (field[qy][px] == '1') 
			{
				collided = 1;
				if (cool)
					rectfill(
						shipbuff, 
						(px * BLOCKSIZE_2), (qy * BLOCKSIZE_2), 
						((px + 1) * BLOCKSIZE_2), ((qy + 1) * BLOCKSIZE_2), 
						clr
					);

				if (!nobounce && dabs(current2->speed) > 0.05) // static minimal speed required
				{
// --
// -- Afkaatsen				
// --
					// Vooruit tegen de bovenkant muur (van links)
					if (
						current2->deg >= (90 - deg_diff_max) 
							&& 
						current2->deg <= 90
					) {
						current2->deg = ( (90 - current2->deg) / rdemp ) + 90;
						BOUNCE_POSSIBLE_OR_RESTORE
						
					}
					// Vooruit tegen de bovenkant muur (van rechts)
					else if (
						current2->deg >= 270
							&& 
						current2->deg <= (270 + deg_diff_max)
					) {
						current2->deg = 270 - ( (current2->deg - 270) / rdemp);
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Achterruit tegen de bovenkant muur (van rechts!)
					else if (
						current2->deg >= 90
							&& 
						current2->deg <= (90 + deg_diff_max)
					) {
						current2->deg = 90 - ( (current2->deg - 90) / rdemp);
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Achterruit tegen de bovenkant muur (van links!)
					else if (
						current2->deg >= (270 - deg_diff_max)
							&& 
						current2->deg <= 270
					) {
						current2->deg = 270 + ( (270 - current2->deg) / rdemp);
						BOUNCE_POSSIBLE_OR_RESTORE
					}
// --
// --
// --
				}
			}
		}
		/* BLOCKSIZE / 4 down the ship - tested */
		if ( (py + 1) <= Y_BLOCKS )
		{
			qy = (int)( (y + (BLOCKSIZE / 4) ) / BLOCKSIZE);
			if (field[qy][px] == '1') 
			{
				if (cool)
					rectfill(
						shipbuff, 
						(px * BLOCKSIZE_2), (qy * BLOCKSIZE_2), 
						((px + 1) * BLOCKSIZE_2), ((qy + 1) * BLOCKSIZE_2), 
						clr
					);

				collided = 1;

				if (!nobounce && dabs(current2->speed) > 0.05) // static minimal speed required
				{
// --
// -- Afkaatsen				
// --
					// Vooruit tegen de onderkant muur (van links)
					if (
						current2->deg >= (270 - deg_diff_max)
							&& 
						current2->deg <= 270
					) {
						current2->deg = 270 + ( (270 - current2->deg) / rdemp );
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Vooruit tegen de onderkant muur (van rechts)
					else if (
						current2->deg >= 90
							&& 
						current2->deg <= (90 + deg_diff_max)
					) {
						current2->deg = 90 - ( (current2->deg - 90) / rdemp );
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Achterruit tegen de onderkant muur (van rechts!)
					else if (
						current2->deg >= (90 - deg_diff_max)
							&& 
						current2->deg <= 90
					) {
						current2->deg = 90 + ( (90 - current2->deg) / rdemp );
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Achterruit tegen de onderkant muur (van links!)
					else if (
						current2->deg >= 270
							&& 
						current2->deg <= (270 + deg_diff_max)
					) {
						current2->deg = 270 - ( (current2->deg - 270) / rdemp);
						BOUNCE_POSSIBLE_OR_RESTORE
					}
// --
// --
// --
				}
			}
		}
		/* BLOCKSIZE / 4 on the right of the ship - tested*/
		if ( (px + 1) <= (X_BLOCKS))
		{
			qx = (int)( (x + (BLOCKSIZE / 4) ) / BLOCKSIZE);
			if (field[py][qx] == '1') 
			{
				if (cool)
					rectfill(
						shipbuff, 
						(qx * BLOCKSIZE_2), (py * BLOCKSIZE_2), 
						((qx + 1) * BLOCKSIZE_2), ((py + 1) * BLOCKSIZE_2), 
						clr
					);

					collided = 1;

				if (!nobounce && dabs(current2->speed) > 0.05) // static minimal speed required
				{
// --
// -- Afkaatsen				
// --
					// Vooruit tegen de rechterkant muur (van links)
					if (
						current2->deg >= (180 - deg_diff_max)
							&& 
						current2->deg <= 180
					) {
						current2->deg = ( (180 - current2->deg) / rdemp) + 180;
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Vooruit tegen de rechterkant muur (van rechts)
					else if (
						current2->deg >= 0
							&& 
						current2->deg <= deg_diff_max
					) {
						current2->deg = 360 - ( current2->deg / rdemp );
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Achterruit tegen de rechterkant muur (van rechts!)
					else if (
						current2->deg >= 180
							&& 
						current2->deg <= (180 + deg_diff_max)
					) {
						current2->deg = 180 - ( (current2->deg - 180) / rdemp);
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Achterruit tegen de rechterkant muur (van links!)
					else if (
						current2->deg >= (360 - deg_diff_max)
							&& 
						current2->deg <= 360
					) {
						current2->deg = ( (360 - current2->deg) / rdemp);
						BOUNCE_POSSIBLE_OR_RESTORE
					}
// --
// --
// --
				}
			}
		}
		/* BLOCKSIZE / 4 on the left of the ship - tested*/
		if ( (px - 1) >= 0 )
		{
			qx = (int)( ( (x + (BLOCKSIZE / 4) ) / BLOCKSIZE) - 0.5); // dunno why i had to add the - 0.5
																	// in the VB version it wasn't necessary
			if (field[py][qx] == '1') 
			{														// TODO; find out why :)
				if (cool)
					rectfill(
						shipbuff, 
						(qx * BLOCKSIZE_2), (py * BLOCKSIZE_2), 
						((qx + 1) * BLOCKSIZE_2), ((py + 1) * BLOCKSIZE_2), 
						clr
					);

				collided = 1;

				if (!nobounce && dabs(current2->speed) > 0.05) // static minimal speed required
				{
// --
// -- Afkaatsen				
// --
					// Vooruit tegen de linkerkant muur (van links)
					if (
						current2->deg >= (360 - deg_diff_max)
							&& 
						current2->deg <= 360
					) {
						current2->deg = (360 - current2->deg) / rdemp;
						// Need to know if it won't collide now..
						if (collidecheck2(current2->id, 0, 1) != 1)
							collided = 0;
					}
					// Vooruit tegen de linkerkant muur (van rechts)
					else if (
						current2->deg >= 180
							&& 
						current2->deg <= (180 + deg_diff_max)
					) {
						current2->deg = 180 - ( (current2->deg - 180) / rdemp );
						// Need to know if it won't collide now..
						if (collidecheck2(current2->id, 0, 1) != 1)
							collided = 0;
					}
					// Achterruit tegen de linkerkant muur (van rechts!)
					else if (
						current2->deg >= (180 - deg_diff_max)
							&& 
						current2->deg <= (180 + deg_diff_max)
					) {
						current2->deg = 180 + ( (180 - current2->deg) / rdemp );
						// Need to know if it won't collide now..
						if (collidecheck2(current2->id, 0, 1) != 1)
							collided = 0;
					}
					// Achterruit tegen de rechterkant muur (van links!)
					else if (
						current2->deg >= 0
							&& 
						current2->deg <= deg_diff_max
					) {
						current2->deg = 360 - (current2->deg / rdemp);
						// Need to know if it won't collide now..
						BOUNCE_POSSIBLE_OR_RESTORE
					}
// --
// --
// --
				}
			}
		}
	}

	if (collided == 1)
	{
		if (current2->id == our_id)
		{	/* our ship */
			if (current2->speed != 0.0)
			{	
/*
*sough*
this is really bitching (there's distance between the
center of the ship and the collition point. And the collision
point itself is also not known exactly..
*/
				if (0)
				{
				int retval = -1; // make args compatible ;)
					process_bounce(current2, &retval, SPEED);

				} else {
					//addtext("C: collided at velocity: %d", our_node->velocity);
					//addtext("Set To 0");
					current2->velocity = 0;
					current2->speed = 0.0;
					// current2->freeze = 1;
					if (variation == 0)
						send_accel(4);
				}
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


void process_bounce(struct data *ptr, int *retval, double speed)
{
int done_move = 0;
double x, y;
char tmp[512];
static FILE *fd;
int a,b,c,d;
int px, py;
int old_px, old_py;

	x = futureX(ptr, speed);
	y = futureY(ptr, speed);

			if (!fd) fd = fopen("collition.txt", "w");

			sprintf(tmp, "%d %d COLLITION AT DEG: %f X: %f Y: %f", 
				ptr->move, ourtime, ptr->deg, ptr->x, ptr->y);


	circlefill(fieldbuff, ptr->x, ptr->y, 2, makecol(255, 0, 0));
	old_px = (int)(ptr->x / BLOCKSIZE);
	old_py = (int)(ptr->y / BLOCKSIZE);
	px = (int)(x / BLOCKSIZE);
	py = (int)(y / BLOCKSIZE);

	a = (px > old_px?1:0);
	b = (py > old_py?1:0);
	c = (px < old_px?1:0);
	d = (py < old_py?1:0);

	if (a || b || c || d)
	{
		ptr->start_x = ptr->x;
		ptr->start_y = ptr->y;
	}

	/* Correct degrees if needed */
	if (ptr->deg < 0)
	{
		sprintf(tmp, "wtf: %2.2f", ptr->deg);
		alert(tmp, "", "", "", "", 1, 1);
		ptr->deg += 90;
	}

	/* Lets bounce this bullet~~ */

	if (c && b)
	{	
		addtext("MUUR = LINKS / BENEDEN");
		addtext("status: %d %d %d %d", a, b, c, d);

		if (py > 0 && field[py - 1][px] == '1')
			ptr->deg -= 90; // 1
		else if (px < X_BLOCKS && field[py][px + 1] == '1')
			ptr->deg += 90; // 2
		else
			ptr->deg -= 180; // 3

		// ?? ptr->deg = 45;
		*retval = 1;
	}
	else if (a && b)
	{
		addtext("MUUR = RECHTS / BENEDEN");
		addtext("status: %d %d %d %d", a, b, c, d);

		if (py > 0 && field[py - 1][px] == '1')
			ptr->deg += 90; // 1
		if (px > 0 && field[py][px - 1] == '1')
			ptr->deg -= 90; // 2
		else
			ptr->deg += 180; // 3

		// ?? ptr->deg = 315;
		*retval = 1;
	}
	else if (a && d)
	{
		addtext("MUUR = RECHTS / BOVEN");
		addtext("status: %d %d %d %d", a, b, c, d);
		
		if (px > 0 && field[py][px - 1] == '1')
			ptr->deg += 90; // 1
		if (py < Y_BLOCKS && field[py + 1][px] == '1')
			ptr->deg += 270; // 2
		else
			ptr->deg += 180; // 3

		// ?? ptr->deg = 225;
		*retval = 1;
	}
	else if (c && d)
	{
		addtext("MUUR = LINKS / BOVEN");
		addtext("status: %d %d %d %d", a, b, c, d);
		
		if (px < X_BLOCKS && field[py][px + 1] == '1')
			ptr->deg -= 90; // 1
		if (py < Y_BLOCKS && field[py + 1][px] == '1')
			ptr->deg -= 270; // 2
		else
			ptr->deg -= 180; // 3

		// ?? ptr->deg = 135;
		*retval = 1;
	}

	/* if ( (c && d) ||
		 (a && b) ||
		 (a && d) ||
		 (c && d) )
	{
		err = 1;

		if (direction == -1)
			if (ptr->deg >= 180)
				direction = 0;
			else
				direction = 1;

		if (direction == 1)
		{
			ptr->deg++;
		}
		else
		{
			ptr->deg--;
		}
	} */
	else if (a || c)
	{
	//	if (c) 
	//		addtext("MUUR = LINKS");
	//	else 
	//		addtext("MUUR = RECHTS");

		if (ptr->deg > 0 && ptr->deg < 90)
			ptr->deg = 360.0 - ptr->deg;
		else if (ptr->deg == 0)
			ptr->deg = 180;
		else if (ptr->deg == 90)
			ptr->deg = 270;
		else if (ptr->deg > 90 && ptr->deg < 180)
			ptr->deg = 360 - ptr->deg;
		else if (ptr->deg == 180)
			ptr->deg = 0;
		else if (ptr->deg > 180 && ptr->deg < 270)
			ptr->deg = 360 - ptr->deg;
		else if (ptr->deg == 270)
			ptr->deg = 90;
		else if (ptr->deg > 270 && ptr->deg < 360)
			ptr->deg = 360.0 - ptr->deg;
		else if (ptr->deg == 360)
			ptr->deg = 180;

		if (ptr->deg < 0)
			ptr->deg += 360;
		if (ptr->deg > 360)
			ptr->deg -= 360;

		*retval = 1;
	}
	else if (b || d)
	{
	//	if (b) 
	//		addtext("MUUR = BENEDEN");
	//	else
	//		addtext("MUUR = BOVEN");
		if (ptr->deg > 0 && ptr->deg < 90)
			ptr->deg = 180.0 - ptr->deg;
		else if (ptr->deg == 0)
			ptr->deg = 180;
		else if (ptr->deg == 90)
			ptr->deg = 270;
		else if (ptr->deg > 90 && ptr->deg < 180)
			ptr->deg = 180 - ptr->deg;
		else if (ptr->deg == 180)
			ptr->deg = 0;
		else if (ptr->deg > 180 && ptr->deg < 270)
			ptr->deg = 180 - ptr->deg;
		else if (ptr->deg == 270)
			ptr->deg = 90;
		else if (ptr->deg > 270 && ptr->deg < 360)
			ptr->deg = 180 - ptr->deg;
		else if (ptr->deg == 360)
			ptr->deg = 180;

		if (ptr->deg < 0)
			ptr->deg += 360;
		if (ptr->deg > 360)
			ptr->deg -= 360;

		*retval = 1;
	}
}

// bullet

int collidecheck2b(struct data *ptr)
{
LINK current2;
int collided = 0; /* 0=no, 1=yes */
int err = 0;
double x, y;
// int qx, qy;
int retval = 0;
int direction = -1; /* 1 is degrees ++, 0 --
	              -1 = not set :) */
int px, py;
int old_px, old_py;
	
	// wat hier staat is echt zwaar fucking nuttig
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
				process_bounce(ptr, &retval, B_SPEED);
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
					 (tmp2->dead == 0) && (tmp2->invincible == 0) )
				{
					/* Compare coordinates with this ship */
					if ( (dabs(tmp2->x - (tmp->x) ) <= (BLOCKSIZE / 4)) &&
						 (dabs(tmp2->y - (tmp->y) ) <= (BLOCKSIZE / 4)) )
					{
						// addtext("del bullet at addr: %d", tmp);
						explosion(tmp->x, tmp->y, 30, 30, makecol(255, 255, 255));
						// verbose("ID %u was hit by bullet owned by ID: %u", tmp2->id, tmp->owner);
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