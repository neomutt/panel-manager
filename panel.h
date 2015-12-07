#ifndef _PANEL_H_
#define _PANEL_H_

typedef struct _rect {
	int x;
	int y;
	int w;
	int h;
} Rect;

typedef enum {
	O_VERTICAL,
	O_HORIZONTAL
} Orientation;

typedef enum {
	V_VISIBLE,
	V_HIDDEN
} Visibility;

typedef struct _box {
	const char *name;
	struct _box *parent;
	struct _box **children;
	int count;
	Orientation orient;
	Visibility visible;
	Rect computed;
	int min;
	int max;
} Box;

#endif /* _PANEL_H_ */
