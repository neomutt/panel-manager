#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

FILE *flog = NULL;

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
			fprintf (flog, "Screen %dx%d\n", COLS, LINES);
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
create_newwin (int height, int width, int y, int x)
{
	WINDOW *local_win;

	local_win = newwin (height, width, y, x);
	box (local_win, 0 , 0);		/* 0, 0 gives default characters for the vertical and horizontal lines*/
	wrefresh (local_win);		/* Show that box */

	return local_win;
}

int
main ()
{
	flog = fopen ("/home/mutt/log.txt", "a");

	WINDOW *main_win = initscr (); /*  curses */

	// init_signal_handler();

	// int i;
	// for (i = 0; i < 30; i++) {
	while (1) {
		WINDOW *win1, *win2, *win3;
		int x, y, w, h;

		int third = COLS / 3;

		x = 0;
		y = 0;
		w = third;
		h = LINES;

		win1 = create_newwin (h, w, y, x);

		x += third;
		win2 = create_newwin (h, w, y, x);

		x += third;
		win3 = create_newwin (h, w, y, x);

		wrefresh (win1);
		wrefresh (win2);
		wrefresh (win3);
		// wrefresh (main_win);
		sleep (999);

		wborder (win1, ' ', ' ', ' ',' ',' ',' ',' ',' ');
		wrefresh (win1);

		wborder (win2, ' ', ' ', ' ',' ',' ',' ',' ',' ');
		wrefresh (win2);

		wborder (win3, ' ', ' ', ' ',' ',' ',' ',' ',' ');
		wrefresh (win3);

		delwin (win1);
		delwin (win2);
		delwin (win3);
	}

	endwin (); /* end curses */
	return 0;
}

