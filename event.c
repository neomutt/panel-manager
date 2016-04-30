/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>

#include "log.h"
#include "signal.h"
#include "event.h"

enum signal signal_waiting = signal_none;

void
cb_signal (enum signal number)
{
	if (signal_waiting == signal_none) {
		signal_waiting = number;
		log_message ("EVENT: signal %d\n", number);
	} else {
		if ((signal_waiting == signal_winch) && (number == signal_winch)) {
			// Quietly ignore
		} else {
			log_message ("EVENT: LOGJAM: signal %d,%d\n", signal_waiting, number);
		}
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

Event *
event_listen (Panel *p)
{
	if (!p)
		return NULL;

	return NULL;
}

