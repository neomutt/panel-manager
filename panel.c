#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void
catcher (int sig)
{
	LINES = -1;
	COLS  = -1;

	int fd = open ("/dev/tty", O_RDONLY);
	if (fd != -1) {
		struct winsize w;
		if (ioctl (fd, TIOCGWINSZ, &w) != -1) {
			LINES = w.ws_row;
			COLS  = w.ws_col;
		}
		close (fd);
	}
}

void
init_signal_handler()
{
	struct sigaction act;

	sigemptyset (&act.sa_mask);

	act.sa_flags = 0;
	act.sa_handler = catcher;

	sigaction (SIGWINCH, &act, NULL);
}

int
main ()
{
	initscr (); /* Start curses mode */

	init_signal_handler();

	int i;
	for (i = 0; i < 30; i++) {
		printf ("screen %dx%d\r\n", COLS, LINES);
		sleep (1);
	}

	endwin (); /* End curses mode */
	return 0;
}

