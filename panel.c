/* Copyright (c) 2015 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "panel.h"
#include "log.h"
#include "notify.h"

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

	log_message ("%s has changed: flags: %s%s%s%s%s\n",
		panel->name,
		(flags & N_SIZE_CHANGED) ? s : "",
		(flags & N_POSN_CHANGED) ? p : "",
		(flags & N_VISIBLE)      ? v : "",
		(flags & N_HIDDEN)       ? h : "",
		(flags & N_DELETED)      ? d : "",
		(flags & N_REPAINT)      ? r : "");
}

static void
notify_delete (Panel *p)
{
	if (!p)
		return;

	int i;
	for (i = 0; i < p->count; i++) {
		notify_delete (p->children[i]);
	}

	Rect dead = { -1, -1, -1, -1 };

	p->computed = dead;
	p->notify (p, N_DELETED);
}


#if 0
void
notify_change (Panel *p)
{
	if (!p)
		return;

	Notification flags = 0;
	if (p->visible != p->old_visible) {
		flags |= N_VISIBLE_CHANGED;
	}
	if (rect_positions_differ (&p->computed, &p->old_computed)) {
		flags |= N_POSN_CHANGED;
	}
	if (rect_sizes_differ (&p->computed, &p->old_computed)) {
		flags |= N_SIZE_CHANGED;
	}

	if (flags != 0) {
		p->notify (p, flags);
	}
}

BOOL
send_notification (Panel *p, Notification n)
{
	if (!p || (n == 0))
		return FALSE;

	return TRUE;
}

#endif
BOOL
panel_is_visible (Panel *p)
{
	if (!p)
		return FALSE;

	if (!p->visible)
		return FALSE;

	if (!p->parent) {
		return p->visible;
	}

	return panel_is_visible (p->parent);
}

BOOL
panel_delete (Panel *p)
{
	if (!p)
		return FALSE;

	Panel *parent = p->parent;
	if (!parent)
		return FALSE;

	int i;
	int match = -1;
	for (i = 0; i < parent->count; i++) {
		if (parent->children[i] == p) {
			match = i;
			break;
		}
	}

	if (match < 0)
		return FALSE;

	parent->count--;
	for (i = match; i < parent->count; i++) {
		parent->children[i] = parent->children[i+1];
	}

	parent->children = realloc (parent->children, (parent->count * sizeof (Panel*)));

	notify_delete (p);
	p->parent = NULL;

	return TRUE;
}

BOOL
panel_insert (Panel *parent, Panel *p, int index)
{
	if (!parent || !p)
		return FALSE;

	if ((index < 0) || (index > parent->count))
		return FALSE;

	parent->count++;
	parent->children = realloc (parent->children, (parent->count * sizeof (Panel*)));

	int i;
	for (i = (parent->count-1); i > index; i--) {
		parent->children[i] = parent->children[i-1];
	}

	parent->children[index] = p;
	p->parent = parent;

	return TRUE;
}

void
panel_dump (Panel *p, int indent)
{
	if (!p)
		return;

	int colour = 32;	// Default: green

	if (p->max_size == -1) {
		if (panel_is_visible (p)) {
			colour = 36;	// Structure: cyan
		} else {
			colour = 34;	// Hidden: blue
		}
	} else if (!panel_is_visible (p)) {
		colour = 30;	// Hidden: grey
	}

	char type = '.';
	if (p->count > 0) {
		if (p->orient == O_VERTICAL) {
			type = 'V';
		} else {
			type = 'H';
		}
	}

	log_message ("\033[1;%dm%*s%c %s %d,%d %dx%d\033[m\n",
		colour,
		indent*4, "",
		type,
		p->name,
		p->computed.x,
		p->computed.y,
		p->computed.w,
		p->computed.h);

	int i;
	for (i = 0; i < p->count; i++) {
		panel_dump (p->children[i], indent + 1);
	}
}

void
panel_reflow (Panel *p, const Rect *avail, BOOL notify)
{
	if (!p)
		return;

	if (!p->visible) {
		// notify all children
		return;
	}

	// log_message ("%c %s: available %d+%d %dx%d\n", (p->orient == O_HORIZONTAL) ? 'H' : 'V', p->name, avail->x, avail->y, avail->w, avail->h);

	Rect old_size = p->computed;

	if (avail) {
		p->computed.x = avail->x;
		p->computed.y = avail->y;
	} else {
		avail = &p->computed;	/* No change to size/posn */
	}

	if (p->orient == O_HORIZONTAL) {
		if (p->min_size > avail->w) {
			p->computed.x = -1;             // Too little width
			p->computed.y = -1;
			p->computed.w = -1;
			p->computed.h = -1;
			p->nts |= N_TOO_LITTLE_SPACE;
		} else if ((p->max_size >= 0) && (p->max_size < avail->w)) {
			p->computed.w = avail->w;           // Get what we asked for
			p->computed.h = p->max_size;
		} else {
			p->computed.w = avail->w;           // Use all available space
			p->computed.h = avail->h;
		}
	} else {
		if (p->min_size > avail->h) {
			p->computed.x = -1;             // Too little height
			p->computed.y = -1;
			p->computed.w = -1;
			p->computed.h = -1;
			p->nts |= N_TOO_LITTLE_SPACE;
		} else if ((p->max_size >= 0) && (p->max_size < avail->h)) {
			p->computed.w = p->max_size;    // Get what we asked for
			p->computed.h = avail->h;
		} else {
			p->computed.w = avail->w;           // Use all available space
			p->computed.h = avail->h;
		}
	}
	// log_message ("%c %s = %d+%d %dx%d\n", (p->orient == O_HORIZONTAL) ? 'H' : 'V', p->name, p->computed.x, p->computed.y, p->computed.w, p->computed.h);

	if (rect_sizes_differ (&old_size, &p->computed)) {
		p->nts |= N_SIZE_CHANGED;
	}

	if (rect_positions_differ (&old_size, &p->computed)) {
		p->nts |= N_POSN_CHANGED;
	}

	if (notify && (p->nts != 0)) {
		// p->notify (p, p->nts);
	}

	int i;
	int filler = -1;
	int max = 0;

	// Quick scan of sizes, looking for expanders
	for (i = 0; i < p->count; i++) {
		Panel *child = p->children[i];
		if (!child->visible)
			continue;

		if (child->max_size < 0) {
			max += child->min_size;
			if (filler >= 0) {
				// ERROR, but allow it *some* space
				log_message ("ERROR: multiple expanders: %s, %s\n", p->children[filler]->name, child->name);
				p->max_size = p->min_size;
			}
			filler = i;
		} else {
			max += child->max_size;
		}
	}

	Rect remain = p->computed;
	Rect space;

	if (p->count > 0) {
		// log_message ("Children of %c %s get %d+%d %dx%d\n", (p->orient == O_HORIZONTAL) ? 'H' : 'V', p->name, remain.x, remain.y, remain.w, remain.h);
	}

	int total = 0;
	if (p->orient == O_VERTICAL) {
		total = avail->h;
	} else {
		total = avail->w;
	}

	for (i = 0; i < p->count; i++) {
		Panel *child = p->children[i];
		if (!child->visible)
			continue;

		int size;
		space.x = remain.x;
		space.y = remain.y;
		if (i == filler) {
			// Temporarily lie about the remaining space
			if (p->orient == O_VERTICAL) {
				size = (total - max + child->min_size);
				space.w = remain.w;
				space.h = size;
				panel_reflow (child, &space, TRUE);
				remain.h -= size;
				remain.y += size;
			} else {
				size = (total - max + child->min_size);
				space.w = size;
				space.h = remain.h;
				panel_reflow (child, &space, TRUE);
				remain.w -= size;
				remain.x += size;
			}
		} else {
			if (p->orient == O_VERTICAL) {
				size = child->max_size;
				space.w = remain.w;
				space.h = size;
				panel_reflow (child, &space, TRUE);
				remain.h -= size;
				remain.y += size;
			} else {
				size = child->max_size;
				space.w = size;
				space.h = remain.h;
				panel_reflow (child, &space, TRUE);
				remain.w -= size;
				remain.x += size;
			}
		}

		// log_message ("Child: %s -- %d+%d %dx%d\n", child->name, remain.x, remain.y, remain.w, remain.h);
	}

}

void
panel_free (Panel *p)
{
	if (!p)
		return;

	if ((p->count > 0) && p->children) {
		int i;
		for (i = 0; i < p->count; i++) {
			panel_free (p->children[i]);
		}
	}

	free (p->children);
	free (p);
}

void
panel_add_child (Panel *parent, Panel *p)
{
	if (!parent || !p)
		return;

	parent->count++;
	Panel **children = realloc (parent->children, (parent->count * sizeof (Panel*)));
	if (!children)
		exit (1);

	parent->children = children;
	parent->children[parent->count-1] = p;
}

Panel *
panel_new (const char *name, Panel *parent, Orientation orient, int visible, int min, int max)
{
	Panel *p = malloc (sizeof (Panel));
	if (!p)
		return NULL;

	p->name     = name;
	p->parent   = parent;
	p->children = NULL;
	p->count    = 0;
	p->orient   = orient;
	p->visible  = visible;
	p->min_size = min;
	p->max_size = max;

	p->notify   = cb_notify;
	p->window   = NULL;

	p->computed.x = -1;
	p->computed.y = -1;
	p->computed.w = -1;
	p->computed.h = -1;

	if (parent) {
		panel_add_child (parent, p);
	}

	return p;
}

Panel *
panel_get_by_name (Panel *top, const char *name)
{
	if (!top || !name)
		return NULL;

	if (strcmp (name, top->name) == 0)
		return top;

	Panel *match = NULL;
	int i;
	for (i = 0; i < top->count; i++) {
		match = panel_get_by_name (top->children[i], name);
		if (match) {
			break;
		}
	}

	return match;
}


