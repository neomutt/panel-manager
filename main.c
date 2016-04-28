/* Copyright (c) 2015 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>
#include <locale.h>

#include "bool.h"
#include "panel.h"
#include "rect.h"
#include "gfx.h"
#include "signal.h"
#include "log.h"

#include "contact.h"
#include "mail.h"
#include "help.h"

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
static void
cb_notify (Panel *panel, Notification flags)
{
	if (!panel || (flags == 0))
		return;

	char *s = "SIZE ";
	char *p = "POSN ";
	char *v = "VISIBLE ";
	char *h = "HIDDEN ";
	char *d = "DELETE ";
	char *r = "REPAINT ";

	log_message ("%s has changed: %s%s%s%s%s%s\n",
		panel->name,
		(flags & N_SIZE_CHANGED) ? s : "",
		(flags & N_POSN_CHANGED) ? p : "",
		(flags & N_VISIBLE)      ? v : "",
		(flags & N_HIDDEN)       ? h : "",
		(flags & N_DELETED)      ? d : "",
		(flags & N_REPAINT)      ? r : "");

	// log_message ("\t%d,%d %dx%d\n", panel->computed.x, panel->computed.y, panel->computed.w, panel->computed.h);

	// if (!panel->parent) {	// Top-level window
	// 	gfx_clear_screen (panel->window);
	// }

#if 0
	if (!panel->parent) {	// Top-level window
		if ((panel->computed.x < 0) || (panel->computed.y < 0) || (panel->computed.w < 0) || (panel->computed.h < 0))
			return;

		if (!panel->window)
			return;

		int x, y;
		for (y = 1; y < (panel->computed.h - 1); y++) {
			gfx_move (panel->window, 1, y);
			for (x = 1; x < (panel->computed.w - 1); x++) {
				gfx_print (panel->window, ".");
			}
		}
		gfx_refresh (panel->window);
		sleep (2);
	}
#endif
}


void
set_top_level (Panel *global, Panel *show)
{
	if (!global || !show)
		return;

	int i;
	for (i = 0; i < global->count; i++) {
		Panel *child = global->children[i];
		if (child == show) {
			panel_set_visible (child, TRUE);
		} else {
			panel_set_visible (child, FALSE);
		}
	}
}


int
main (int argc, char *argv[])
{
	// sleep(3);
	setlocale (LC_ALL, "");
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

	Panel *global = panel_new ("global", NULL, O_HORIZONTAL, TRUE, 1, -1);
	if (!global)
		return 1;
	global->notify = cb_notify;

	if (!help_init (global))
		return 1;

	if (!mail_init (global))
		return 1;

	if (!contact_init (global))
		return 1;

	// panel_dump (global, 0);

	Rect old = R_DEAD;

	while (TRUE) {
		Rect r = gfx_get_rect (NULL);

		if (rect_sizes_differ (&old, &r)) {
			old = r;
			// log_message ("RECT: %d,%d %dx%d\n", r.x, r.y, r.w, r.h);
			gfx_wipe_window  (global->window);
			gfx_close_window (global->window);
			global->window = gfx_create_window (&r, 1);
			panel_reflow (global, &r, FALSE);
			panel_send_notification (global, TRUE);
			panel_dump (global, 0);
			panel_set_focus (global);
		}

		// Panel *m = panel_get_by_name (global, "mail");
		// panel_set_visible (m, TRUE);
		// Panel *i = panel_get_by_name (m, "index-window");
		// panel_set_focus (i);

		// Panel *i = panel_get_by_name (global, "contacts");
		// This will block until a key is pressed, or a signal is received.
		int ch = gfx_get_char();
		// log_message ("Key press: %c (%d)\n", ch, ch);
		if ((ch == 'q') || (ch < 0)) {
			break;
		} else if (ch == 'c') {
			Panel *c = panel_get_by_name (global, "contact");
			set_top_level (global, c);
			panel_reflow (global, &r, TRUE);
			gfx_clear_screen (global->window);
			panel_send_notification (global, TRUE);
			gfx_refresh (global->window);
			panel_dump (global, 0);
		} else if (ch == 'h') {
			Panel *h = panel_get_by_name (global, "help");
			set_top_level (global, h);
			panel_reflow (global, &r, TRUE);
			gfx_clear_screen (global->window);
			panel_send_notification (global, TRUE);
			gfx_refresh (global->window);
			panel_dump (global, 0);
		} else if (ch == 'm') {
			Panel *m = panel_get_by_name (global, "mail");
			set_top_level (global, m);
			panel_reflow (global, &r, TRUE);
			gfx_clear_screen (global->window);
			panel_send_notification (global, TRUE);
			gfx_refresh (global->window);
			panel_dump (global, 0);
		} else if (ch == 12) {
			panel_set_repaint (global);
			gfx_clear_screen (global->window);
			panel_send_notification (global, TRUE);
			gfx_refresh (global->window);
			panel_dump (global, 0);
		}
	}

	panel_free (global);
	gfx_shutdown();
	log_close();
	return 0;
}

