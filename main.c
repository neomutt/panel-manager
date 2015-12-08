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
	Panel *top      =       panel_new ("top",      NULL,   O_VERTICAL,   FALSE,  1,  -1);

				panel_new ("helpline", top,    O_HORIZONTAL, TRUE,   1,   1);
	Panel *middle   =       panel_new ("middle",   top,    O_HORIZONTAL, FALSE,  1,  -1);
				panel_new ("status",   top,    O_HORIZONTAL, TRUE,   1,   1);

				panel_new ("sidebar",  middle, O_VERTICAL,   TRUE,  20,  20);
	Panel *right    =       panel_new ("right",    middle, O_VERTICAL,   FALSE,  1,  -1);

				panel_new ("index",    right,  O_HORIZONTAL, TRUE,  10,  10);
				panel_new ("pager",    right,  O_HORIZONTAL, TRUE,   1,  -1);
				panel_new ("helppage", right,  O_HORIZONTAL, FALSE,  1,  -1);
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
	Panel *hp = top->children[1]->children[1]->children[2];
	Panel *st = top->children[2];

	BOOL finished = FALSE;
	BOOL force_repaint = FALSE;
	int repaints = 0;
	Rect old = { -1, -1, -1, -1 };

	WINDOW *win1 = NULL;
	WINDOW *win2 = NULL;
	WINDOW *win3 = NULL;
	WINDOW *win4 = NULL;
	WINDOW *win5 = NULL;
	WINDOW *win6 = NULL;

	while (!finished) {
		Rect r = gfx_get_rect (NULL);

		if (force_repaint || rect_sizes_differ (&old, &r)) {
			force_repaint = FALSE;
			repaints++;
			old = r;

			panel_set_size (top, &r);

			gfx_wipe_window (win1); gfx_close_window (win1); win1 = NULL;
			gfx_wipe_window (win2); gfx_close_window (win2); win2 = NULL;
			gfx_wipe_window (win3); gfx_close_window (win3); win3 = NULL;
			gfx_wipe_window (win4); gfx_close_window (win4); win4 = NULL;
			gfx_wipe_window (win5); gfx_close_window (win5); win5 = NULL;
			gfx_wipe_window (win6); gfx_close_window (win6); win6 = NULL;

			if (hl->visible) {
				win1 = gfx_create_window (&hl->computed, 1);
				gfx_print (win1, hl->name, hl->redraws);
			}
			if (sb->visible) {
				win2 = gfx_create_window (&sb->computed, 2);
				gfx_print (win2, sb->name, sb->redraws);
			}
			if (in->visible) {
				win3 = gfx_create_window (&in->computed, 3);
				gfx_print (win3, in->name, in->redraws);
			}
			if (pg->visible) {
				win4 = gfx_create_window (&pg->computed, 4);
				gfx_print (win4, pg->name, pg->redraws);
			}
			if (st->visible) {
				win5 = gfx_create_window (&st->computed, 5);
				gfx_print (win5, st->name, st->redraws);
			}
			if (hp->visible) {
				win6 = gfx_create_window (&hp->computed, 6);
				gfx_print (win6, hp->name, hp->redraws);
			}
		}

		// This will block until a key is pressed, or a signal is received.
		int ch = gfx_get_char (win5);
		if ((ch == 'q') || (ch < 0)) {
			break;
		} else if (ch == 'b') {
			sb->visible = !sb->visible;
			force_repaint = TRUE;
		} else if (ch == 'i') {
			in->visible = !in->visible;
			force_repaint = TRUE;
		} else if (ch == 'p') {
			pg->visible = !pg->visible;
			force_repaint = TRUE;
		} else if (ch == 'h') {
			if (hp->visible) {
				pg->visible = TRUE;
				in->visible = TRUE;
				hp->visible = FALSE;
			} else {
				pg->visible = FALSE;
				in->visible = FALSE;
				hp->visible = TRUE;
			}
			force_repaint = TRUE;
		}
	}

	gfx_shutdown();
	return 0;
}

