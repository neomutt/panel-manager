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

static void
wipe_window (WINDOW *win)
{
	if (!win)
		return;

	gfx_wipe_window  (win);
	gfx_close_window (win);
}

static void
draw_window (WINDOW **win, Panel *pan, int colour)
{
	if (!win || !pan)
		return;

	if (pan->visible) {
		log_message ("%s visible\n", pan->name);
		*win = gfx_create_window (&(pan->computed), colour);
		gfx_print (*win, pan->name, pan->redraws);
	} else {
		log_message ("%s hidden\n", pan->name);
		*win = NULL;
	}
}

static void
exchange_sidebar (Panel *pan)
{
	if (!pan)
		return;

	Panel *parent = pan->parent;
	if (!parent)
		return;

	int index = -1;
	int i;
	for (i = 0; i < parent->count; i++) {
		if (parent->children[i] == pan) {
			index = i;
			break;
		}
	}

	log_message ("found sidebar %d\n", index);

	if (index < 0)
		return;

	panel_delete (pan);
	if (index == 0) {
		panel_insert (parent, pan, parent->count);
	} else {
		panel_insert (parent, pan, 0);
	}
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

	Panel *hl = panel_get_by_name (top, "helpline");
	Panel *sb = panel_get_by_name (top, "sidebar");
	Panel *in = panel_get_by_name (top, "index");
	Panel *pg = panel_get_by_name (top, "pager");
	Panel *hp = panel_get_by_name (top, "helppage");
	Panel *st = panel_get_by_name (top, "status");

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

			wipe_window (win1);
			wipe_window (win2);
			wipe_window (win3);
			wipe_window (win4);
			wipe_window (win5);
			wipe_window (win6);

			draw_window (&win1, hl, 1);
			draw_window (&win2, sb, 2);
			draw_window (&win3, in, 3);
			draw_window (&win4, pg, 4);
			draw_window (&win5, st, 5);
			draw_window (&win6, hp, 6);
		}

		// This will block until a key is pressed, or a signal is received.
		int ch = gfx_get_char (win5);
		if ((ch == 'q') || (ch < 0)) {
			break;
		} else if (ch == 's') {
			sb->visible = !sb->visible;
			force_repaint = TRUE;
		} else if (ch == 'i') {
			in->visible = !in->visible;
			force_repaint = TRUE;
		} else if (ch == 'p') {
			pg->visible = !pg->visible;
			force_repaint = TRUE;
		} else if (ch == 'x') {
			exchange_sidebar (sb);
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

	panel_free (top);
	gfx_close_window (win1);
	gfx_close_window (win2);
	gfx_close_window (win3);
	gfx_close_window (win4);
	gfx_close_window (win5);
	gfx_close_window (win6);
	gfx_shutdown();
	log_close();
	return 0;
}

