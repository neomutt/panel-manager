#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

FILE *flog = NULL;

void (*old_winch) (int) = NULL;

void
catcher (int sig)
{
	old_winch (sig);

	fprintf (flog, "Screen %dx%d\n", COLS, LINES);
}

void
init_signal_handler()
{
	struct sigaction act;
	struct sigaction old;

	sigemptyset (&act.sa_mask);

	act.sa_flags = 0;
	act.sa_handler = catcher;

	sigaction (SIGWINCH, &act, &old);

	old_winch = old.sa_handler;
}

WINDOW *
create_newwin (int height, int width, int y, int x, int colour)
{
	WINDOW *local_win;

	local_win = newwin (height, width, y, x);
	wcolor_set (local_win, colour, NULL);
	box (local_win, 0 , 0);		/* 0, 0 gives default characters for the vertical and horizontal lines*/
	wrefresh (local_win);		/* Show that box */

	return local_win;
}

int
main ()
{
	flog = fopen ("/home/mutt/log.txt", "a");

	WINDOW *main_win = initscr (); /*  curses */
	start_color();

	init_pair(1, COLOR_RED,   COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE,  COLOR_BLACK);

	init_signal_handler();

	while (1) {
		WINDOW *win1, *win2, *win3;
		int x, y, w, h;

		int third = COLS / 3;

		x = 0;
		y = 0;
		w = third;
		h = LINES;

		win1 = create_newwin (h, w, y, x, 1);

		x += third;
		win2 = create_newwin (h, w, y, x, 2);

		x += third;
		win3 = create_newwin (h, w, y, x, 3);

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

