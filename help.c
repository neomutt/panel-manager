/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>

#include "bool.h"
#include "panel.h"
#include "log.h"
#include "notify.h"

static void
wipe_window (Panel *p)
{
	if (!p || !p->window)
		return;

	gfx_wipe_window  (p->window);
	gfx_close_window (p->window);
	p->window = NULL;
}

static void
draw_window (Panel *p, int colour)
{
	if (!p)
		return;

	if (panel_is_visible (p)) {
		// log_message ("%s visible\n", p->name);
		p->window = gfx_create_window (&(p->computed), colour);
		gfx_print_at (p->window, 2, 0, p->name);
	}
}

void
show_help (Panel *p)
{
	if (!p)
		return;

	if (!p->window)
		return;

	char *help_text[] = {
		"Global",
		"\tc      - Contacts Panel",
		"\th or ? - Help Page",
		"\tm      - Mail Panel",
		"",
		"Mail",
		"\tGeneral",
		"\t\ti - change pager_index_lines",
		"\t\tt - toggle status_on_top",
		"\t\ts - toggle widescreen mode",
		"",
		"\tNavigation",
		"\t\t<enter> - Index -> Pager",
		"\t\tv       -          Pager -> Attach Index",
		"\t\t<enter> -                   Attach Index -> Attach Window",
		"",
		"\t\tUse q to leave Attach Window, Attach Index and Pager",
		"",
		"\tSidebar",
		"\t\ts - toggle sidebar_visible",
		"\t\tS - change sidebar_width",
		"\t\tx - toggle sidebar_on_right",
		NULL
	};

	int i;
	for (i = 0; help_text[i]; i++) {
		gfx_print_at (p->window, 2, i + 2, help_text[i]);
	}
}

static void
notify (Panel *p, Notification n)
{
	if (!p)
		return;

	log_message ("notify help/%s %s\n", p->name, notify_flags (n));

	// if (p->max_size < 0)
	// 	return;

	// if (n & (N_SIZE_CHANGED | N_POSN_CHANGED | N_REPAINT)) {
		wipe_window (p);
		draw_window (p, 2);
		show_help (p);
		// Rect *r = &p->computed;
		// log_message ("window %s: %d+%d %dx%d\n", p->name, r->x, r->y, r->w, r->h);
	// }
}

static Panel*
create_panels (Panel *parent)
{
	if (!parent)
		return NULL;

	Panel *help   =      panel_new ("help",  parent,  O_HORIZONTAL, FALSE,   1, -1);

	help->notify = notify;
	return help;
}


BOOL
help_init (Panel *parent)
{
	if (!parent)
		return FALSE;

	Panel *help = create_panels (parent);
	if (!help)
		return FALSE;

	return TRUE;
}

void
help_shutdown (void)
{
}

