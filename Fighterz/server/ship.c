#include "common.h"


/* ALMOST EXACT DUPLICATE OF THE FUNCTIONS IN THE CLIENT */

void moveships()
{
LINK current = head;
	while (current != NULL)
	{
		if (current->bullet != 1)
		{
			if ( (current->velocity == 1) || (current->velocity == 2) ||
				 (current->velocity == -1) || (current->velocity == -2) || (current->velocity == 0) )
			{
				/* Nice moment to check if invinsibility should be turned off :) */
				if (current->invincible == 1)
				{
					if ((servertime - current->invincibility_t2) > 
						(TIME)current->invincibility_t )
					{
						/* No longer invincible */
						current->invincible = 0;
					}
				}
				/* Also a nice moment to check if this dude may be respawned */
				if (current->dead == 1)
				{
					int diff;
					diff = servertime - current->died;
					
					if ( diff > (signed)(RESPAWN_DELAY * 1000) )
					{
						if (IsPlayer(current) && IsRegistered(current))
						{
							/* Only mark 'allowed to respawn' and send the SPAWNREADY msg */
							send_spawnready(current);
							current->dead = 2;
						}
						else if (IsBot(current))
						{
							current->invincible = 1;
							current->invincibility_t = 5000;
							current->invincibility_t2 = servertime;
							current->x = (double)(BLUE.x + (BLOCKSIZE / 2));
							current->y = (double)(BLUE.y + (BLOCKSIZE / 2));
							current->dead = 0;
							notify_of_respawn(EVERYONE, NULL, current);
							flyto(current->id, REDFLAG.x, REDFLAG.y);
						}
					}
				}

				// addtext("moving: %s", (current->bot == 1?"bot":"human") );
				moveship(current->id, servertime);
			}
		}
		current = current->next;
	}
}

#define TURN_SPEED  10// 18
#define ACCEL_SPEED 30
void moveship(unsigned int id2, TIME t2)
{								/* t2 wasn't needed :} */
	long int diff = 0, cnt;
	long int times = 0;
	int tmp = 0;
	int radius;
	double direction; 
	double ret;
	double pos_x, pos_y;
	LINK current = head;

	while (current != NULL)
	{
		if (current->bullet != 1)
		{
			if ((unsigned)current->id == id2)
			{

				/* NEW */
				diff = (servertime - current->turn_t);
				tmp = TURN_SPEED; /* Turn speed */
				while (diff >= tmp)
				{
					diff -= tmp;
					current->turn_t += tmp;
					times += 1;
				}

				// for the other ships
				for (cnt=0; cnt<times; cnt++)
				{
					if (current->turn == 0) {
						current->turn_t = servertime + diff;
						/* Todo? break; Nothing to do anyway */
					}
					if (current->turn > 0) {
						current->deg += current->turn;
						if (current->deg > 360)
							current->deg = (double)current->turn;
					} else if (current->turn < 0) {
						current->deg += current->turn;
						if (current->deg < 0)
							current->deg = 358.0; // 360 - current->deg ??
					}
				}



diff = (servertime - current->vel_time);
tmp = ACCEL_SPEED; /* Accel speed */
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
		{
			current->speed -= 0.01;
		}
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
		;

/*
---
--- Start: Old
---
	//addtext("XXXXXXXXXXXXXXXXXXXXXXX");
	if (current->velocity == 1)
	{ 
		if (current->speed < SPEED)
			current->speed += 0.01;

		//addtext("%20lu Speed atm: %.2f", servertime, current->speed);
	}
	if (current->velocity == -1)
	{
		if (current->speed > (-1 * SPEED))
			current->speed -= 0.01;

		//addtext("%20lu Speed atm: %.2f", servertime, current->speed);
	}
//	else
//		; // current->vel_time += diff;

---
--- End: Old
---
*/
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

				/* /NEW */

				/* move this ship*/
				if (current->t > t2)
				{	/* time is not synched correctly */
					/* todo: call function to sync again*/
					// addtext("ERROR: Time out of sync for client: %d", current->id);
					return;
				}
				diff = (servertime - current->t);
				
			// added
			// --
				if (current->freeze == 1)
				{
					current->t = servertime + diff;
					return;
					/* we don't move because we THINK (and are probably right)
					   that this person has collided with some wall.. */
				}
			// --
				//if ( (current->velocity == 1) || (current->velocity == -1) )
				//{
				//	//tmp = MOVE_INTERVAL * 2;
				//	tmp = 2;
				//} else if ((current->velocity == 2) || (current->velocity == -2)) 
				//{
				//	//tmp = MOVE_INTERVAL;
				//	tmp = 1;
				//} else {
				//	current->t = servertime + diff;
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
					if (current->bot == 1)
					{

						if (current == NULL)
						{
							printf("Hier klopt geen reet van\n");
							return;
						}

						//alert("hihi", "","","","",1,1);
						if (current->path[PATH_MAX_-1][1] < current->path[PATH_MAX_-1][0])
						{
							int a;
							double tx, ty;
							double ox, oy;
							double fx, fy; /* futurex, /y */
							double diffx1, diffx2, diffy1, diffy2;

							a = (int)(current->path[PATH_MAX_-1][1]);


							tx = current->path[a][0];
							ty = current->path[a][1];

							//circlefill(fieldbuff, tx, ty, 5, makecol(0,255,0));
							//circlefill(fieldbuff, tx + field_width, ty + field_height, 5, makecol(0,255,0));
							//circlefill(fieldbuff, tx - field_width, ty + field_height, 5, makecol(0,255,0));
							//circlefill(fieldbuff, tx + field_width, ty - field_height, 5, makecol(0,255,0));
							//circlefill(fieldbuff, tx - field_width, ty - field_height, 5, makecol(0,255,255));

							// addtext("Tx: %.2f Ty: %.2f 1;%.2f 2;%.2f   ? %.2f", head->bot_x, head->bot_y, head->x, head->y, head->y);

							/* We're following a path :) */
							
							fx = futureX(current, current->deg, current->speed);
							fy = futureY(current, current->deg, current->speed);
							
							diffx1 = dabs(current->bot_x - tx);
							diffx2 = dabs(fx - tx);
							diffy1 = dabs(current->bot_y - ty);
							diffy2 = dabs(fy - ty);
							
							//fprintf(juub, "x1;%.2f x2;%.2f y1;%.2f y2;%.2f  x: %.2f %.2f\n", diffx1, diffx2, diffy1, diffy2, current->bot_x, fx);

							/* If coordinates between ship and the target (tx/ty) will
							increase with the next step .. :) */
							if ((diffx2 >= diffx1 &&
								diffy2 >= diffy1) && current->speed != 0.0)
							{
								
								ox = current->path[(int)(current->path[PATH_MAX_-1][1])][0];
								oy = current->path[(int)(current->path[PATH_MAX_-1][1])][1];

								current->path[PATH_MAX_-1][1] = current->path[PATH_MAX_-1][1] + 1.0;

								addtext("Done path %.2f of %.2f", current->path[PATH_MAX_-1][1], current->path[PATH_MAX_-1][0]);

								//if (current->path[PATH_MAX-1][1] != current->path[PATH_MAX-1][0])
								//{
									
								// current->velocity = 0;		

									tx = current->path[(int)(current->path[PATH_MAX_-1][1])][0];
									ty = current->path[(int)(current->path[PATH_MAX_-1][1])][1];

									current->x -= (current->bot_x - ox);
									current->y -= (current->bot_y - oy);

									current->bot_x = ox;
									current->bot_y = oy;

									if ( (current->path[PATH_MAX_-1][0] - current->path[PATH_MAX_-1][1]) == 0)
									{
										//current->deg = getangle(
										//	endc * BLOCKSIZE + (BLOCKSIZE / 2),
										//	endr * BLOCKSIZE + (BLOCKSIZE / 2),
										//	current->bot_x,
										//	current->bot_y
										//); Hmmm
										current->velocity = 0;
										current->speed = 0.0;

										/* Acceleration update */
										send_accel(EVERYONE, NULL, current->id, current->x, current->y, (signed char)current->velocity, current->speed);
#ifndef NOT_DEFINED
										// flyto random place
										{
											int rand_x, rand_y, tx, ty;
											int continue_loop = 0;
											do
											{
												rand_x = 1+(int) (field_width*rand()/(RAND_MAX+1.0));
												rand_y = 1+(int) (field_height*rand()/(RAND_MAX+1.0));

												tx = (int) ((rand_x - (BLOCKSIZE / 4)) / BLOCKSIZE);
												ty = (int) ((rand_y - (BLOCKSIZE / 4)) / BLOCKSIZE);
											} while (field[ty][tx] == '1');

											flyto(current->id, rand_x, rand_y);
										}
#endif
										/* XXXXXXZ */
										//circlefill(fieldbuff, current->target_x, current->target_y, 20, makecol(255,255,255));
										//circlefill(fieldbuff, current->target_x - field_width, current->target_y - field_height, 20, makecol(255,255,255));

									} else {
										current->deg = getangle(
											tx,
											ty,
											current->bot_x,
											current->bot_y	
										);

										/* Direction update */
										/* [TODO: Is this right? turn wasn't sent before ;P] */
										send_turn(EVERYONE, NULL, current->id, current->x, current->y, (signed char)current->turn, current->deg);
									}

								//}
							}
						}
						//printf("Checking id: %d (bot: %d)\n", current->id, current->bot);
						//fflush(stdout);
						//fflush(stdin);
						collidecheck2(current, 0);
						// heh foutje @ collidecheck2b(current);
					} // if (current->bot == 1)
					else if (current->bot == 0)
					{
						collidecheck2(current, 0);
					}

					/* collidecheck2(current);
					do not collide check at the server, stuff could
					be out of sync, and that would mess up some things */
						

					//if (current->velocity == 0) break;

				//	if ((current->velocity) < 0)
				//	{
				//		direction = SPEED * -1; ** pixzls to move :) **
				//	} else {
				//		direction = SPEED;
				//	}

					direction = current->speed;

					pos_x = cos(ret);
					pos_x = pos_x * direction; /* ? */
					pos_x = pos_x + radius;
					pos_x = pos_x + current->x - (BLOCKSIZE / 2);
						current->x = pos_x;
					pos_x = cos(ret);
					pos_x = pos_x * direction; /* ? */
					pos_x = pos_x + radius;
					pos_x = pos_x + current->bot_x - (BLOCKSIZE / 2);
						current->bot_x = pos_x;

					pos_y = sin(ret);
					pos_y = pos_y * direction;
					pos_y = pos_y + radius;
					pos_y = pos_y + current->y - (BLOCKSIZE / 2);
						current->y = pos_y;
					
					pos_y = sin(ret);
					pos_y = pos_y * direction;
					pos_y = pos_y + radius;
					pos_y = pos_y + current->bot_y - (BLOCKSIZE / 2);
						current->bot_y = pos_y;

					//addtext("set to: y: %.2f", pos_y);
					
					/* did the ship fly out the field? */
					if (current->y < 0) 
					{	// *warp*
						current->y = (double)(field_height - 1);
					} else if ((unsigned long)current->y > field_height) {
						current->y = 1.0;
					} else if (current->x < 0) {
						current->x = (double)(field_width - 1);
					} else if ((unsigned long)current->x > field_width) {
						current->x = 1.0;
					}
				}

#if DEBUG == 1
				fprintf(outp, "Current position of ID:%u is X:%2.2f Y:%2.2f DEG:%d VEL:%d\n", 
					(unsigned int)current->id, current->x, current->y, current->deg, current->velocity);
				fflush(outp);
#endif

				return;
			}
		}
		current = current->next;
	}
}

#if DEBUG2 == 1
void drawships() 
{
	int groen = makecol(0, 255, 128);
	/* BLOCKSIZE = diameter */
	int radius = BLOCKSIZE / 2;	
	double ret;
	double pos_x, pos_y; /* ship's x,y */
	int i;
	LINK current = head;

	clear_to_color (shipbuff, 0);

	while (current != NULL)
	{
		if (current->bullet != 1 && current->dead != 1)
		{			
			if (current->bot == 1)
			{
				ret = (PI / 180) * (current->deg - 90);
				pos_x = fbX(current->id);
				pos_y = fbY(current->id);

				circle(shipbuff, (int)current->x, (int)current->y, BLOCKSIZE / 4, makecol(255,255,255));
				//circle(shipbuff, pos_x, pos_y, BLOCKSIZE / 4, makecol(255,128,0));
				line(shipbuff, (int)current->x, (int)current->y, (int)pos_x, (int)pos_y, makecol(255,255,255));

                if (current->invincible != 0)
				    rect(shipbuff, (int)(current->x - (BLOCKSIZE / 2)), (int)(current->y - (BLOCKSIZE / 2)), 
				    	(int)(current->x + (BLOCKSIZE / 2)), (int)(current->y + (BLOCKSIZE / 2)), makecol(255,0,0));				

				if (0)
				{
					i = 0;
					while (i < current->path[PATH_MAX_-1][0])
					{
						circlefill(shipbuff, (int)current->path[i][0], (int)current->path[i][1]-field_height, 5, makecol(0,0,255));
						i++;
					}
				}

			} else {
				double left_wing_deg, right_wing_deg; 
				double lw_x, lw_y; /* leftwing x,y */
				double rw_x, rw_y; /* rightwing x,y */
				/**********************
				 **   CALCULATIONS   **
				 **********************/

				/* ship coordinates */
				ret = (PI / 180) * (current->deg - 90);

				pos_x = cos(ret);
				pos_x = pos_x * (radius - (BLOCKSIZE / 4));
				pos_x = pos_x + radius;
				pos_x = pos_x + current->x - (BLOCKSIZE / 2);

				pos_y = sin(ret);
				pos_y = pos_y * (radius - (BLOCKSIZE / 4));
				pos_y = pos_y + radius;
				pos_y = pos_y + current->y - (BLOCKSIZE / 2);

				/* wing coordinates */
				left_wing_deg = current->deg + 120;
				right_wing_deg = current->deg - 120;

				/* left wing */
				ret = (PI / 180) * (left_wing_deg - 90);
				lw_x = cos(ret);
				lw_x = lw_x * radius;
				lw_x = lw_x + radius;
				lw_x = lw_x + current->x - (BLOCKSIZE / 2);

				lw_y = sin(ret);
				lw_y = lw_y * radius;
				lw_y = lw_y + radius;
				lw_y = lw_y + current->y - (BLOCKSIZE / 2);

				/* right wing */
				ret = (PI / 180) * (right_wing_deg - 90);
				rw_x = cos(ret);
				rw_x = rw_x * radius;
				rw_x = rw_x + radius;
				rw_x = rw_x + current->x - (BLOCKSIZE / 2);

				rw_y = sin(ret);
				rw_y = rw_y * radius;
				rw_y = rw_y + radius;
				rw_y = rw_y + current->y - (BLOCKSIZE / 2);

				/**********************
				 **     DRAW SHIP    **
				 **********************/
				/* ship base */
				line(shipbuff, (int)current->x, (int)current->y, (int)pos_x, (int)pos_y, groen);
				/* left wing */
				line(shipbuff, (int)pos_x,  (int)pos_y, (int)lw_x, (int)lw_y, groen);
				line(shipbuff, (int)current->x, (int)current->y, (int)lw_x, (int)lw_y, groen);
				if (current->invincible != 0)
				    rect(shipbuff, (int)(current->x - (BLOCKSIZE / 2)), (int)(current->y - (BLOCKSIZE / 2)), 
				    	(int)(current->x + (BLOCKSIZE / 2)), (int)(current->y + (BLOCKSIZE / 2)), makecol(255,0,0));

				/* right wing */
				line(shipbuff, (int)pos_x, (int)pos_y, (int)rw_x, (int)rw_y, groen);
				line(shipbuff, (int)current->x, (int)current->y, (int)rw_x, (int)rw_y, groen);
			}

		}
		/* next ship */
		current = current->next;
	}
}
#endif

void flyto(int id, double x, double y)
{
	LINK tmp;
	double tx, ty;

	tmp = head;
	while (tmp != NULL && (signed)tmp->id != id)
		tmp = tmp->next;
	
	if (tmp == NULL)
		exit(-2);
		
	if ( (tmp->x <= (double)(field_width / 2)) && (tmp->y <= (double)(field_height / 2)))
	{
		//addtext("We are 1");
		extra_x = (int)field_width;
		extra_y = (int)field_height;
	}
	else if ( (tmp->x > (double)(field_width / 2)) && (tmp->y <= (double)(field_height / 2)))
	{
		//addtext("We are 2");
		extra_x = 0;
		extra_y = (int)field_height;
	}
	else if ( (tmp->x <= (double)(field_width / 2)) && (tmp->y > (double)(field_height / 2)))
	{
		//addtext("We are 3");
		extra_x = (int)field_width;
		extra_y = (int)field_height;
	}
	else if ( (tmp->x > (double)(field_width / 2)) && (tmp->y > (double)(field_height / 2)))
	{
		//addtext("We are 4");
		extra_x = 0;
		extra_y = 0;
	}

	findpath(tmp, x, y);

//return;
// he no fly!

	/* bot shall fly :) */
	tmp->path[PATH_MAX_-1][1] = 0;

	tx = tmp->path[(int)(tmp->path[PATH_MAX_-1][1])][0];
	ty = tmp->path[(int)(tmp->path[PATH_MAX_-1][1])][1];

	tmp->deg = getangle(
		tx,
		ty,
		tmp->bot_x,
		tmp->bot_y
	);
	// tmp->velocity = 2;
	tmp->velocity = 1;
	tmp->speed = 0.00;
	verbose("flyto speed @ max: %.2f" , tmp->max_speed);
	
	send_turn(EVERYONE, NULL, tmp->id, tmp->x, tmp->y, (signed char)tmp->turn, tmp->deg);
	send_accel(EVERYONE, NULL, tmp->id, tmp->x, tmp->y, (signed char)tmp->velocity, tmp->speed);
}
