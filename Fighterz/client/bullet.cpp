#include "common.h"

/********************************************
*  BULLET FUNCTIONS
****************************************************/

struct data *add_bullet(struct data *_id, int _x, int _y, double _deg, unsigned long _time)
{
	/* create first node in linked list */
	if (!(new_node = (LINK)malloc(sizeof(user))))
	{   printff_direct("Error allocating memory");
		exit(-1);
	}

	/* insert the created node */
	new_node->next = head;
	head = new_node;
	/* intiate some values */
	head->owner = _id; /* owner's id */
	head->bullet = 1;
	head->TTL = BULLET_TTL;
	head->x = _x;
	head->y = _y;
	head->start_x = _x;
	head->start_y = _y;

	head->deg = _deg;
	head->t = _time;
	head->vel_time = 0;
	head->turn_t = _time; /*
			when bullet, used as creation time
  */

	head->impact = 0;

	return head;
}

void del_bullet(struct data *ptr)
{
	LINK old = NULL;
	LINK future = NULL;

	current = head;
	
	while (current != NULL)
	{
		future = current->next;
		if ((struct data *)current == ptr)
		{
			if (current->owner == our_node)
				BULLET_COUNT--;

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


void movebullets()
{
	current = head;
	while (current != NULL)
	{

		if (current->bullet == 1)
		{
			if (movebullet(current, ourtime) == 1)
			{
				current = head; /* easy solution ;) */
			}
		}

		current = current->next;
	}
}

int movebullet(struct data *ptr, unsigned long t2)
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
			if ( (ourtime - current->turn_t) >= (unsigned)BULLET_TTL)
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
				// times += ( MOVE_STEPW * ( BLOCKSIZE / DEF_BLOCKSIZE ) );
				times += MOVE_BSTEPW;
			}

			radius = BLOCKSIZE / 2;
			ret = (PI / 180) * (current->deg - 90);

			for (cnt=0; cnt<times; cnt++)
			{	

				if (collidecheck2b(current) == 1)
					return 1;

				direction = B_SPEED;

				pos_x = cos(ret);
				pos_x = pos_x * direction; /* ? */
				pos_x = pos_x + radius;
				pos_x = pos_x + current->x - (BLOCKSIZE / 2);

				pos_y = sin(ret);
				pos_y = pos_y * direction;
				pos_y = pos_y + radius;
				pos_y = pos_y + current->y - (BLOCKSIZE / 2);

				current->x = pos_x;
				current->y = pos_y;
				//current->x2 = current->x * (1.0 + ZOOM);
				//current->x2 = current->x * (1.0 + ZOOM);
				setsec(current); /* recent */

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



void drawbullets() 
{
	int groen = makecol(0, 255, 128);
	/* BLOCKSIZE = diameter */
	int radius;	

	radius = BLOCKSIZE / 2;

	current = head;
	while (current != NULL)
	{
		if (current->bullet == 1)
		{
			/**********************
			 **     DRAW BULLET  **
			 **********************/
			
			circlefill(shipbuff , current->x2, current->y2, 1, makecol(255, 255, 0));
		}
		/* next ship */
		current = current->next;
	}
}