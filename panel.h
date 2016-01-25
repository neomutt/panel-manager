/* Copyright (c) 2015 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _PANEL_H_
#define _PANEL_H_

#include "rect.h"
#include "bool.h"
#include "notify.h"

typedef enum {
	O_VERTICAL,
	O_HORIZONTAL
} Orientation;

typedef struct _panel {
	const char *name;
	struct _panel *parent;
	struct _panel **children;
	int count;
	Orientation orient;
	BOOL visible;
	Rect computed;
	int min_size;
	int max_size;
	BOOL old_visible;
	Rect old_computed;
	cb_notify_t notify;
} Panel;

Panel * panel_new         (const char *name, Panel *parent, Orientation orient, int visible, int min, int max);
void    panel_free        (Panel *b);
Panel * panel_get_by_name (Panel *top, const char *name);

void    panel_add_child (Panel *parent, Panel *b);
BOOL    panel_insert    (Panel *parent, Panel *b, int index);
BOOL    panel_delete    (Panel *b);
void    panel_set_size  (Panel *b, Rect *r);

void    panel_dump      (Panel *b, int indent);

#endif /* _PANEL_H_ */
