#ifndef _PANEL_H_
#define _PANEL_H_

typedef struct _rect {
	int x;
	int y;
	int w;
	int h;
} RECT;

typedef struct _box {
	int type;
	RECT r;
} BOX;

#endif /* _PANEL_H_ */
