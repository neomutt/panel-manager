/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _SIGNAL_H_
#define _SIGNAL_H_

enum signal {
	signal_none,
	signal_hup,
	signal_int,
	signal_usr1,
	signal_usr2,
	signal_winch
};

typedef void (*cb_signal_t) (enum signal number);

void signal_init_handlers (cb_signal_t notify);

#endif /* _SIGNAL_H_ */

