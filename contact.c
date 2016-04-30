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
show_text (Panel *p, char *text[])
{
	if (!p || !p->window || !text)
		return;

	int i;
	for (i = 0; text[i]; i++) {
		gfx_print_at (p->window, 1, i + 1, text[i]);
	}
}

static void
notify (Panel *p, Notification n)
{
	if (!p)
		return;

	log_message ("notify contact/%s %s\n", p->name, notify_flags (n));

	// Don't paint the wrapper window
	if (strcmp (p->name, "contact") == 0)
		return;

	// if (p->max_size < 0)
	// 	return;

	// if (n & (N_SIZE_CHANGED | N_POSN_CHANGED | N_REPAINT)) {

		wipe_window (p);
		draw_window (p, 3);
		if (strcmp (p->name, "groups") == 0) {
			char *folders[] = {
				"musicians",
				"comedians",
				"weathermen",
				NULL
			};

			show_text (p, folders);
		} else if (strcmp (p->name, "contacts") == 0) {
			char *mail[] = {
				"Jimmy Page",
				"David Bowie",
				"Ray Charles",
				NULL
			};

			show_text (p, mail);
		}

		// Rect *r = &p->computed;
		// log_message ("window %s: %d+%d %dx%d\n", p->name, r->x, r->y, r->w, r->h);
	// }
}

static Panel*
create_panels (Panel *parent)
{
	if (!parent)
		return NULL;

	Panel *contact   =      panel_new ("contact",  parent,  O_HORIZONTAL, FALSE,   1, -1);

				panel_new ("groups",   contact, O_VERTICAL,   TRUE,   30, 30);
				panel_new ("contacts", contact, O_VERTICAL,   TRUE,   10, -1);

	contact->notify = notify;
	return contact;
}


BOOL
contact_init (Panel *parent)
{
	if (!parent)
		return FALSE;

	Panel *contact = create_panels (parent);
	if (!contact)
		return FALSE;

	return TRUE;
}

void
contact_shutdown (void)
{
}

