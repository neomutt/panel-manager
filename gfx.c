#include <ncurses.h>

#include "bool.h"
#include "rect.h"

static void
init_colours (void)
{
	start_color();

	init_pair(1, COLOR_RED,     COLOR_BLACK);
	init_pair(2, COLOR_GREEN,   COLOR_BLACK);
	init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
	init_pair(4, COLOR_BLUE,    COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN,    COLOR_BLACK);
	init_pair(7, COLOR_WHITE,   COLOR_BLACK);
}


Rect
gfx_get_rect (WINDOW *win)
{
	if (win) {
	}

	// noecho();	// fake call to ensure curses is up-to-date
	Rect r = { 0, 0, COLS, LINES };
	return r;
}

WINDOW *
gfx_create_window (Rect *r, int colour)
{
	if (!r)
		return NULL;

	WINDOW *w;

	w = newwin (r->h, r->w, r->y, r->x);
	wcolor_set (w, colour, NULL);
	box (w, 0, 0);		/* 0, 0 gives default characters for the vertical and horizontal lines*/
	wrefresh (w);

	return w;
}

void
gfx_init (void)
{
	WINDOW *main_win = initscr ();
	init_colours ();
	cbreak();
	noecho();
	nodelay (main_win, FALSE);
}

void
gfx_shutdown (void)
{
	endwin (); /* end curses */
}

void
gfx_wipe_window (WINDOW *win)
{
	if (!win)
		return;

	wborder (win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh (win);
}

void
gfx_close_window (WINDOW *win)
{
	if (!win)
		return;

	delwin (win);
}

void
gfx_print (WINDOW *win, const char *msg, int redraw)
{
	wprintw (win, "%s: %d", msg, redraw);
	wrefresh (win);
}

int
gfx_get_char (WINDOW *win)
{
	if (win) {
		return wgetch (win);
	} else {
		return getch();
	}
}

