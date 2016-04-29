/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>
#include <string.h>

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

static void
notify (Panel *p, Notification n)
{
	if (!p)
		return;

	log_message ("notify mail/%s %s\n", p->name, notify_flags (n));

	// Don't paint the wrapper windows
	if ((strcmp (p->name, "mail")        == 0) ||
	    (strcmp (p->name, "mail-middle") == 0) ||
	    (strcmp (p->name, "mail-right")  == 0))
		return;

	// if (p->max_size < 0)
	// 	return;

	// if (n & (N_SIZE_CHANGED | N_POSN_CHANGED | N_REPAINT)) {
		wipe_window (p);
		draw_window (p, 6);
		// Rect *r = &p->computed;
		// log_message ("window %s: %d+%d %dx%d\n", p->name, r->x, r->y, r->w, r->h);
	// }
}

static Panel*
create_panels (Panel *parent)
{
	if (!parent)
		return NULL;

	Panel *mail     =       panel_new ("mail",          parent, O_VERTICAL,     FALSE,    1,  -1);

				panel_new ("help-line",     mail,   O_HORIZONTAL,     TRUE,    1,   1);
	Panel *middle   =       panel_new ("mail-middle",   mail,   O_HORIZONTAL,     TRUE,    1,  -1);
				panel_new ("command",       mail,   O_HORIZONTAL,     TRUE,    1,   1);

				panel_new ("sidebar",       middle, O_VERTICAL,   TRUE,   20,  20);
	Panel *right    =       panel_new ("mail-right",    middle, O_VERTICAL,   TRUE,    1,  -1);

				panel_new ("index-window",  right,  O_HORIZONTAL,     TRUE,   10,  10);
				panel_new ("index-status",  right,  O_HORIZONTAL,     TRUE,    1,   1);
				panel_new ("pager-window",  right,  O_HORIZONTAL,     TRUE,    1,  -1);
				panel_new ("pager-status",  right,  O_HORIZONTAL,     TRUE,    1,   1);
				// panel_new ("attach-index",  right,  O_HORIZONTAL,     FALSE,   1,  10);
				// panel_new ("attach-window", right,  O_HORIZONTAL,     FALSE,   1,  -1);

	mail->notify = notify;
	return mail;
}


BOOL
mail_init (Panel *parent)
{
	if (!parent)
		return FALSE;

	Panel *mail = create_panels (parent);
	if (!mail)
		return FALSE;

	return TRUE;
}

void
mail_shutdown (void)
{
}

