#include "common.h"

/********************************************
 *  BULLET FUNCTIONS
 ****************************************************/

struct data *add_bullet(struct data *_owner, double _x, double _y, double _deg, TIME _time) {
  LINK ownr, new_node;
  ownr = _owner;

  /* create first node in linked list */
  if (!(new_node = (LINK)malloc(sizeof(user)))) {
    printf_("Error allocating memory");
    exit(-1);
  }
  // addtext("Adding bullet at x:%2.2f y:%2.2f id:%d", _x, _y, 0);

  /* intiate some values */
  new_node->owner = _owner; /* owner's id */

  new_node->bullet = 1;
  new_node->TTL = BULLET_TTL;
  ownr->bulletcnt = (ownr->bulletcnt + 1);
  new_node->x = _x;
  new_node->y = _y;
  new_node->deg = _deg;
  new_node->t = _time;
  new_node->turn_t = _time; /*
                  when bullet, used as creation time
*/
  new_node->velocity = 1;
  new_node->speed = 0.10;
  new_node->lag[0] = 0;
  new_node->lag[1] = 0;
  new_node->lag[2] = 0;
  new_node->lag[3] = 0;
  new_node->lag[4] = 0;
  strcpy(new_node->nick, "__Bullet__");

  new_node->type = T_BULLET;
  /* insert the created node */
  new_node->next = head;
  head = new_node;

  return head;
}

void del_bullet(struct data *ptr) {
  LINK old = NULL;
  LINK future = NULL;
  LINK current = head;

  while (current != NULL) {
    future = current->next;
    if ((struct data *)current == ptr) {
      if (current == head) {
        /* first node */
        future = current->next;
#if DEBUG == 1
        fprintf(outp, "FREEINGa current(%s) at addr: %d\n", current->nick, current);
        fflush(stdout);
#endif
        free(current);
        head = future;
        return;
      } else {
        if (current->next != NULL) {
          /* mid node */
#if DEBUG == 1
          fprintf(outp, "FREEINGb current(%s) at addr: %d\n", current->nick, current);
          fflush(stdout);
#endif
          free(current);
          old->next = future;
          return;
        } else {
          /* last node */
#if DEBUG == 1
          fprintf(outp, "FREEINGc current(%s) at addr: %d\n", current->nick, current);
          fflush(stdout);
#endif
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

void del_bullets(ID sock_fd) {
  LINK tmp = head;
  LINK tmp2 = head;
  LINK old = NULL;
  int i = 0;

  while (tmp != NULL) {
    if (tmp->bullet != 1)
      if (tmp->id == sock_fd) break;
    tmp = tmp->next;
  }
  if (tmp == NULL) return;

  while (tmp2 != NULL) {
    i = 0;
    if (tmp2->bullet == 1) {
      if (tmp2->owner == tmp) {
#if DEBUG == 1
        fprintf(outp, "deletinga bullet(%d) at addr: %d\n", tmp2->id, tmp2);
        fflush(stdout);
#endif
        tmp->bulletcnt--;
        del_bullet(tmp2);
        i = 1;
        if (old != NULL)
          tmp2 = old;
        else
          tmp2 = head;
      }
    }
    if (i == 0) {
      old = tmp2;
      tmp2 = tmp2->next;
    }
  }
}

void movebullets() {
  LINK current = head;

  while (current != NULL) {
    if (current->bullet == 1) {
      if (movebullet(current) == 1) {
        current = head; /* easy solution ;) */
      }
    }
    if (current != NULL) current = current->next;
  }
}

/*
 * movebullet: moves the bullet and removes it if necessary
 * returns 1 if bullet has been removed, ptr is then NO LONGER VALID.
 */
int movebullet(struct data *ptr) {
  TIME diff = 0, tmp = 0;
  long int times = 0, cnt;
  int radius;
  double direction;
  double ret;
  double pos_x, pos_y;
  LINK current = head;

  while (current != NULL) {
    if ((struct data *)current == ptr) {
      if (current->bullet != 1) return 0;

      /* TTL */
      if ((servertime - current->turn_t) >= BULLET_TTL) {
        LINK tmp;
        tmp = head;
        while (tmp != NULL) {
          if (tmp == current->owner) tmp->bulletcnt--;

          tmp = tmp->next;
        }
#if DEBUG == 1
        fprintf(outp, "deletingb bullet(%d) at addr: %d\n", current->id, current);
        fflush(stdout);
#endif
        current->owner->bulletcnt--;
        del_bullet(current);

        return 1;
      }

      /* move this bullet */
      diff = (servertime - current->t);

      tmp = 1;

      while (diff >= tmp) {
        diff -= (long)tmp;
        current->t += (unsigned long)tmp;
        // times += ( MOVE_STEPW * ( BLOCKSIZE / DEF_BLOCKSIZE ) );
        times += MOVE_BSTEPW;
      }

      radius = BLOCKSIZE / 2;
      ret = (PI / 180) * (current->deg - 90);

      for (cnt = 0; cnt < times; cnt++) {
        if (collidecheck2b(current) == 1) return 1;

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

        /* did the bullet fly out the field? */
        if (current->y < 0) {  // *warp*
          current->y = (double)(field_height - 1);
        } else if (current->y > (double)field_height) {
          current->y = 1.0;
        } else if (current->x < 0) {
          current->x = (double)(field_width - 1);
        } else if (current->x > (double)field_width) {
          current->x = 1.0;
        }

        if (bs_collidecheck(current) == 1) {
          /* Collided and the bullet is removed */
          return 1;
        }
      }

      return 0;
    }
    current = current->next;
  }
  return 0;
}

#if DEBUG2 == 1
void drawbullets() {
  int groen = makecol(0, 255, 128);
  /* BLOCKSIZE = diameter */
  int radius;
  double ret;
  double pos_x, pos_y; /* ship's x,y */
  LINK current = head;

  radius = BLOCKSIZE / 2;

  while (current != NULL) {
    if (current->bullet == 1) {
      /**********************
       **   CALCULATIONS   **
       **********************/

      /* bullet coordinates */
      ret = (PI / 180) * (current->deg - 90);

      pos_x = cos(ret);
      pos_x = pos_x * (radius - (BLOCKSIZE / 4));
      pos_x = pos_x + radius;
      pos_x = pos_x + current->x - (BLOCKSIZE / 2);

      pos_y = sin(ret);
      pos_y = pos_y * (radius - (BLOCKSIZE / 4));
      pos_y = pos_y + radius;
      pos_y = pos_y + current->y - (BLOCKSIZE / 2);

      /**********************
       **     DRAW BULLET  **
       **********************/

      circlefill(shipbuff, (int)current->x, (int)current->y, 1, makecol(255, 0, 0));
    }
    /* next ship */
    current = current->next;
  }
}
#endif
