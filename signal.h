/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _SIGNAL_H_
#define _SIGNAL_H_

typedef void (*cb_signal_t) (int number);

void signal_init_handlers (cb_signal_t notify);

#endif /* _SIGNAL_H_ */
