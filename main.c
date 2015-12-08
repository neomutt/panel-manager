#include <stdio.h>
#include <unistd.h>

#include "bool.h"
#include "panel.h"
#include "rect.h"
#include "gfx.h"
#include "signal.h"

static Panel *
create_panels (void)
{
	Panel *top      =       new_panel ("top",      NULL,    O_VERTICAL,   0,    1,  -1);

				new_panel ("helpline", top,     O_HORIZONTAL, 1,    1,   1);
	Panel *middle   =       new_panel ("middle",   top,     O_HORIZONTAL, 0,    1,  -1);
				new_panel ("status",   top,     O_HORIZONTAL, 1,    1,   1);

				new_panel ("sidebar",  middle,  O_VERTICAL,   1,   20,  20);
	Panel *right    =       new_panel ("right",    middle,  O_VERTICAL,   0,    1,  -1);

				new_panel ("index",    right,   O_HORIZONTAL, 1,   10,  10);
				new_panel ("pager",    right,   O_HORIZONTAL, 1,    1,  -1);
				new_panel ("helppage", right,   O_HORIZONTAL, 0,    1,  -1);

	return top;
}

int
main ()
{
	// Rect space = { 0, 0, 140, 30 };
	// set_size (top, &space);

	gfx_init();

	signal_init_handlers();

	Panel *top = create_panels();

	Panel *hl = top->children[0];
	Panel *sb = top->children[1]->children[0];
	Panel *in = top->children[1]->children[1];
	Panel *pg = top->children[1]->children[1]->children[0];
	Panel *st = top->children[2];

	while (1) {
		Rect r = gfx_get_rect (NULL);

		set_size (top, &r);

		WINDOW *win1, *win2, *win3, *win4, *win5;

		Rect rhl = hl->computed;
		Rect rsb = sb->computed;
		Rect rin = in->computed;
		Rect rpg = pg->computed;
		Rect rst = st->computed;

		win1 = gfx_create_newwin (rhl.h, rhl.w, rhl.y, rhl.x, 1);
		win2 = gfx_create_newwin (rsb.h, rsb.w, rsb.y, rsb.x, 2);
		win3 = gfx_create_newwin (rin.h, rin.w, rin.y, rin.x, 3);
		win4 = gfx_create_newwin (rpg.h, rpg.w, rpg.y, rpg.x, 4);
		win5 = gfx_create_newwin (rst.h, rst.w, rst.y, rst.x, 5);

		gfx_print (win1, "helpline");
		gfx_print (win2, "sidebar");
		gfx_print (win3, "index");
		gfx_print (win4, "pager");
		gfx_print (win5, "status");

		sleep (999);

		gfx_wipe_window (win1);
		gfx_wipe_window (win2);
		gfx_wipe_window (win3);
		gfx_wipe_window (win4);
		gfx_wipe_window (win5);

		gfx_close_window (win1);
		gfx_close_window (win2);
		gfx_close_window (win3);
		gfx_close_window (win4);
		gfx_close_window (win5);
	}

	gfx_shutdown();
	return 0;
}

