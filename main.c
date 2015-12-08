#include <stdio.h>
#include <unistd.h>

#if 0
#include <curses.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include "bool.h"
#include "panel.h"
#include "rect.h"
#include "gfx.h"
#include "signal.h"

int
create (void)
{
	Panel *top      =       new_panel ("top",      NULL,    O_VERTICAL,   0,    1,  -1);

	/* Panel *helpline = */ new_panel ("helpline", top,     O_HORIZONTAL, 1,    1,   1);
	Panel *middle   =       new_panel ("middle",   top,     O_HORIZONTAL, 0,    1,  -1);
#if 0
	/* Panel *index    = */ new_panel ("dummy1",   top,     O_HORIZONTAL, 1,    5,   5);
	/* Panel *index    = */ new_panel ("dummy2",   top,     O_HORIZONTAL, 1,    5,   5);
	/* Panel *index    = */ new_panel ("dummy3",   top,     O_HORIZONTAL, 1,    5,   5);
#endif
	/* Panel *status   = */ new_panel ("status",   top,     O_HORIZONTAL, 1,    1,   1);

#if 1
	Panel *sidebar  =       new_panel ("sidebar",  middle,  O_VERTICAL,   1,   20,  20);
	Panel *right    =       new_panel ("right",    middle,  O_VERTICAL,   0,    1,  -1);

	/* Panel *index    = */ new_panel ("index",    right,   O_HORIZONTAL, 1,   10,  10);
	/* Panel *pager    = */ new_panel ("pager",    right,   O_HORIZONTAL, 1,    1,  -1);
	/* Panel *helppage = */ new_panel ("helppage", right,   O_HORIZONTAL, 0,    1,  -1);
#endif

	Rect space = { 0, 0, 140, 30 };
	set_size (top, &space);

	set_size (top, &space);

	dump_panels (top, 0);
	printf ("\n");

#if 0
	index->visible = 0;
	set_size (top, &space);
	dump_panels (top, 0);
	printf ("\n");
#endif

#if 0
	sidebar->visible = 0;
	set_size (top, &space);
	dump_panels (top, 0);
	printf ("\n");

	sidebar->visible = 1;
	set_size (top, &space);
	dump_panels (top, 0);
	printf ("\n");
#endif
#if 1
	delete_panel (sidebar);
	set_size (top, &space);
	dump_panels (top, 0);
	printf ("\n");
#endif
#if 1
	insert_panel (middle, sidebar, middle->count);
	set_size (top, &space);
	dump_panels (top, 0);
	printf ("\n");
#endif

	free_panel (top);
	return 0;
}

int
main ()
{
	gfx_init();

	signal_init_handlers();

	Rect r = gfx_get_window (NULL);

	int COLS  = r.w;
	int LINES = r.h;

	while (1) {
		WINDOW *win1, *win2, *win3;
		int x, y, w, h;

		int third = COLS / 3;

		x = 0;
		y = 0;
		w = third;
		h = LINES;

		win1 = gfx_create_newwin (h, w, y, x, 1);

		x += third;
		win2 = gfx_create_newwin (h, w, y, x, 2);

		x += third;
		win3 = gfx_create_newwin (h, w, y, x, 3);

		sleep (999);

		gfx_wipe_window (win1);
		gfx_wipe_window (win2);
		gfx_wipe_window (win3);

		gfx_close_window (win1);
		gfx_close_window (win2);
		gfx_close_window (win3);
	}

	gfx_shutdown();
	return 0;
}

