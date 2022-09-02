#include "common.h"

#if DEBUG2 == 1
void drawconsole() {
  int i = 0;
  // int c_top = TOP + field_height + 10 - 200;
  int c_top = TOP + SCREEN_Y + 10 - 200;
  int cur_pos = c_top;

  // AL_INLINE(void, rectfill, (BITMAP *bmp, int x1, int y1, int x2, int y2, int color),
  while (i < MAX_C_LINES) {
    putpixel(tmpscreen, 0, cur_pos, 42);
    textprintf(tmpscreen, font, LEFT, cur_pos, makecol(255, 128, 0), "%s", console[i]);
    cur_pos += 10;
    i++;
  }
  cur_pos = c_top;
}

void addtext(char *pattern, ...) {
  char buf[512];
  int i = 0;

  va_list ap;
  va_start(ap, pattern);
  vsprintf(buf, pattern, ap);
  va_end(ap);

  if (strlen(buf) > MAX_C_LLENGTH) buf[MAX_C_LLENGTH - 1] = '\0';

  while (i < (MAX_C_LINES - 1)) {
    strcpy(console[i], console[i + 1]);
    i++;
  }

  strcpy(console[i], buf);
  //  printf("add_text: %s\n", buf);
}
#else

void addtext(char *pattern, ...) {
  return;
};

#endif
