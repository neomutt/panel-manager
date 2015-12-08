#include <ncurses.h>

#include "bool.h"
#include "rect.h"

Rect
gfx_get_rect (WINDOW *win)
{
	if (win) {
	}

	Rect r = { 0, 0, COLS, LINES };
	return r;
}

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


WINDOW *
gfx_create_newwin (int height, int width, int y, int x, int colour)
{
	WINDOW *w;

	w = newwin (height, width, y, x);
	wcolor_set (w, colour, NULL);
	box (w, 0 , 0);		/* 0, 0 gives default characters for the vertical and horizontal lines*/
	wrefresh (w);		/* Show that panel */

	return w;
}

void
gfx_init (void)
{
	// WINDOW *main_win = 
	initscr ();
	init_colours ();
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

