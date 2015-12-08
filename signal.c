#include <signal.h>
#include <stddef.h>

void (*old_winch) (int) = NULL;

void
catcher (int sig)
{
	old_winch (sig);
}

void
signal_init_handlers (void)
{
	struct sigaction act;
	struct sigaction old;

	sigemptyset (&act.sa_mask);

	act.sa_flags = 0;
	act.sa_handler = catcher;

	sigaction (SIGWINCH, &act, &old);

	old_winch = old.sa_handler;
}

