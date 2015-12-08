#include <stdio.h>

#include "bool.h"
#include "panel.h"
#include "rect.h"
#include "gfx.h"
#include "signal.h"
#include "log.h"

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
				// new_panel ("helppage", right,  O_HORIZONTAL, FALSE,  1,  -1);
	return top;
}

int
main (int argc, char *argv[])
{
	const char *log_file = "/home/mutt/log.txt";

	if (argc > 1) {
		log_file = argv[1];
	}

	if (!log_open (log_file)) {
		printf ("Can't open log file: '%s'\n", argv[1]);
		return 1;
	}

	gfx_init();

	signal_init_handlers();

	Panel *top = create_panels();

	Panel *hl = top->children[0];
	Panel *sb = top->children[1]->children[0];
	Panel *in = top->children[1]->children[1]->children[0];
	Panel *pg = top->children[1]->children[1]->children[1];
	Panel *st = top->children[2];

	BOOL finished = FALSE;
	int repaints = 0;
	Rect old = { -1, -1, -1, -1 };

	WINDOW *win1 = NULL;
	WINDOW *win2 = NULL;
	WINDOW *win3 = NULL;
	WINDOW *win4 = NULL;
	WINDOW *win5 = NULL;

	while (!finished) {
		Rect r = gfx_get_rect (NULL);

		if (rect_sizes_differ (&old, &r)) {
			old = r;

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

			repaints++;

			set_size (top, &r);

			win1 = gfx_create_window (&hl->computed, 1);
			win2 = gfx_create_window (&sb->computed, 2);
			win3 = gfx_create_window (&in->computed, 3);
			win4 = gfx_create_window (&pg->computed, 4);
			win5 = gfx_create_window (&st->computed, 5);

			gfx_print (win1, hl->name, hl->redraws);
			gfx_print (win2, sb->name, sb->redraws);
			gfx_print (win3, in->name, in->redraws);
			gfx_print (win4, pg->name, pg->redraws);
			gfx_print (win5, st->name, st->redraws);
		}

		// This will block until a key is pressed, or a signal is received.
		int ch = gfx_get_char (win5);
		if ((ch == 'q') || (ch < 0))
			break;
	}

	gfx_shutdown();
	return 0;
}

