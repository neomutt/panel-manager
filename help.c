#include <stdio.h>

#include "bool.h"
#include "panel.h"
#include "log.h"

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

	log_message ("%s visible\n", p->name);
	if (panel_is_visible (p)) {
		p->window = gfx_create_window (&(p->computed), colour);
		gfx_print_at (p->window, 2, 0, p->name);
	}
}

static void
notify (Panel *p, Notification n)
{
	if (!p)
		return;

	// log_message ("mail notify %s %d\n", p->name, (int) n);

	// if (p->max_size < 0)
	// 	return;

	// if (n & (N_SIZE_CHANGED | N_POSN_CHANGED | N_REPAINT)) {
		wipe_window (p);
		draw_window (p, 2);
		Rect *r = &p->computed;
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

