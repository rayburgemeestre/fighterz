#include "common.h"

//bullets
double bullet_movespeed; /**< bulletspeed: pixels to move each iteration e.g.: value=0.40(pixels) */

//linked list: players a.k.a. clients
PLAYER head = NULL; /**< pointer to the first node in the linked list */
PLAYER new_node = NULL; /**< pointer used for new nodes in the linked list */
PLAYER current = NULL; /**< pointer to the current selected node in the linked list */
PLAYER our_node = NULL; /**< pointer that always points to our node in the linked list */

//linked list: explosions (circular)
EXPLOSION expl_head = NULL; /**< pointer to the first node in the explosion linked list */
EXPLOSION expl_new = NULL; /**< pointer used for new nodes in the explosion linked list */
EXPLOSION expl_current = NULL; /**< pointer to the current selected node in the explosion linked list */

/* ############# */
/* ### SHIPS ### */
/* ############# */

void draw_ships() 
{
int color;
double radius;	
double ret;
double pos_x, pos_y; /* ship's x,y */
double left_wing_deg, right_wing_deg; 
double lw_x, lw_y; /* leftwing x,y */
double rw_x, rw_y; /* rightwing x,y */
double current_x, current_y;

//	double fill_x, fill_y;

	radius = blocksize / 2;

	current = head;
	while (current != NULL)
	{
	current_x = current->x - x_on_map;
	current_y = current->y - y_on_map;

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
				if (mod_grid)
				{
					ret = (PI / 180) * (current->deg - 90);
					pos_x = futureX(current, current->deg, radius);
					pos_y = futureY(current, current->deg, radius);

					circle(bmp_shipfield, current_x, current_y, blocksize / 4, color);
					line(bmp_shipfield, current_x, current_y, pos_x, pos_y, color);
				}
			} else {

				if (mod_grid)
				{
					/**********************
					 **   CALCULATIONS   **
					 **********************/

					/* ship coordinates */
					ret = (PI / 180) * (current->deg - 90);

					pos_x = cos(ret);
					pos_x = pos_x * (radius - (blocksize / 4));
					pos_x = pos_x + radius;
					pos_x = pos_x + current_x - (blocksize / 2);

					pos_y = sin(ret);
					pos_y = pos_y * (radius - (blocksize / 4));
					pos_y = pos_y + radius;
					pos_y = pos_y + current_y - (blocksize / 2);

					/* wing coordinates */
					left_wing_deg = current->deg + 120;
					right_wing_deg = current->deg - 120;

					/* left wing */
					ret = (PI / 180) * (left_wing_deg - 90);
					lw_x = cos(ret);
					lw_x = lw_x * radius;
					lw_x = lw_x + radius;
					lw_x = lw_x + current_x - (blocksize / 2);

					lw_y = sin(ret);
					lw_y = lw_y * radius;
					lw_y = lw_y + radius;
					lw_y = lw_y + current_y - (blocksize / 2);

					/* right wing */
					ret = (PI / 180) * (right_wing_deg - 90);
					rw_x = cos(ret);
					rw_x = rw_x * radius;
					rw_x = rw_x + radius;
					rw_x = rw_x + current_x - (blocksize / 2);

					rw_y = sin(ret);
					rw_y = rw_y * radius;
					rw_y = rw_y + radius;
					rw_y = rw_y + current_y - (blocksize / 2);

					/**********************
					 **     DRAW SHIP    **
					 **********************/
					
					// draw as polygon
					{
						int points[8] = {
							pos_x, pos_y,
							lw_x, lw_y,
							current_x, current_y,
							rw_x, rw_y
						};
						int *p = points;

						polygon(bmp_shipfield, 4, p, makecol(0, 0, 0));
					}
					/* ship base */
					// line(bmp_shipfield , current_x, current_y, pos_x, pos_y, color);
					/* left wing */
					line(bmp_shipfield , pos_x, pos_y, lw_x, lw_y, color);
					line(bmp_shipfield , current_x, current_y, lw_x, lw_y, color);
					/* right wing */
					line(bmp_shipfield , pos_x, pos_y, rw_x, rw_y, color);
					line(bmp_shipfield , current_x, current_y, rw_x, rw_y, color);
				}
			
			}
            if (current->invincible == 1)
			    rect(bmp_shipfield, current_x - (blocksize / 2), current_y - (blocksize / 2), 
			    	current_x + (blocksize / 2), current_y + (blocksize / 2), makecol(255,0,0));				
            else if (current->invincible == 2)
			    rect(bmp_shipfield, current_x - (blocksize / 2), current_y - (blocksize / 2), 
			    	current_x + (blocksize / 2), current_y + (blocksize / 2), makecol(255,128,0));				

			if (!mod_grid && current->dead == 0)
			{
				if (current->bot == 0) //test 
				{
				int shiptype;
#ifdef NOT_DEFINED
					PLAYER f;
					for (f = head; f != our_node; f=f->next)
					{
						if (f->bot == 1)
							line(bmp_shipfield, f->x, f->y, our_node->x - x_on_map, our_node->y - y_on_map, makecol(0,0,64));
					}
#endif
					
					switch (current->shiptype)
					{
						case 1:
							shiptype = SHIPS1; break;
						case 2:
							shiptype = SHIPS2; break;
						case 3:
							shiptype = SHIPS3; break;
						case 4:
							shiptype = SHIPS4; break;
						case 5:
							shiptype = SHIPS5; break;
					}

					rotate_sprite(bmp_shipfield, (BITMAP *)dat_base[shiptype].dat, current_x - (blocksize / 2), current_y - (blocksize / 2), itofix( (int)((current->deg*256)/360)) );
					if (our_node == current)
					{
						rotate_sprite(bmp_radar, (BITMAP *)dat_base[shiptype].dat, radar_shiptype_x, radar_shiptype_y, 0 );
					}
				}
				else
					rotate_sprite(bmp_shipfield, (BITMAP *)dat_base[SHIPS4].dat, current_x - (blocksize / 2), current_y - (blocksize / 2), itofix( (int)((current->deg*256)/360)) );

				if (show_names == 1)
				{
					int _bullets = ((our_bullet_max - our_bullet_count) * 100) / our_bullet_max;
					char tmpstr[128];
					// sprintf(tmpstr, "%s [%d] [%2.2f] [%d]", current->nick, current->velocity, current->speed, current->freeze);
					sprintf(tmpstr, "%s", current->nick);
					textprintf_centre(bmp_shipfield, (FONT *)dat_base[NOKIA].dat, current_x, 
						current_y + blocksize, makecol(192,192,192), tmpstr);
				}
			}

		}
		/* next ship */
		current = current->next;
	}
}

void move_ships()
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
 			move_ship(current->id, ourtime);

			/* something to remove the 'red' glow of hit ships after 200 ms */
			if ( (ourtime - current->hit_t) >= 200)
			current->impact = 0;
		}
		current = current->next;
	}
}

#define TURN_SPEED  10//18
#define ACCEL_SPEED 30
void move_ship(unsigned int id2, unsigned long t2)
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
				tmp = TURN_SPEED; /* Turn speed */
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
			tmp = ACCEL_SPEED;
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
					if (current->speed < current->max_speed)
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
					if (current->speed > (-1 * current->max_speed))
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
		
			diff = (ourtime - current->turn_t);
			tmp = TURN_SPEED;
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
			
			tmp = 1;
			while (diff >= tmp)
			{
				diff -= tmp;
				current->t += tmp;
				times += ship_movespeed_multiplier;
			}

			radius = blocksize / 2;
			ret = (PI / 180) * (current->deg - 90);

			for (cnt=0; cnt<times; cnt++)
			{
				int cret = 0;

				ret = collidecheck2(current->id, 1, 0);
				if (ret == 1)
				{
					// dont move
					break;
				}

				pos_x = futureX(current, current->deg, current->speed );
				pos_y = futureY(current, current->deg, current->speed );

				current->x = pos_x;
				current->y = pos_y;
				
				/* did the ship fly out the field? */
				if (current->y < 0) 
					current->y = field_height - 1;
				else if (current->y > field_height)
					current->y = 1;
				else if (current->x < 0)
					current->x = field_width - 1;
				else if (current->x > field_width)
					current->x = 1;

				collidecheck2(current->id, 0, 0); 
			}
			return;
		}
		current = current->next;
	}
}

/* ############### */
/* ### BULLETS ### */
/* ############### */

void draw_bullets() 
{
int groen = makecol(0, 255, 128);
int radius;	

	radius = blocksize / 2;

	current = head;
	while (current != NULL)
	{
		if (current->bullet == 1)
		{
			circlefill(bmp_shipfield , current->x - x_on_map, current->y - y_on_map, 
				1, makecol(255, 255, 0));
		}
		/* next bullet */
		current = current->next;
	}
}

struct data *add_bullet(struct data *_id, int _x, int _y, double _deg, unsigned long _time)
{
	/* create first node in linked list */
	if (!(new_node = (PLAYER)malloc(sizeof(user))))
	{   printff_direct("Error allocating memory");
		exit(-1);
	}

	/* insert the created node */
	new_node->next = head;
	head = new_node;
	/* intiate some values */
	head->owner = _id; /* owner's id */
	head->bullet = 1;
	head->TTL = our_bullet_ttl;
	head->x = _x;
	head->y = _y;
	head->start_x = _x;
	head->start_y = _y;

	head->deg = _deg;
	head->t = _time;
	head->vel_time = 0;
	head->turn_t = _time;

	head->impact = 0;
	return head;
}

void del_bullet(struct data *ptr)
{
	PLAYER old = NULL;
	PLAYER future = NULL;

	current = head;
	
	while (current != NULL)
	{
		future = current->next;
		if ((struct data *)current == ptr)
		{
			if (current->owner == our_node)
				our_bullet_count--;

			if (current == head)
			{
				/* first node */
				future = current->next;
				free(current);
				head = future;
				return;
			} else {
				if (current->next != NULL)
				{
					/* mid node */
					free(current);
					old->next = future;
					return;
				} else {
					/* last node */
					free(current);
					old->next = NULL;
					return;
				}
			}
		}
		old = current;
		current = current->next;
	}
}


void move_bullets()
{
	current = head;
	while (current != NULL)
	{

		if (current->bullet == 1)
		{
			if (move_bullet(current, ourtime) == 1)
			{
				current = head; /* easy solution ;) */
			}
		}

		current = current->next;
	}
}

int move_bullet(struct data *ptr, unsigned long t2)
{
	long int diff = 0, cnt;
	long int times = 0;
	double tmp = 0;
	int radius;
	double direction; 
	double ret;
	double pos_x, pos_y;

	current = head;

	while (current != NULL)
	{
		if ((struct data *)current == ptr)
		{
			if (current->bullet != 1)
				return 0;

			/* TTL */
			if ( (ourtime - current->turn_t) >= (unsigned)our_bullet_ttl)
			{
				del_bullet(current);
				return 1;
			}

			/* move this bullet */
			diff = (ourtime - current->t);
			
			tmp = 1;
			
			while (diff >= tmp)
			{
				diff -= tmp;
				current->t += tmp;
				// times += ( ship_movespeed_multiplier * ( blocksize / DEF_BLOCKSIZE ) );
				times += bullet_movespeed_multiplier;
			}

			radius = blocksize / 2;
			ret = (PI / 180) * (current->deg - 90);

			for (cnt=0; cnt<times; cnt++)
			{	

				if (collidecheck2b(current) == 1)
					return 1;

				direction = bullet_movespeed;

				pos_x = cos(ret);
				pos_x = pos_x * direction; /* ? */
				pos_x = pos_x + radius;
				pos_x = pos_x + current->x - (blocksize / 2);

				pos_y = sin(ret);
				pos_y = pos_y * direction;
				pos_y = pos_y + radius;
				pos_y = pos_y + current->y - (blocksize / 2);

				current->x = pos_x;
				current->y = pos_y;
				

				/* did the bullet fly out the field? */
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
				

				/* ship/bullet collition? */
				if (bs_collidecheck(current) == 1)
				{
					/* this bullet is now deleted */
					return 1;
				}
			}
			return 0;
		}
		current = current->next;
	}
	return 0;
}

/* ################## */
/* ### Explosions ### */
/* ################## */

void draw_explosions() 
{
	int diff, tmp, times, cnt;
	double current_x, current_y;
	EXPLOSION current;
	current = expl_head;

	while (current != NULL)
	{
		current_x = current->x - x_on_map;
		current_y = current->y - y_on_map;

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
				del_explosion(current);
				current = expl_head; /* again, easy easy.. TODO: change this 
								     pathatic solution */
				continue;
			}

		circle(bmp_shipfield, current_x, current_y, current->radius, current->color);
		current = current->next;
	}
}


struct data2 *add_explosion(double x, double y, int maxrad, int speed, int color)
{
	/* create first node in linked list */
	if (!(expl_new = (EXPLOSION)malloc(sizeof(_explosion))))
	{   printff_direct("Error allocating memory");
		exit(-1);
	}

	/* insert the created node */
	expl_new->next = expl_head;
	expl_head = expl_new;

	/* intiate some values */
	expl_head->x = x;
	expl_head->y = y;
	expl_head->color = color;
	expl_head->radius = 0;
	expl_head->cradius = 0;
	expl_head->maxradius = maxrad;
	expl_head->speed = speed; /* increase every x millisecs */
	expl_head->t = ourtime;

//	addtext("Created node addr: %d", expl_head);

	return expl_head;

}

void del_explosion(struct data2 *ptr)
{
EXPLOSION old = NULL;
EXPLOSION future = NULL;
EXPLOSION current = expl_head;

	while (current != NULL)
	{
		future = current->next;
		if (current == ptr)
		{
			if (current == expl_head)
			{
				/* first node */
				future = current->next;
				free(current);
				expl_head = future;
				return;
			} else {
				if (current->next != NULL)
				{
					/* mid node */
					free(current);
					old->next = future;
					return;
				} else {
					/* last node */
					free(current);
					old->next = NULL;
					return;
				}
			}
		}
		old = current;
		current = current->next;
	}
	return;
}

