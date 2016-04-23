/* Copyright (c) 2015 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>

#include "bool.h"
#include "panel.h"
#include "rect.h"
#include "gfx.h"
#include "signal.h"
#include "log.h"

#include "contact.h"
#include "mail.h"

#if 0
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
		gfx_print (*win, pan->name);
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

	if (index < 0)
		return;

	panel_delete (pan);
	if (index == 0) {
		panel_insert (parent, pan, parent->count);
	} else {
		panel_insert (parent, pan, 0);
	}
}

#endif

int
main (int argc, char *argv[])
{
	// sleep (3);
	const char *log_file = "log.txt";

	if (argc > 1) {
		log_file = argv[1];
	}

	if (!log_open (log_file)) {
		printf ("Can't open log file: '%s'\n", argv[1]);
		return 1;
	}

	gfx_init();

	signal_init_handlers();

	Panel *global = panel_new ("global", NULL, O_HORIZONTAL, TRUE, -1, -1);
	if (!global)
		return 1;

	Panel *help = panel_new ("help", global, O_VERTICAL, FALSE, -1, -1);
	if (!help)
		return 1;

	if (!mail_init (global))
		return 1;

	if (!contact_init (global))
		return 1;

	Rect old = { -1, -1, -1, -1 };

	while (TRUE) {
		Rect r = gfx_get_rect (NULL);

		if (rect_sizes_differ (&old, &r)) {
			old = r;
			panel_reflow (global, &r, TRUE);
			panel_dump (global, 0);
		}

		break;

		// This will block until a key is pressed, or a signal is received.
		Panel *m = panel_get_by_name (global, "mail");
		int ch = gfx_get_char (m->window);
		// log_message ("Key press: %c (%d)\n", ch, ch);
		if ((ch == 'q') || (ch < 0)) {
			break;
		} else if (ch == 12) {
			if (global->notify) {
				global->notify (global, N_REPAINT);
			}
		}
	}

	panel_free (global);
	gfx_shutdown();
	log_close();
	return 0;
}

