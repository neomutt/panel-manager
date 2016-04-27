/* Copyright (c) 2015 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _GFX_H_
#define _GFX_H_

#include "rect.h"

typedef struct _w WINDOW;

void gfx_init     (void);
void gfx_shutdown (void);

Rect gfx_get_rect (WINDOW *win);

WINDOW * gfx_create_window (Rect *r, int colour);
void     gfx_wipe_window   (WINDOW *win);
void     gfx_close_window  (WINDOW *win);
void     gfx_print         (WINDOW *win, const char *msg);
void     gfx_print_at      (WINDOW *win, int x, int y, const char *msg);
int      gfx_get_char      (WINDOW *win);
void     gfx_move          (WINDOW *win, int x, int y);
void     gfx_refresh       (WINDOW *win);

#endif /* _GFX_H_ */
