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
	int redraws;
} Panel;

BOOL delete_panel (Panel *b);
BOOL insert_panel (Panel *parent, Panel *b, int index);
void dump_panels (Panel *b, int indent);
void set_size (Panel *b, Rect *r);
void free_panel (Panel *b);
void add_child (Panel *parent, Panel *b);
Panel * new_panel (const char *name, Panel *parent, Orientation orient, int visible, int min, int max);

#endif /* _PANEL_H_ */
