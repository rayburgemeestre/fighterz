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

#define AFKAATSEN true
int collidecheck2(unsigned int id, int variation, int nobounce) /* variation = only 1 when 
								called in mainloop() at UP/DOWN stuff */
{
PLAYER current2;
int collided = 0; /* 0=no, 1=yes */
int cool = 0;
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
	#define BOUNCE_POSSIBLE_OR_RESTORE \
		if (collidecheck2(current2->id, 0, 1) != 1) \
		{ \
			collided = 0; \
		} else { \
			current2->deg = old_deg; \
		}

	/* TODO: This is weird, change this?? */
    if (current2->speed != 0.00)
        if (current2->speed > -0.01)
            direction = step;
        else
            direction = step * -1;
    else
        if (our_direction)
            direction = step;
        else
            direction = step * -1;

	x = futureX(current2, current2->deg, direction);
	y = futureY(current2, current2->deg, direction);

//	addtext("After x:%.2f y:%.2f", x, y);

	px = (int)(x / blocksize);
	py = (int)(y / blocksize);

	if ( !(px > map_blocks_x) && !(px < 0) && 
		!(py > map_blocks_y) && !(py < 0) )
	{
	rdemp = (dabs(current2->speed) / current2->max_speed) * (demp_max - demp_min);
	rdemp = (demp_max - demp_min) - rdemp;
	rdemp = (rdemp > 0 ? rdemp : 1) + demp_min;

		/* center of the ship */
		if (field[py][px] == '1')
		{
			collided = 1;
		}

		/* blocksize / 4 above the ship - tested */
		if ((py - 1) >= 0)
		{
			qy = (int) ((y - (blocksize / 4)) / blocksize);
			if (field[qy][px] == '1') 
			{
				collided = 1;
				if (cool)
					rectfill(
						bmp_shipfield, 
						(px * blocksize), (qy * blocksize), 
						((px + 1) * blocksize), ((qy + 1) * blocksize), 
						clr
					);

				if (!nobounce && dabs(current2->speed) > 0.05 && AFKAATSEN) // static minimal speed required
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
		/* blocksize / 4 down the ship - tested */
		if ( (py + 1) <= map_blocks_y )
		{
			qy = (int)( (y + (blocksize / 4) ) / blocksize);
			if (field[qy][px] == '1') 
			{
				if (cool)
					rectfill(
						bmp_shipfield, 
						(px * blocksize), (qy * blocksize), 
						((px + 1) * blocksize), ((qy + 1) * blocksize), 
						clr
					);

				collided = 1;

				if (!nobounce && dabs(current2->speed) > 0.05 && AFKAATSEN) // static minimal speed required
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
		/* blocksize / 4 on the right of the ship - tested*/
		if ( (px + 1) <= (map_blocks_x))
		{
			qx = (int)( (x + (blocksize / 4) ) / blocksize);
			if (field[py][qx] == '1') 
			{
				if (cool)
					rectfill(
						bmp_shipfield, 
						(qx * blocksize), (py * blocksize), 
						((qx + 1) * blocksize), ((py + 1) * blocksize), 
						clr
					);

					collided = 1;

				if (!nobounce && dabs(current2->speed) > 0.05 && AFKAATSEN) // static minimal speed required
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
		/* blocksize / 4 on the left of the ship - tested*/
		if ( (px - 1) >= 0 )
		{
			qx = (int)( ( (x + (blocksize / 4) ) / blocksize) - 0.5); // dunno why i had to add the - 0.5
																	// in the VB version it wasn't necessary
			if (field[py][qx] == '1') 
			{														// TODO; find out why :)
				if (cool)
					rectfill(
						bmp_shipfield, 
						(qx * blocksize), (py * blocksize), 
						((qx + 1) * blocksize), ((py + 1) * blocksize), 
						clr
					);

				collided = 1;

				if (!nobounce && dabs(current2->speed) > 0.05 && AFKAATSEN) // static minimal speed required
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
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Vooruit tegen de linkerkant muur (van rechts)
					else if (
						current2->deg >= 180
							&& 
						current2->deg <= (180 + deg_diff_max)
					) {
						current2->deg = 180 - ( (current2->deg - 180) / rdemp );
						// Need to know if it won't collide now..
						BOUNCE_POSSIBLE_OR_RESTORE
					}
					// Achterruit tegen de linkerkant muur (van rechts!)
					else if (
						current2->deg >= (180 - deg_diff_max)
							&& 
						current2->deg <= (180 + deg_diff_max)
					) {
						current2->deg = 180 + ( (180 - current2->deg) / rdemp );
						// Need to know if it won't collide now..
						BOUNCE_POSSIBLE_OR_RESTORE
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
#ifdef OLD_CODE
	if (collided == 1)
	{
		if (current2->id == our_id)
		{	/* our ship */
			if (current2->speed != 0.0)
			{	
				if (0)
				{
				int retval = -1; // make args compatible ;)
					process_bounce(current2, &retval, current2->max_speed);

				} else {
					//addtext("C: collided at velocity: %d", our_node->velocity);
					//addtext("Set To 0");
					//current2->velocity = 0;
					//current2->speed = 0.0;
					current2->freeze = 1;
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
#else
	if (collided == 1)
	{
// we lose speed here:
		if (current2 == our_node && DONT_LOSE_VELOCITY_AT_COLLISION == false)
		{
			current2->velocity = 0;
			current2->speed    = 0.0;
			current2->freeze   = 1;
			if (variation == 0)
				send_accel(4);
		}
// until here..
		current2->freeze = 1;
	} else {
		current2->freeze = 0;
	}
#endif
	return collided;
}

double futureX(struct data *ptr, double angle, double speed)
{
double radius;
double rads;
double pos_x;

    radius = speed;
	rads   = (PI * (angle-90) / 180);
    pos_x  = (cos(rads) * radius) + ptr->x;

    if (pos_x < 0)
        pos_x = field_width - 1;
    else if (pos_x > field_width)
        pos_x = 1;

    return pos_x;
}

double futureY(struct data *ptr, double angle, double speed)
{
double radius;
double rads;
double pos_y;

	radius = speed;
	rads   = (PI * (angle-90) / 180);
	pos_y  = (sin(rads) * radius) + ptr->y;

	if (pos_y < 0)
		pos_y = field_height - 1;
	else if (pos_y > field_height)
		pos_y = 1;

	return pos_y;
}

double X2(double x, double y, double angle, double radius)
{
double rads;
double x2;

	rads = (PI * (angle-90) / 180);
    x2   = (cos(rads) * radius) + x;

    return x2;
}

double Y2(double x, double y, double angle, double radius)
{
double rads;
double y2;

	rads = (PI * (angle-90) / 180);
	y2   = (sin(rads) * radius) + y;

	return y2;
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

	x = futureX(ptr, ptr->deg, speed);
	y = futureY(ptr, ptr->deg, speed);

			if (!fd) fd = fopen("collition.txt", "w");

			sprintf(tmp, "%d %lu COLLITION AT DEG: %f X: %f Y: %f",
				ptr->move, ourtime, ptr->deg, ptr->x, ptr->y);


	// circlefill(bmp_mapfield, ptr->x, ptr->y, 2, makecol(255, 0, 0));

	old_px = (int)(ptr->x / blocksize);
	old_py = (int)(ptr->y / blocksize);
	px = (int)(x / blocksize);
	py = (int)(y / blocksize);

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
	while (ptr->deg < 0)
	{
		ptr->deg += 360;
	}

	/* Lets bounce this bullet~~ */

	if (c && b)
	{	
		addtext("MUUR = LINKS / BENEDEN");
		addtext("status: %d %d %d %d", a, b, c, d);

		if (py > 0 && field[py - 1][px] == '1')
			ptr->deg -= 90; // 1
		else if (px < map_blocks_x && field[py][px + 1] == '1')
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
		if (py < map_blocks_y && field[py + 1][px] == '1')
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
		
		if (px < map_blocks_x && field[py][px + 1] == '1')
			ptr->deg -= 90; // 1
		if (py < map_blocks_y && field[py + 1][px] == '1')
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
PLAYER current2;
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

		x = futureX(current2, current2->deg, bullet_movespeed);
		y = futureY(current2, current2->deg, bullet_movespeed);
		
		old_px = (int)(current2->x / blocksize);
		old_py = (int)(current2->y / blocksize);
		px = (int)(x / blocksize);
		py = (int)(y / blocksize);
		

		if ( !(px > map_blocks_x) && !(px < 0) && 
			!(py > map_blocks_y) && !(py < 0) )
		{
			/* center of the bullet */
			if (field[py][px] == '1')
				collided = 1;

// Dirty fix:
#define MARGE (blocksize / 8)
								/* MARGE on the right of the bullet *** */
								if ( (px + 1) <= (map_blocks_x))
								{
									int qx;
									qx = (int)( (x + MARGE ) / blocksize);
									if (field[py][qx] == '1') 
									{
										rectfill(
											bmp_shipfield, 
											(qx * blocksize), (py * blocksize), 
											((qx + 1) * blocksize), ((py + 1) * blocksize), 
											makecol(255, 0, 255)
										);
										collided = 1;
									}
								}
								/* MARGE on the left of the bullet *** */
/*
								if ( (px - 1) >= 0 )
								{
									int qx;
									qx = (int)( ( (x + MARGE ) / blocksize));
									if (field[py][qx] == '1') 
									{														// TODO; find out why :)
										rectfill(
											bmp_shipfield, 
											(qx * blocksize), (py * blocksize), 
											((qx + 1) * blocksize), ((py + 1) * blocksize), 
											makecol(255,0,0)
										);
										collided = 1;
									}
								} */
								/* *** Should prevent bullets from going through walls like:  #
																							   #
																								#
																								 #
								*/
		}

		//if ( (collided == 1 && dabs(ourtime - current2->vel_time) > 100) )
		if ( collided == 1 )
		{
			current2->vel_time = ourtime;

			if (mod_bounce == 1)
			{
				process_bounce(ptr, &retval, bullet_movespeed);
			} else {
				/* A non bouncing bullet would die here */
				add_explosion(current2->x, current2->y, 15, 10, makecol(0, 128, 255));
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
	PLAYER tmp = ptr;
	int retval = 0;
	// while (tmp != NULL)
	if (tmp != NULL)
	{
		if (tmp->bullet == 1)
		{	
			int i;

			if ( (ourtime - tmp->hit_t) >= 200)
				tmp->impact = 0;

			PLAYER tmp2 = head;
			PLAYER old = NULL;
			/* Check if this ship hit a bullet */
			while (tmp2 != NULL)
			{
				i = 0;
				if ( (tmp2->bullet != 1) && (tmp->owner != tmp2) && 
					 (tmp2->dead == 0) && (tmp2->invincible == 0) )
				{
					/* Compare coordinates with this ship */
					if ( (dabs(tmp2->x - (tmp->x) ) <= (blocksize / 4)) &&
						 (dabs(tmp2->y - (tmp->y) ) <= (blocksize / 4)) )
					{
						// addtext("del bullet at addr: %d", tmp);
						add_explosion(tmp->x, tmp->y, 30, 30, makecol(255, 255, 255));
						// verbose("ID %u was hit by bullet owned by ID: %u", tmp2->id, tmp->owner);
						del_bullet(tmp);
						
						retval = 1;


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