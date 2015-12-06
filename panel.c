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

WINDOW *
create_newwin (int height, int width, int starty, int startx)
{
	WINDOW *local_win;

	local_win = newwin (height, width, starty, startx);
	box (local_win, 0 , 0);		/* 0, 0 gives default characters for the vertical and horizontal lines*/
	wrefresh (local_win);		/* Show that box */

	return local_win;
}

int
main ()
{
	WINDOW *main_win = initscr (); /* start curses */

	init_signal_handler();

	WINDOW *my_win;
	int startx, starty, width, height;

	height = 30;
	width = 60;
	starty = (LINES - height) / 2;
	startx = (COLS - width) / 2;

	my_win = create_newwin (height, width, starty, startx);
	// scrollok (my_win, 1);
	// refresh();

	int i;
	for (i = 0; i < 30; i++) {
		// werase (my_win);
		wprintw (my_win, "screen %dx%d\n", COLS, LINES);
		wrefresh (my_win);
		sleep (1);
	}

	endwin (); /* end curses */
	return 0;
}

