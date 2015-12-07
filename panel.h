#ifndef _PANEL_H_
#define _PANEL_H_

#include "rect.h"
#include "bool.h"
#include "notify.h"

typedef enum {
	O_VERTICAL,
	O_HORIZONTAL
} Orientation;

typedef struct _box {
	const char *name;
	struct _box *parent;
	struct _box **children;
	int count;
	Orientation orient;
	BOOL visible;
	Rect computed;
	int min_size;
	int max_size;
	BOOL old_visible;
	Rect old_computed;
	cb_notify_t notify;
} Box;

#endif /* _PANEL_H_ */
