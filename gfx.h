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
void     gfx_print         (WINDOW *win, const char *msg, int redraw);
int      gfx_get_char      (WINDOW *win);

#endif /* _GFX_H_ */
