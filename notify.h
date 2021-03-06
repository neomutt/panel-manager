/* Copyright (c) 2015 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _NOTIFY_H_
#define _NOTIFY_H_

typedef enum {
	N_SIZE_CHANGED     = (1<<0),
	N_POSN_CHANGED     = (1<<1),
	N_VISIBLE_CHANGED  = (1<<2),
	N_TOO_LITTLE_SPACE = (1<<3),
	N_DELETED          = (1<<4),
} Notification;

typedef struct _panel Panel;

typedef void (*cb_notify_t) (Panel*, Notification);

#endif /* _NOTIFY_H_ */
