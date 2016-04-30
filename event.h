/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _EVENT_H_
#define _EVENT_H_

typedef struct _panel Panel;

enum event_type {
	E_KEY_PRESS,
	E_MOUSE_CLICK,
	E_SIGNAL
};

typedef struct _kp {
	unsigned char key;
} KEY_PRESS;

enum button_modifier {
	BM_NONE  = 0,
	BM_SHIFT = (1 << 0),
	BM_CTRL  = (1 << 1),
	BM_ALT   = (1 << 2)
};

typedef struct _mc {
	int button;
	enum button_modifier modifier;
	int x;
	int y;
} MOUSE_CLICK;

typedef struct _s {
	int number;
} SIGNAL;

typedef struct _event {
	enum event_type type;
	Panel *source;

	union {
		KEY_PRESS   k;
		MOUSE_CLICK m;
		SIGNAL      s;
	};
} Event;

void event_init       (void);
int  event_get_signal (void);

#endif /* _EVENT_H_ */
