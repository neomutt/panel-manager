/* Copyright (c) 2015 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _RECT_H_
#define _RECT_H_

#include "bool.h"

typedef struct _rect {
	int x;
	int y;
	int w;
	int h;
} Rect;

extern Rect R_DEAD;

BOOL rect_positions_differ (const Rect *a, const Rect *b);
BOOL rect_sizes_differ     (const Rect *a, const Rect *b);

#endif /* _RECT_H_ */
