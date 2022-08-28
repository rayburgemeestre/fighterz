#include "common.h"
//refactored 13 nov 2004

//ships:
//--
// void draw_ships();
// void move_ships();

//bullets:
//--
// void draw_bullets();
// struct data *add_bullet(struct data *owner, int x, int y, double deg, unsigned long time);
// void del_bullet(struct data *ptr);
// void move_bullets();
// int move_bullet(struct data *ptr);

//explosions:
//--
// void draw_explosions() ;
// struct data2 *add_explosion(double x, double y, int maxrad, int speed, int color);
// void del_explosion(struct data2 *ptr);

//variables:
//--

//bullets:
double bullet_movespeed; /**< bulletspeed: pixels to move each iteration e.g.: value=0.40(pixels) */

//linked list: players a.k.a. clients
PLAYER head            = NULL; /**< pointer to the first node in the linked list */
PLAYER new_node        = NULL; /**< pointer used for new nodes in the linked list */
PLAYER current         = NULL; /**< pointer to the current selected node in the linked list */
PLAYER our_node        = NULL; /**< pointer that always points to our node in the linked list */

//linked list: explosions (circular)
EXPLOSION expl_head    = NULL; /**< pointer to the first node in the explosion linked list */
EXPLOSION expl_new     = NULL; /**< pointer used for new nodes in the explosion linked list */
EXPLOSION expl_current = NULL; /**< pointer to the current selected node in the explosion linked list */

/* ***             *** */
/* ***    ships    *** */
/* ***             *** */

static double radius2 = -1;
void draw_ships()
{
int color;
if (radius2 == -1) {
    radius2 = blocksize / 2;
}
double radius = radius2;
double ret;
double pos_x, pos_y; /* ship's x,y */
double left_wing_deg, right_wing_deg;
double lw_x, lw_y; /* leftwing x,y */
double rw_x, rw_y; /* rightwing x,y */
double current_x, current_y; /* x & y to draw on the bmp_shipfield
								(ship x & y minus the map left+top) */

	for (current=head; current!=NULL; current=current->next)
	{
	current_x = current->x - x_on_map;
	current_y = current->y - y_on_map;

		/* if this node is an alive ship */
		if ((current->bullet != 1) && (current->dead == 0))
		{
			/* determine color of the ship (as a grid) */
			if (current->impact == 1)     color = makecol(255, 0, 0); /* red */
			else if (current->dead == 2)  color = makecol(128, 128, 128); /* grey */
			else if (current == our_node) color = makecol(255, 255, 255); /* white */
			else color = makecol(0, 255, 128); /* green */

			/* is this ship a bot */
			if (current->bot == 1)
			{
				if (!mod_grid)
				{
					rotate_sprite(bmp_shipfield, (BITMAP *)dat_base[SHIPS4].dat,
						current_x - (blocksize / 2), current_y - (blocksize / 2),
						itofix( (int)((current->deg*256)/360)) );

				} else {
					ret = (PI / 180) * (current->deg - 90);
					pos_x = futureX(current, current->deg, radius);
					pos_y = futureY(current, current->deg, radius);

					circle(bmp_shipfield, current_x, current_y, blocksize / 4, color);
					line(bmp_shipfield, current_x, current_y, pos_x, pos_y, color);
				}
			} else {

				if ((!mod_grid) && (current->shiptype != 6))
				{                //shiptype 6 = old school ship
				int shiptype;

					switch (current->shiptype)
					{
						case 1: shiptype = SHIPS1; break;
						case 2: shiptype = SHIPS2; break;
						case 3: shiptype = SHIPS3; break;
						case 4: shiptype = SHIPS4; break;
						case 5: shiptype = SHIPS5; break;
					}

					rotate_sprite(bmp_shipfield, (BITMAP *)dat_base[shiptype].dat,
						current_x - (blocksize / 2), current_y - (blocksize / 2),
						itofix( (int)((current->deg*256)/360)) );

					// display our ship in the radar too
					if (our_node == current)
						rotate_sprite(bmp_radar, (BITMAP *)dat_base[shiptype].dat,
							radar_shiptype_x, radar_shiptype_y, 0 );

				} else {
					/* ship coordinates */
					ret = (PI / 180) * (current->deg - 90);

					pos_x = cos(ret);
					pos_x *= (radius - (blocksize / 4));
					pos_x += radius;
					pos_x += current_x - (blocksize / 2);

					pos_y = sin(ret);
					pos_y *= (radius - (blocksize / 4));
					pos_y += radius;
					pos_y += current_y - (blocksize / 2);

					/* wing coordinates */
					left_wing_deg  = current->deg + 120;
					right_wing_deg = current->deg - 120;

					/* left wing */
					ret = (PI / 180) * (left_wing_deg - 90);
					lw_x = cos(ret);
					lw_x *= radius;
					lw_x += radius;
					lw_x += current_x - (blocksize / 2);

					lw_y = sin(ret);
					lw_y *= radius;
					lw_y += radius;
					lw_y += current_y - (blocksize / 2);

					/* right wing */
					ret = (PI / 180) * (right_wing_deg - 90);
					rw_x = cos(ret);
					rw_x *= radius;
					rw_x += radius;
					rw_x += current_x - (blocksize / 2);

					rw_y = sin(ret);
					rw_y *= radius;
					rw_y += radius;
					rw_y += current_y - (blocksize / 2);

					{// fill polygon with black
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
					//even more old school: line(bmp_shipfield , current_x, current_y, pos_x, pos_y, color);
					/* left wing */
					line(bmp_shipfield, pos_x, pos_y, lw_x, lw_y, color);
					line(bmp_shipfield, current_x, current_y, lw_x, lw_y, color);
					/* right wing */
					line(bmp_shipfield, pos_x, pos_y, rw_x, rw_y, color);
					line(bmp_shipfield, current_x, current_y, rw_x, rw_y, color);
				}

			}

			//temporary invincibility shield
            if (current->invincible == 1)
			    rect(bmp_shipfield, current_x - (blocksize / 2), current_y - (blocksize / 2),
			    	current_x + (blocksize / 2), current_y + (blocksize / 2), makecol(255,0,0));
			//godmode shield (unfair)
            else if (current->invincible == 2)
			    rect(bmp_shipfield, current_x - (blocksize / 2), current_y - (blocksize / 2),
			    	current_x + (blocksize / 2), current_y + (blocksize / 2), makecol(255,128,0));

			// show names underneath the player/bot ships
			if (show_names == 1)
			{
			int _bullets = ((our_bullet_max - our_bullet_count) * 100) / our_bullet_max;
			char tmpstr[128];
			int teamcolor = current->team == 1 ? makecol(255, 64, 64) : makecol(0, 186, 255);

			//	sprintf(tmpstr, "%s [%d] [%2.2f] [%d] [%2.2f] [%d]", current->nick, current->velocity,
			//				current->speed, current->freeze, current->deg, (int)current->team);
				sprintf(tmpstr, "%s", current->nick);
				textprintf_centre(bmp_shipfield, (FONT *)dat_base[NOKIA].dat, current_x,
					current_y + (blocksize/2) + 3, teamcolor, "%s", tmpstr);
			}
		}
	}
}

void move_ships()
{
	for (current=head; current!=NULL; current=current->next)
	{
		// if node is not a bullet
		if (current->bullet != 1)
		{
 			move_ship(current);

			/* something to remove the 'red' glow of
			   hit ships after 200 ms */
			if ( (ourtime - current->hit_t) >= 200)
				current->impact = 0;
		}
	}
}

static int radius3 = -1;
void move_ship(struct data *client)
{
long int diff = 0, cnt;
long int times = 0;
double tmp = 0;
if (radius3 == -1) { radius3 = blocksize / 2; }
int radius = radius3;
double ret;
double pos_x, pos_y;

	//precondition: no bullets
	if (client->bullet == 1)
		return;

// #ACCELERATION PROCEDURE#

	diff  = (ourtime - client->vel_time);
	tmp   = ACCEL_SPEED;
	times = 0;
	while (diff >= tmp)
	{
		diff -= tmp;
		client->vel_time += tmp;
		times += 1;
	}

	for (cnt=0; cnt<times; cnt++)
	{
		if (client->velocity == 1) // speed must become maxspeed forwards
		{
			if (client->speed < client->max_speed)
				client->speed += 0.01;
		}
		else if (client->velocity == 0.00) // speed must become zero
		{
			if (client->speed < -0.001)
			{// we don't want -0.00 to valid this statement
				client->speed += 0.01;
				if (client->speed == 0.00)
					client->speed = 0.00;
			}
			else if (client->speed > 0.001) // 0.0000 > 0.00
				client->speed -= 0.01;		// (therefore the vague extra 0.001)
		}
		else if (client->velocity == -1) // speed must become maxspeed backwards
		{
			if (client->speed > (-1 * client->max_speed))
				client->speed -= 0.01;
		}
		else
			; // should never be reached
	}

// #TURN PROCEDURE#

	times = 0;
	tmp   = 0;

	diff = (ourtime - client->turn_t);
	tmp = TURN_SPEED;
	while (diff >= tmp)
	{
		diff -= tmp;
		client->turn_t += tmp;
		times += 1;
	}

	// for the other ships
	for (cnt=0; cnt<times; cnt++)
	{
		switch (client->turn)
		{
		case 2: // slow turn right
		case 4: // fast turn right
			client->deg += client->turn;
			if ( client->deg > 360) client->deg -= 360;
			break;
		case -2: // slow turn left
		case -4: // fast turn left
			client->deg += client->turn;
			if ( client->deg < 0 ) client->deg += 360;
			break;
		case 0: // no turning
			client->turn_t = ourtime + diff;
			break;
		default:
			// this should not be reached.
			break;
		}
	}

// #MOVE PROCEDURE#

	times = 0;
	diff = (ourtime - client->t);
	tmp = 1;

	while (diff >= tmp)
	{
		diff -= tmp;
		client->t += tmp;
		times += ship_movespeed_multiplier;
	}

	ret = (PI / 180) * (client->deg - 90);

	for (cnt=0; cnt<times; cnt++)
	{
	int cret = 0;

		ret = collidecheck2(client->id, 1, 0);
		if (ret == 1)
			break; // dont move, because the next move would collide

		pos_x = futureX(client, client->deg, client->speed );
		pos_y = futureY(client, client->deg, client->speed );

		client->x = pos_x;
		client->y = pos_y;

		/* did the ship fly out the field? */
		if (client->y < 0)
			client->y = field_height - 1;
		else if (client->y > field_height)
			client->y = 1;
		else if (client->x < 0)
			client->x = field_width - 1;
		else if (client->x > field_width)
			client->x = 1;

		collidecheck2(client->id, 0, 0);
	}
	return;
}

/* ***               *** */
/* ***    bullets    *** */
/* ***               *** */

void draw_bullets()
{
int groen = makecol(0, 255, 128);
int radius = blocksize / 2;

	for (current=head; current!=NULL; current=current->next)
	{
		if (current->bullet == 1)
		{
			circlefill(bmp_shipfield , current->x - x_on_map, current->y - y_on_map,
				1, makecol(255, 255, 0));
		}
	}
}

struct data *add_bullet(struct data *owner, int x, int y, double deg, unsigned long time)
{
	/* create first node in linked list */
	if (!(new_node = (PLAYER)malloc(sizeof(user))))
	{
		printff_direct("Error allocating memory");
		exit(-1);
	}

	/* insert the created node */
	new_node->next = head;
	head           = new_node;

	/* intiate some values */
	head->owner    = owner;
	head->bullet   = 1;
	head->TTL      = our_bullet_ttl;
	head->x        = x;
	head->y        = y;
	head->start_x  = x;
	head->start_y  = y;
	head->deg      = deg;
	head->t        = time;
	head->turn_t   = time;
	head->vel_time = 0;
	head->impact   = 0;

	return head;
}

void del_bullet(struct data *ptr)
{
	PLAYER old = NULL;
	PLAYER future = NULL;

	for (current=head; current; current=current->next)
	{
		future = current->next;
		if ((struct data *)current == ptr)
		{
			if (current->owner == our_node)
				our_bullet_count--;

			if (current == head)
			{/* first node */
				future = current->next;
				free(current);
				head = future;
				return;
			} else {
				if (current->next != NULL)
				{/* mid node */
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
	}
}

void move_bullets()
{
// PLAYER previous = head;

	for (current=head; current; current=current->next)
	{
		if (current->bullet == 1)
		{
			// if move_bullet() returns 1 (the bullet is gone now):
			// start over at first node, because the current pointer
			// may reference an illegal address
			if (move_bullet(current, ourtime) == 1)
			{
				current = head;
			}
		}
	}
}

static int radius = -1;
int move_bullet(struct data *bullet, unsigned long t2)
{
long int diff = 0, cnt;
long int times = 0;
double tmp = 0;
if (radius == -1) radius = blocksize / 2;

double direction;
double ret;
double pos_x, pos_y;

	// precondition not a bullet
	if (bullet->bullet != 1)
		return 0;

	// check if bullet lifetime exceeded TTL
	if ( (ourtime - bullet->turn_t) >= (unsigned)our_bullet_ttl)
	{
		del_bullet(bullet);
		return 1; // TTL passed, bullet died here
	}

// #MOVE PROCEDURE#

	diff = (ourtime - bullet->t);
	tmp = 1;

	while (diff >= tmp)
	{
		diff -= tmp;
		bullet->t += tmp;
		times += bullet_movespeed_multiplier;
	}

	ret = (PI / 180) * (bullet->deg - 90);

	for (cnt=0; cnt<times; cnt++)
	{
		// if collidecheck2b sees the bullet collides (with some wall)
		// it will delete it and return 1
		if (collidecheck2b(bullet) == 1)
			return 1; // so we return 1 as well, because this bullet is dead.

		direction = bullet_movespeed;

		pos_x = cos(ret);
		pos_x *= direction;
		pos_x += radius;
		pos_x += bullet->x - (blocksize / 2);

		pos_y = sin(ret);
		pos_y *= direction;
		pos_y += radius;
		pos_y += bullet->y - (blocksize / 2);

		bullet->x = pos_x;
		bullet->y = pos_y;

		/* did the bullet fly out the field? */
		if (bullet->y < 0)
		{	// *warp*
			bullet->y = field_height - 1;
		} else if (bullet->y > field_height) {
			bullet->y = 1;
		} else if (bullet->x < 0) {
			bullet->x = field_width - 1;
		} else if (bullet->x > field_width) {
			bullet->x = 1;
		}

		/* ship/bullet collition? bs_collidecheck handles the event
		   and deletes the bullet itself */
		if (bs_collidecheck(bullet) == 1)
			return 1; // this bullet is now dead
	}
	return 0; // nothing collided, bullet is still there.
}

/* ***                  *** */
/* ***    Explosions    *** */
/* ***                  *** */

void draw_explosions()
{
int diff, tmp, times, cnt;
double current_x, current_y;
EXPLOSION current;

	for (current=expl_head; current; )
	{
		// the x,y on the bmp_shipfield (so ship x,y minus the
		// left/top padding of the map)
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
			// the explosion has ended, it will be deleted
			// and the current node is (re)set to 'head', because
			// current won't refer a valid address after this.
			del_explosion(current);
			current = expl_head;
			continue; // we cannot draw a deleted explosion!
		}
		circle(bmp_shipfield, current_x, current_y, current->radius, current->color);
		current = current->next; // can't leave this in for loop
		// maybe after the continue statement a few lines back
		// the incrementing part of the for loop is triggered first
	}
}

struct data2 *add_explosion(double x, double y, int maxrad, int speed, int color)
{
	/* create first node in linked list */
	if (!(expl_new = (EXPLOSION)malloc(sizeof(_explosion))))
	{
		printff_direct("Error allocating memory");
		exit(-1);
	}

	/* insert the created node */
	expl_new->next       = expl_head;
	expl_head            = expl_new;

	/* intiate some values */
	expl_head->x         = x;
	expl_head->y         = y;
	expl_head->color     = color;
	expl_head->radius    = 0;
	expl_head->cradius   = 0;
	expl_head->maxradius = maxrad;
	expl_head->speed     = speed; /* increase every x millisecs */
	expl_head->t         = ourtime;

	return expl_head;
}

void del_explosion(struct data2 *ptr)
{
EXPLOSION old     = NULL;
EXPLOSION future  = NULL;
EXPLOSION current = expl_head;

	while (current != NULL)
	{
		future = current->next;
		if (current == ptr)
		{
			if (current == expl_head)
			{/* first node */
				future = current->next;
				free(current);
				expl_head = future;
				return;
			} else {
				if (current->next != NULL)
				{/* mid node */
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

