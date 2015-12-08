#ifndef _RECT_H_
#define _RECT_H_

#include "bool.h"

typedef struct _rect {
	int x;
	int y;
	int w;
	int h;
} Rect;

BOOL rect_positions_differ (const Rect *a, const Rect *b);
BOOL rect_sizes_differ     (const Rect *a, const Rect *b);

#endif /* _RECT_H_ */
