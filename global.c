/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>

#include "panel.h"
#include "log.h"

static void
cb_notify (Panel *panel, Notification flags)
{
	if (!panel || (flags == 0))
		return;

	log_message ("%s has changed: %s\n", panel->name, notify_flags (flags));

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

Panel *
global_init (void)
{
	Panel *global = panel_new ("global", NULL, O_HORIZONTAL, TRUE, 1, -1);
	if (!global)
		return NULL;

	global->notify = cb_notify;

	return global;
}

