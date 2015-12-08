#include <signal.h>
#include <stddef.h>

#include "log.h"

void (*old_winch) (int) = NULL;
void (*old_int)   (int) = NULL;

static void
winch_catcher (int sig)
{
	log_message ("Caught SIGWINCH (%d)\n", sig);
	old_winch (sig);
}

static void
int_catcher (int sig)
{
	log_message ("Caught SIGINT (%d)\n", sig);
	// old_int (sig);
}


void
signal_init_handlers (void)
{
	struct sigaction act;
	struct sigaction old;

	sigemptyset (&act.sa_mask);

	act.sa_flags = 0;

	act.sa_handler = winch_catcher;
	sigaction (SIGWINCH, &act, &old);
	old_winch = old.sa_handler;

	act.sa_handler = int_catcher;
	sigaction (SIGINT, &act, &old);
	old_int = old.sa_handler;
}

