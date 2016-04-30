/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <signal.h>
#include <stddef.h>

#include "log.h"
#include "signal.h"

void (*old_hup)   (int) = NULL;
void (*old_int)   (int) = NULL;
void (*old_usr1)  (int) = NULL;
void (*old_usr2)  (int) = NULL;
void (*old_winch) (int) = NULL;

cb_signal_t notify_fn = NULL;

static void
hup_catcher (int sig)
{
	log_message ("Caught SIGHUP (%d)\n", sig);
	if (notify_fn) {
		notify_fn (signal_hup);
	}
	// old_hup (sig);
}

static void
int_catcher (int sig)
{
	log_message ("Caught SIGINT (%d)\n", sig);
	if (notify_fn) {
		notify_fn (signal_int);
	}
	// old_int (sig);
}

static void
usr1_catcher (int sig)
{
	log_message ("Caught SIGUSR1 (%d)\n", sig);
	if (notify_fn) {
		notify_fn (signal_usr1);
	}
	// old_usr1 (sig);
}

static void
usr2_catcher (int sig)
{
	log_message ("Caught SIGUSR2 (%d)\n", sig);
	if (notify_fn) {
		notify_fn (signal_usr2);
	}
	// old_usr2 (sig);
}

static void
winch_catcher (int sig)
{
	log_message ("Caught SIGWINCH (%d)\n", sig);
	if (notify_fn) {
		notify_fn (signal_winch);
	}
	old_winch (sig);
}


void
signal_init_handlers (cb_signal_t notify)
{
	struct sigaction act;
	struct sigaction old;

	sigemptyset (&act.sa_mask);

	act.sa_flags = 0;

	act.sa_handler = hup_catcher;
	sigaction (SIGHUP, &act, &old);
	old_hup = old.sa_handler;

	act.sa_handler = int_catcher;
	sigaction (SIGINT, &act, &old);
	old_int = old.sa_handler;

	act.sa_handler = usr1_catcher;
	sigaction (SIGUSR1, &act, &old);
	old_usr1 = old.sa_handler;

	act.sa_handler = usr2_catcher;
	sigaction (SIGUSR2, &act, &old);
	old_usr2 = old.sa_handler;

	act.sa_handler = winch_catcher;
	sigaction (SIGWINCH, &act, &old);
	old_winch = old.sa_handler;

	notify_fn = notify;
}

