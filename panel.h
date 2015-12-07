#ifndef _PANEL_H_
#define _PANEL_H_

#include "rect.h"

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
	int visible;
	Rect computed;
	int min_size;
	int max_size;
} Box;

#endif /* _PANEL_H_ */
