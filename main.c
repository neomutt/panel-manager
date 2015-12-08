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
	Panel *top      =       new_panel ("top",      NULL,   O_VERTICAL,   FALSE,  1,  -1);

				new_panel ("helpline", top,    O_HORIZONTAL, TRUE,   1,   1);
	Panel *middle   =       new_panel ("middle",   top,    O_HORIZONTAL, FALSE,  1,  -1);
				new_panel ("status",   top,    O_HORIZONTAL, TRUE,   1,   1);

				new_panel ("sidebar",  middle, O_VERTICAL,   TRUE,  20,  20);
	Panel *right    =       new_panel ("right",    middle, O_VERTICAL,   FALSE,  1,  -1);

				new_panel ("index",    right,  O_HORIZONTAL, TRUE,  10,  10);
				new_panel ("pager",    right,  O_HORIZONTAL, TRUE,   1,  -1);
				new_panel ("helppage", right,  O_HORIZONTAL, FALSE,  1,  -1);

	return top;
}

int
main ()
{
	gfx_init();

	signal_init_handlers();

	Panel *top = create_panels();

	Panel *hl = top->children[0];
	Panel *sb = top->children[1]->children[0];
	Panel *in = top->children[1]->children[1]->children[0];
	Panel *pg = top->children[1]->children[1]->children[1];
	Panel *st = top->children[2];

	while (1) {
		Rect r = gfx_get_rect (NULL);

		set_size (top, &r);

		WINDOW *win1, *win2, *win3, *win4, *win5;

		win1 = gfx_create_newwin (&hl->computed, 1);
		win2 = gfx_create_newwin (&sb->computed, 2);
		win3 = gfx_create_newwin (&in->computed, 3);
		win4 = gfx_create_newwin (&pg->computed, 4);
		win5 = gfx_create_newwin (&st->computed, 5);

		gfx_print (win1, hl->name);
		gfx_print (win2, sb->name);
		gfx_print (win3, in->name);
		gfx_print (win4, pg->name);
		gfx_print (win5, st->name);

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
