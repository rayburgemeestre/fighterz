/*
        Used palmer's code as basis for this:

        crpalmer@undergrad.math.uwaterloo.ca
        http://www.undergrad.math.uwaterloo.ca/~crpalmer/sp.html
*/

#include "common.h"

const int __INFINITY__ = 16384;

typedef struct pq_elist {
  int row, col;
  struct pq_elist *next;
} ELIST;

typedef struct pq_plist {
  int wt;
  ELIST *elements;
  struct pq_plist *next;
} PLIST;

typedef struct {
  int count;
  PLIST *priorities;
} PQUEUE;

int pqueue_insert(PQUEUE *pq, int row, int col, int wt);
int pqueue_peekmin(PQUEUE *pq, int *row, int *col);
int pqueue_popmin(PQUEUE *pq, int *row, int *col);
PQUEUE *pqueue_new(void);

struct timeb t_x;
struct timeb started_x;

static int use_heuristic = 1; /* don't use a heuristic */
static int abort_at_dest = 0; /* don't abort at destination */

void map_read() {
  int row, col;
  int i = 0;
  char line[1001];
  char *line_ptr;

  cols_read = 0;

  for (row = 0; row < Y_BLOCKS; row++) {
    strcpy(line, map2[i++]);

    for (col = 0, line_ptr = line; col < X_BLOCKS && *line_ptr; col++, line_ptr++)
      if (isdigit(*line_ptr))
        map[row][col] = *line_ptr - '0';
      else
        map[row][col] = __INFINITY__;
  }

  cols_read = X_BLOCKS;
  rows_read = Y_BLOCKS;

  for (row = 0; row < Y_BLOCKS; row++)
    for (col = 0; col < X_BLOCKS; col++) {
      dist[row][col] = __INFINITY__;
      parent[row][col][0] = -1;
      parent[row][col][1] = -1;
    }

  startr = startc = endr = endc = -1;
}

int map_draw_path(void) {
  int row, col;
  int wt = 0;

  for (row = 0; row < rows_read; row++)
    for (col = 0; col < cols_read; col++)
      if (parent[row][col][0] >= 0 && parent[row][col][1] >= 0) wt += map[row][col];

  // map_draw_path2();

  return wt;  // weight of path
}

void map_create_path(struct data *ptr) {
  int wt = 0, dir;
  int current_r, current_c;
  int old_r, old_c;
  int ret;
  int tcoord_x, tcoord_y; /* tussen coordinaten, waar die zeg maar maximaal in 1 lijn naartoe kan vliegen */
  double x1, x2, y1, y2;

  element = 0;

  old_r = current_r = startr;
  old_c = current_c = startc;

  tcoord_x = startc * BLOCKSIZE + (BLOCKSIZE / 2);
  tcoord_y = startr * BLOCKSIZE + (BLOCKSIZE / 2);

  ptr->bot_x = (double)((startc * BLOCKSIZE) + (BLOCKSIZE / 2));
  ptr->bot_y = (double)((startr * BLOCKSIZE) + (BLOCKSIZE / 2));

  dir = 10;
  while (dir != 0) {
    /* find a walkable spot */
    dir = 0;

    /* left of us */
    if (current_c > 0 && dir == 0)
      if (parent[current_r][current_c - 1][0] >= 0 && parent[current_r][current_c - 1][1] >= 0) dir = 1;

    /* on our right */
    if (current_c < cols_read && dir == 0)
      if (parent[current_r][current_c + 1][0] >= 0 && parent[current_r][current_c + 1][1] >= 0) dir = 2;

    /* above us */
    if (current_r > 0 && dir == 0)
      if (parent[current_r - 1][current_c][0] >= 0 && parent[current_r - 1][current_c][1] >= 0) dir = 3;

    /* underneath us */
    if (current_r < rows_read && dir == 0)
      if (parent[current_r + 1][current_c][0] >= 0 && parent[current_r + 1][current_c][1] >= 0) dir = 4;

    parent[current_r][current_c][0] = -1;
    parent[current_r][current_c][1] = -1;

    old_c = current_c;
    old_r = current_r;

    if (dir == 1)
      current_c--;
    else if (dir == 2)
      current_c++;
    else if (dir == 3)
      current_r--;
    else if (dir == 4)
      current_r++;

    // fix coordinates
    x1 = (double)(startc * BLOCKSIZE) + (BLOCKSIZE / 2);
    y1 = (double)(startr * BLOCKSIZE) + (BLOCKSIZE / 2);
    x2 = (double)((current_c)*BLOCKSIZE) + (BLOCKSIZE / 2);
    y2 = (double)((current_r)*BLOCKSIZE) + (BLOCKSIZE / 2);
    // while (x1 > field_width) x1 -= field_width;
    // while (y1 > field_height) y1 -= field_height;
    // while (x2 > field_width) x2 -= field_width;
    // while (y2 > field_height) y2 -= field_height;
    ret = valid_target(x1, y1, x2, y2, (double)(BLOCKSIZE / 2));

#if DEBUG2 == 1
    {
      double a, b, c, d;
      a = x1;
      //      while (a > field_width) a -= field_width;
      b = y1;
      //      while (b > field_height) b -= field_height;
      c = x2;
      //      while (c > field_width) c -= field_width;
      d = y2;
      //      while (d > field_height) d -= field_height;
      line(fieldbuff, (int)a, (int)b, (int)c, (int)d, makecol(255, 0, 0));
    }
#endif
    if (ret == 1) {
      static int old_x, old_y;

      // these two lines help if ret = 1
      // tcoord_x = current_c * BLOCKSIZE + (BLOCKSIZE / 2);
      // tcoord_y = current_r * BLOCKSIZE + (BLOCKSIZE / 2);
      /* not a valid target  */
      // LOG: current++ == 1222
      ptr->path[element][0] = (double)tcoord_x;
      ptr->path[element++][1] = (double)tcoord_y;

      //			current_c = startc = old_c;
      //			current_r = startr = old_r; /* was: current instead of old */
      //			fprintf(fp, "LOG: current_c = start_c = old_c (%d)\n", old_c);
      //			fprintf(fp, "LOG: current_r = start_r = old_r (%d)\n", old_r);
      old_c = startc = current_c;
      old_r = startr = current_r; /* was: current instead of old */
#if DEBUG2 == 1
      {
        double x = (double)tcoord_x;
        double y = (double)tcoord_y;

        while (x > field_width) x -= field_width;
        while (y > field_height) y -= field_height;

        circlefill(fieldbuff, (int)x, (int)y, 5, makecol(255, 0, 0));
        line(fieldbuff, old_x, old_y, (int)x, (int)y, makecol(255, 0, 0));

        addtext("%.2f, %.2f", x, y);
        old_x = x;
        old_y = y;
      }
#endif
    } else {
      tcoord_x = current_c * BLOCKSIZE + (BLOCKSIZE / 2);
      tcoord_y = current_r * BLOCKSIZE + (BLOCKSIZE / 2);

#if DEBUG2 == 1
      {
        double x = (double)tcoord_x;
        double y = (double)tcoord_y;

        while (x > field_width) x -= field_width;
        while (y > field_height) y -= field_height;
        circlefill(fieldbuff, (int)x, (int)y, 5, makecol(0, 0, 255));
        addtext("%.2f, %.2f", x, y);

        {
          double a, b, c, d;
          a = x1;
          while (a > field_width) a -= field_width;
          b = y1;
          while (b > field_height) b -= field_height;
          c = x2;
          while (c > field_width) c -= field_width;
          d = y2;
          while (d > field_height) d -= field_height;
          line(fieldbuff, (int)a, (int)b, (int)c, (int)d, makecol(0, 255, 0));
        }
      }
#endif
    }
  }

  /* also our end coordinates */
  ptr->path[element][0] = (double)(current_c * BLOCKSIZE + (BLOCKSIZE / 2));
  ptr->path[element++][1] = (double)(current_r * BLOCKSIZE + (BLOCKSIZE / 2));

  ptr->path[PATH_MAX_ - 1][0] = (double)element;
  addtext("done");
  return;
}

void map_draw_path2(void) {
  int row, col;
  int wt;

  wt = 0;
  for (row = 0; row < rows_read; row++) {
    for (col = 0; col <= cols_read; col++) {
      if (startr == row && startc == col)
        ;
      else if (endr == row && endc == col)
        ;
      else if (parent[row][col][0] >= 0 && parent[row][col][1] >= 0)
        wt += map[row][col];
    }
  }
}

#define drow(row, i) (row + delta[i][0])
#define dcol(col, i) (col + delta[i][1])

/* int delta[8][2] = { -1, -1, -1, 1, 1, -1, 1, 1, -1, 0, 0, 1, 1, 0, 0, -1 }; */

/* TODO: Hier zit iets fout, 8x2=16 dingen, en er staan hier 8, dus de andere 8
 * zijn _NIET_ geinitaliseerd -- Syzop:
 * Nice find :) -- trigen
 * EDIT: blijkbaar intentioneel geweest, alleen naar boven/links/rechts/beneden was de bedoeling
 * zat wel bug in idd
 */

int delta[4][2] = {
    // deltas should be:
    // 1 2 3
    // 4   5
    // 6 7 8
    {0, 1},   // 2
    {-1, 0},  // 4
    {1, 0},   // 5
    {0, -1},  // 7
};

#ifdef max
#undef max
#endif
#ifdef abs
#undef abs
#endif

#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(a) ((a) < 0 ? -(a) : (a))

static int h(int r1, int c1, int r2, int c2) {
  if (use_heuristic) return (max(abs(r2 - r1), abs(c2 - c1)));
  return (0);
}

void build_path(void) {
  PQUEUE *pq;
  int row, col;
  int i;

  pops = 0;
  pushes = 0;

  pq = pqueue_new();

  if (startr < 0 || startr >= rows_read || startc < 0 || startc >= cols_read) return;

  dist[startr][startc] = 0;
  parent[startr][startc][0] = -1;
  parent[startr][startc][1] = -1;
  pqueue_insert(pq, startr, startc, 0 + h(startr, startc, endr, endc));
  pushes++;

  while (pqueue_popmin(pq, &row, &col) == 0) {
    /* DEBUG INFORMATION */
    pops++;

    /* See if we only wanted the one path */
    if (abort_at_dest && row == endr && col == endc) {
      /* STOP HERE */
      break;
    }

    /* FOREACH square adjacent to (row,col), call it drow(row,i), dcol(col,i) */
    for (i = 0; i < 4; i++) {
      /* Check to make sure the square is on the map */
      if (drow(row, i) < 0 || drow(row, i) >= rows_read || dcol(col, i) < 0 || dcol(col, i) >= cols_read) continue;

      /* Check to see if we've found a shorter path */
      if (dist[row][col] + map[drow(row, i)][dcol(col, i)] < dist[drow(row, i)][dcol(col, i)]) {
        /* We have a shorter path, update the information */
        dist[drow(row, i)][dcol(col, i)] = map[drow(row, i)][dcol(col, i)] + dist[row][col];
        parent[drow(row, i)][dcol(col, i)][0] = row;
        parent[drow(row, i)][dcol(col, i)][1] = col;
        /* Push the modified square onto the priority queue */
        pqueue_insert(pq,
                      drow(row, i),
                      dcol(col, i),
                      dist[drow(row, i)][dcol(col, i)] + h(drow(row, i), dcol(col, i), endr, endc));
        pushes++;
      }
    }
  }
  return;
}

void limit_path(void) {
  int row, col;
  int tmp;

  if (endr < 0 || endr >= rows_read || endc < 0 || endc >= cols_read) return;
  row = endr;
  col = endc;
  do {
    dist[row][col] = -dist[row][col];
    tmp = parent[row][col][0];
    col = parent[row][col][1];
    row = tmp;
  } while (row >= 0 && col >= 0);

  for (row = 0; row < rows_read; row++)
    for (col = 0; col < cols_read; col++)
      if (dist[row][col] < 0)
        dist[row][col] = -dist[row][col];
      else
        parent[row][col][0] = -1;
  return;
}

void findpath(struct data *ptr, double x, double y) {
  fillmap();

  map_read();

  startr = (int)((ptr->y) / BLOCKSIZE);
  startc = (int)((ptr->x) / BLOCKSIZE);
  endr = (int)y / BLOCKSIZE;
  endc = (int)x / BLOCKSIZE;

  build_path();
  limit_path();

  map_create_path(ptr);
}

/* Priority queue stuff */

size_t max_memory = 0;
size_t current_memory = 0;

void *my_malloc(size_t size) {
  void *ptr;

  if ((ptr = malloc(size)) == NULL) return (NULL);
  current_memory += size;
  if (current_memory > max_memory) max_memory = current_memory;
  return (ptr);
}

void my_free(void *ptr, size_t size) {
  free(ptr);
  current_memory -= size;
}

PQUEUE *pqueue_new(void) {
  PQUEUE *pq;

  if ((pq = (PQUEUE *)my_malloc(sizeof(PQUEUE))) == NULL) return (NULL);
  pq->count = 0;
  pq->priorities = NULL;
  return (pq);
}

int pqueue_insert(PQUEUE *pq, int row, int col, int wt) {
  ELIST *e;
  PLIST *p, *pp;

  if (pq == NULL) return (-1);

  if ((e = (ELIST *)my_malloc(sizeof(ELIST))) == NULL) return (-1);
  e->row = row;
  e->col = col;
  e->next = NULL;

  /* Test empty queue or before the first element of the queue */
  if (pq->priorities == NULL || pq->priorities->wt > wt) {
    if ((p = (PLIST *)my_malloc(sizeof(PLIST))) == NULL) return (-1);
    p->wt = wt;
    p->elements = e;
    p->next = pq->priorities;
    pq->priorities = p;
    pq->count++;
    return (0);
  }

  pq->count++;

  for (p = pq->priorities; p->next && p->next->wt <= wt; p = p->next)
    ;

  if (p->wt == wt) {
    e->next = p->elements;
    p->elements = e;
    return (0);
  }
  if ((pp = (PLIST *)my_malloc(sizeof(PLIST))) == NULL) return (-1);
  pp->wt = wt;
  pp->elements = e;
  pp->next = p->next;
  p->next = pp;
  pq->count++;
  return (0);
}

int pqueue_peekmin(PQUEUE *pq, int *row, int *col) {
  if (pq == NULL || pq->priorities == NULL || pq->priorities->elements == NULL) return (-1);
  *row = pq->priorities->elements->row;
  *col = pq->priorities->elements->col;
  return (0);
}

int pqueue_popmin(PQUEUE *pq, int *row, int *col) {
  ELIST *tmpe;
  PLIST *tmpp;

  if (pqueue_peekmin(pq, row, col) < 0) return (-1);
  pq->count--;

  tmpe = pq->priorities->elements;
  pq->priorities->elements = pq->priorities->elements->next;
  my_free(tmpe, sizeof(ELIST));

  if (pq->priorities->elements == NULL) {
    tmpp = pq->priorities;
    pq->priorities = pq->priorities->next;
    my_free(tmpp, sizeof(PLIST));
  }

  return (0);
}
