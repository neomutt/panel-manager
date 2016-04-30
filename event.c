/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include "log.h"
#include "signal.h"

enum signal signal_waiting = signal_none;

void
cb_signal (enum signal number)
{
	if (signal_waiting == signal_none) {
		signal_waiting = number;
		log_message ("EVENT: signal %d\n", number);
	} else {
		log_message ("EVENT: LOGJAM: signal %d\n", number);
	}
}

int
event_get_signal (void)
{
	if (signal_waiting == signal_none)
		return signal_none;

	int s = signal_waiting;
	signal_waiting = signal_none;
	return s;
}

void
event_init (void)
{
	signal_init_handlers (cb_signal);
}

