/* aase.c
 *
 * This file is part of the Allegro GUI Un-uglification Project.
 * It emulates the look of the allegro-sprite-editor widget set.
 *
 * David A. Capello <dacap@users.sourceforge.net>
 * http://ase.sourceforge.net
 */

#include <allegro.h>

#include "aase.h"
#include "agupitrn.h"

int aase_fg_color;
int aase_mg_color;
int aase_bg_color;

static void ase_rectedge(BITMAP *bmp, int x1, int y1, int x2, int y2, int fg, int bg);
static void ase_rectdotted(BITMAP *bmp, int x1, int y1, int x2, int y2, int fg, int bg);
static void ase_rectgouraud(BITMAP *bmp, int x1, int y1, int x2, int y2, int top, int bottom);
static void ase_rectbox(BITMAP *bmp, int x1, int y1, int x2, int y2, int flags);

void aase_init(void) {
  if (bitmap_color_depth(screen) == 8) {
    aase_fg_color = bestfit_color(_current_palette, 0, 0, 0);
    aase_bg_color = bestfit_color(_current_palette, 63, 63, 63);
    aase_mg_color = bestfit_color(_current_palette, 32, 32, 32);
  } else {
    aase_fg_color = makecol(0, 0, 0);
    aase_bg_color = makecol(255, 255, 255);
    aase_mg_color = makecol(128, 128, 128);
  }
}

void aase_shutdown(void) {}

static void ase_rectedge(BITMAP *bmp, int x1, int y1, int x2, int y2, int fg, int bg) {
  hline(bmp, x1, y1, x2 - 1, fg);
  vline(bmp, x1, y1 + 1, y2 - 1, fg);
  hline(bmp, x1, y2, x2, bg);
  vline(bmp, x2, y1, y2 - 1, bg);
}

static void ase_rectdotted(BITMAP *bmp, int x1, int y1, int x2, int y2, int fg, int bg) {
  int x = ((x1 + y1) & 1) ? 1 : 0;
  int c;

  /* two loops to avoid bank switches */
  for (c = x1; c <= x2; c++) putpixel(bmp, c, y1, (((c + y1) & 1) == x) ? fg : bg);
  for (c = x1; c <= x2; c++) putpixel(bmp, c, y2, (((c + y2) & 1) == x) ? fg : bg);

  for (c = y1 + 1; c < y2; c++) {
    putpixel(bmp, x1, c, (((c + x1) & 1) == x) ? fg : bg);
    putpixel(bmp, x2, c, (((c + x2) & 1) == x) ? fg : bg);
  }
}

static void ase_rectgouraud(BITMAP *bmp, int x1, int y1, int x2, int y2, int top, int bottom) {
  int gray, y;

  for (y = y1; y <= y2; y++) {
    gray = top + (bottom - top) * (y - y1) / (y2 - y1);

    hline(bmp, x1, y, x2, makecol(gray, gray, gray));
  }
}

static void ase_rectbox(BITMAP *bmp, int x1, int y1, int x2, int y2, int flags) {
  int bg, fg, top, bottom, aux;

  if (flags & D_DISABLED) {
    bg = makecol(192, 192, 192);
    fg = makecol(128, 128, 128);
    top = 255;
    bottom = 128;
  } else {
    bg = makecol(255, 255, 255);
    fg = makecol(0, 0, 0);
    top = 255;
    bottom = (flags & D_GOTFOCUS) ? 224 : 194;
  }

  if (flags & D_SELECTED) {
    aux = top;
    top = bottom;
    bottom = aux;
  }

  rect(bmp, x1, y1, x2, y2, fg);
  ase_rectedge(bmp,
               x1 + 1,
               y1 + 1,
               x2 - 1,
               y2 - 1,
               (flags & D_SELECTED) ? makecol(128, 128, 128) : makecol(255, 255, 255),
               (flags & D_SELECTED) ? makecol(255, 255, 255) : makecol(128, 128, 128));

  ase_rectgouraud(bmp, x1 + 2, y1 + 2, x2 - 2, y2 - 2, top, bottom);

  if (flags & D_GOTFOCUS) {
    if (flags & D_SELECTED)
      ase_rectdotted(bmp, x1 + 3, y1 + 3, x2 - 2, y2 - 2, fg, bg);
    else
      ase_rectdotted(bmp, x1 + 2, y1 + 2, x2 - 3, y2 - 3, fg, bg);
  }
}

int d_aase_box_proc(int msg, DIALOG *d, int c) {
  (void)c;

  if (msg == MSG_DRAW) ase_rectbox(screen, d->x, d->y, d->x + d->w - 1, d->y + d->h - 1, d->flags);

  return D_O_K;
}

int d_aase_shadow_box_proc(int msg, DIALOG *d, int c) {
  return d_aase_box_proc(msg, d, c);
}

int d_aase_button_proc(int msg, DIALOG *d, int c) {
  if (msg == MSG_DRAW) {
    ase_rectbox(screen, d->x, d->y, d->x + d->w - 1, d->y + d->h - 1, d->flags);

    if (d->dp) {
      int rtm = text_mode(-1);
      gui_textout(screen,
                  (char *)d->dp,
                  ((d->flags & D_SELECTED) ? 1 : 0) + d->x + d->w / 2,
                  ((d->flags & D_SELECTED) ? 1 : 0) + d->y + d->h / 2 - text_height(font) / 2,
                  ((d->flags & D_DISABLED) ? makecol(128, 128, 128) : makecol(0, 0, 0)),
                  TRUE);
      text_mode(rtm);
    }

    return D_O_K;
  }

  return d_button_proc(msg, d, c);
}

int d_aase_push_proc(int msg, DIALOG *d, int c) {
  int ret = D_O_K;

  d->flags |= D_EXIT;

  ret |= d_aase_button_proc(msg, d, c);

  if (ret & D_CLOSE) {
    ret &= ~D_CLOSE;
    scare_mouse_area(d->x, d->y, d->x + d->w, d->y + d->h);
    object_message(d, MSG_DRAW, 0);
    unscare_mouse();

    if (d->dp3) ret |= ((int (*)(DIALOG *))d->dp3)(d);
  }

  return ret;
}

int d_aase_check_proc(int msg, DIALOG *d, int c) {
  if (msg == MSG_DRAW) {
    int y = d->y + d->h / 2, rtm;
    int size = 12;

    ase_rectbox(screen, d->x, y - size / 2, d->x + size - 1, y - size / 2 + size - 1, d->flags);

    rtm = text_mode(-1);
    gui_textout(screen,
                (char *)d->dp,
                d->x + size + 4,
                y - text_height(font) / 2,
                ((d->flags & D_DISABLED) ? makecol(128, 128, 128) : makecol(0, 0, 0)),
                FALSE);
    text_mode(rtm);

    return D_O_K;
  }

  return d_button_proc(msg, d, c);
}

int d_aase_radio_proc(int msg, DIALOG *d, int c) {
  if (msg == MSG_DRAW) {
    int y = d->y + d->h / 2, rtm;
    int size = 12;

    ase_rectbox(screen, d->x, y - size / 2, d->x + size - 1, y - size / 2 + size - 1, d->flags);

    rtm = text_mode(-1);
    gui_textout(screen,
                (char *)d->dp,
                d->x + size + 4,
                y - text_height(font) / 2,
                ((d->flags & D_DISABLED) ? makecol(128, 128, 128) : makecol(0, 0, 0)),
                FALSE);
    text_mode(rtm);

    return D_O_K;
  }

  return d_radio_proc(msg, d, c);
}

int d_aase_icon_proc(int msg, DIALOG *d, int c) {
  return d_icon_proc(msg, d, c);
}

int d_aase_edit_proc(int msg, DIALOG *d, int c) {
  int ret;

  if (msg == MSG_START) {
    /* this is mainly for `file_select', because the size that
       it put to the `gui_edit_proc' is very small */
    d->h = MAX(d->h, 3 + text_height(font) + 3);
  } else if (msg == MSG_DRAW) {
    int f, l, p, w, x, fg, b, scroll;
    char buf[16];
    char *s;
    int rtm;

    if (!(d->flags & D_INTERNAL)) d->w -= 6;

    s = (char *)d->dp;
    l = ustrlen(s);
    if (d->d2 > l) d->d2 = l;

    /* calculate maximal number of displayable characters */
    if (d->d2 == l) {
      usetc(buf + usetc(buf, ' '), 0);
      x = text_length(font, buf);
    } else
      x = 0;

    b = 0;

    for (p = d->d2; p >= 0; p--) {
      usetc(buf + usetc(buf, ugetat(s, p)), 0);
      x += text_length(font, buf);
      b++;
      if (x > d->w) break;
    }

    if (x <= d->w) {
      b = l;
      scroll = FALSE;
    } else {
      b--;
      scroll = TRUE;
    }

    d->w += 6;

    ase_rectedge(screen, d->x, d->y, d->x + d->w - 1, d->y + d->h - 1, makecol(0, 0, 0), makecol(255, 255, 255));

    ase_rectedge(
        screen, d->x + 1, d->y + 1, d->x + d->w - 2, d->y + d->h - 2, makecol(128, 128, 128), makecol(192, 192, 192));

    ase_rectgouraud(
        screen, d->x + 2, d->y + 2, d->x + d->w - 3, d->y + d->h - 3, (d->flags & D_DISABLED) ? 128 : 224, 255);

    fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;
    x = 3;

    if (scroll) {
      p = d->d2 - b + 1;
      b = d->d2;
    } else
      p = 0;

    for (; p <= b; p++) {
      f = ugetat(s, p);
      usetc(buf + usetc(buf, (f) ? f : ' '), 0);
      w = text_length(font, buf);
      if (x + w > d->w - 2) break;
      f = ((p == d->d2) && (d->flags & D_GOTFOCUS));
      rtm = text_mode((f) ? fg : -1);
      textout(screen, font, buf, d->x + x, d->y + d->h / 2 - text_height(font) / 2, (f) ? d->bg : fg);
      text_mode(rtm);
      x += w;
    }

    if (d->flags & D_INTERNAL) d->w -= 6;

    return D_O_K;
  }

  d->w -= 6;
  d->flags |= D_INTERNAL;

  ret = d_edit_proc(msg, d, c);

  d->flags &= ~D_INTERNAL;
  d->w += 6;

  return ret;
}

int d_aase_list_proc(int msg, DIALOG *d, int c) {
  return d_list_proc(msg, d, c);
}

int d_aase_text_list_proc(int msg, DIALOG *d, int c) {
  return d_text_list_proc(msg, d, c);
}

int d_aase_textbox_proc(int msg, DIALOG *d, int c) {
  return d_textbox_proc(msg, d, c);
}

int d_aase_slider_proc(int msg, DIALOG *d, int c) {
  return d_slider_proc(msg, d, c);
}

int d_aase_menu_proc(int msg, DIALOG *d, int c) {
  return d_menu_proc(msg, d, c);
}

int d_aase_window_proc(int msg, DIALOG *d, int c) {
  (void)c;

  if (msg == MSG_DRAW) {
    ase_rectbox(screen, d->x, d->y, d->x + d->w - 1, d->y + d->h - 1, d->flags);

    if (d->dp) {
      int rtm = text_mode(-1);
      int cl = screen->cl, ct = screen->ct, cr = screen->cr, cb = screen->cb;
      set_clip(screen, d->x, d->y, d->x + d->w - 1, d->y + d->h - 1);

      textout_centre(screen, font, (char *)d->dp, d->x + d->w / 2, d->y + 4, makecol(0, 0, 0));

      set_clip(screen, cl, ct, cr, cb);
      text_mode(rtm);
    }
  }

  return D_O_K;
}

static struct agup_theme the_theme = {&aase_fg_color,
                                      &aase_bg_color,
                                      aase_init,
                                      aase_shutdown,
                                      d_aase_box_proc,
                                      d_aase_shadow_box_proc,
                                      d_aase_button_proc,
                                      d_aase_push_proc,
                                      d_aase_check_proc,
                                      d_aase_radio_proc,
                                      d_aase_icon_proc,
                                      d_aase_edit_proc,
                                      d_aase_list_proc,
                                      d_aase_text_list_proc,
                                      d_aase_textbox_proc,
                                      d_aase_slider_proc,
                                      d_aase_menu_proc,
                                      d_aase_window_proc};

AL_CONST struct agup_theme *aase_theme = &the_theme;
