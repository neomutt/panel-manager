/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include "log.h"
#include "signal.h"

int signal_waiting = 0;

void
cb_signal (int number)
{
	if (signal_waiting == 0) {
		signal_waiting = number;
		log_message ("EVENT: signal %d\n", number);
	} else {
		log_message ("EVENT: LOGJAM: signal %d\n", number);
	}
}

int
event_get_signal (void)
{
	if (signal_waiting == 0)
		return 0;

	int s = signal_waiting;
	signal_waiting = 0;
	return s;
}

void
event_init (void)
{
	signal_init_handlers (cb_signal);
}

