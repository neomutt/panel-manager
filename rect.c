/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include "bool.h"
#include "rect.h"

Rect R_DEAD = { -1, -1, -1, -1 };

BOOL
rect_positions_differ (const Rect *a, const Rect *b)
{
	if (!a || !b)
		return FALSE;

	return ((a->x != b->x) || (a->y != b->y));
}

BOOL
rect_sizes_differ (const Rect *a, const Rect *b)
{
	if (!a || !b)
		return FALSE;

	return ((a->w != b->w) || (a->h != b->h));
}

