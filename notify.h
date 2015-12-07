#ifndef _NOTIFY_H_
#define _NOTIFY_H_

#include "bool.h"

typedef enum {
	N_SIZE_CHANGED     = (1<<0),
	N_POSN_CHANGED     = (1<<1),
	N_VISIBLE_CHANGED  = (1<<2),
	N_TOO_LITTLE_SPACE = (1<<3),
	N_DELETED          = (1<<4),
} Notification;

typedef struct _box Box;
typedef struct _rect Rect;

typedef void (*cb_notify_t) (Box*, Notification);

#endif /* _NOTIFY_H_ */
