/* Copyright (c) 2015 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "panel.h"
#include "log.h"

static void
cb_notify (Panel *b, Notification flags)
{
	if (!b || (flags == 0))
		return;

	char *s = "SIZE ";
	char *p = "POSN ";
	char *v = "VISIBLE ";
	char *d = "DELETE ";

	log_message ("%s has changed: flags: %s%s%s%s\n",
		b->name,
		(flags & N_SIZE_CHANGED)    ? s : "",
		(flags & N_POSN_CHANGED)    ? p : "",
		(flags & N_VISIBLE_CHANGED) ? v : "",
		(flags & N_DELETED)         ? d : "");
}

static void
notify_delete (Panel *b)
{
	if (!b)
		return;

	int i;
	for (i = 0; i < b->count; i++) {
		notify_delete (b->children[i]);
	}

	Rect dead = { -1, -1, -1, -1 };

	b->computed = dead;
	b->notify (b, N_DELETED);
	b->old_computed = dead;
}


BOOL
panel_delete (Panel *b)
{
	if (!b)
		return FALSE;

	Panel *parent = b->parent;
	if (!parent)
		return FALSE;

	int i;
	int match = -1;
	for (i = 0; i < parent->count; i++) {
		if (parent->children[i] == b) {
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

	notify_delete (b);
	b->parent = NULL;

	return TRUE;
}

BOOL
panel_insert (Panel *parent, Panel *b, int index)
{
	if (!parent || !b)
		return FALSE;

	if ((index < 0) || (index > parent->count))
		return FALSE;

	parent->count++;
	parent->children = realloc (parent->children, (parent->count * sizeof (Panel*)));

	int i;
	for (i = (parent->count-1); i > index; i--) {
		parent->children[i] = parent->children[i-1];
	}

	parent->children[index] = b;
	b->parent = parent;

	return TRUE;
}

void
panel_dump (Panel *b, int indent)
{
	if (!b)
		return;

	int colour = 32;	// Default: green

	if (!b->visible) {
		if (b->count == 0) {
			colour = 31;	// Hidden: red
		} else {
			colour = 30;	// Structure: grey
		}
	}

	printf ("\033[1;%dm%*s%c %-10s %d,%d %dx%d\033[m\n",
		colour,
		indent*4, "",
		(b->orient == O_VERTICAL) ? 'V' : 'H',
		b->name,
		b->computed.x,
		b->computed.y,
		b->computed.w,
		b->computed.h);

	int i;
	for (i = 0; i < b->count; i++) {
		panel_dump (b->children[i], indent + 1);
	}
}

void
panel_set_size (Panel *b, Rect *r)
{
	if (!b || !r)
		return;

	Notification flags = 0;
	b->old_computed = b->computed;
	b->old_visible  = b->visible;

	b->computed.x = r->x;
	b->computed.y = r->y;

	if (!b->visible) {
		b->computed.w = r->w;	// Claim all the space
		b->computed.h = r->h;

		int i;
		int filler = -1;
		int max = 0;
		int total = 0;

		if (b->orient == O_VERTICAL) {
			total = r->h;
		} else {
			total = r->w;
		}

		// Quick scan of sizes, looking for expanders
		for (i = 0; i < b->count; i++) {
			Panel *child = b->children[i];
			if (!child->visible && (child->count == 0))
				continue;

			if (child->max_size < 0) {
				max += child->min_size;
				if (filler >= 0) {
					// ERROR, but allow it *some* space
					printf ("ERROR: multiple expanders: %s, %s\n", b->children[filler]->name, child->name);
					b->max_size = b->min_size;
				}
				filler = i;
			} else {
				max += child->max_size;
			}
		}

		if (b->count > 0) {
		}

		Rect s = *r;
		for (i = 0; i < b->count; i++) {
			if (i == filler) {
				// Temporarily lie about the remaining space
				if (b->orient == O_VERTICAL) {
					int old = s.h;
					int size = (total - max + b->min_size);
					s.h = size;
					panel_set_size (b->children[i], &s);
					s.h = (old - size);
					s.y += size;
				} else {
					int old = s.w;
					int size = (total - max + b->min_size);
					s.w = size;
					panel_set_size (b->children[i], &s);
					s.w = (old - size);
					s.x += size;
				}
			} else {
				panel_set_size (b->children[i], &s);
			}
		}
	} else if (b->orient == O_VERTICAL) {
		if (b->min_size > r->w) {
			b->computed.x = -1;	// Too little width
			b->computed.y = -1;
			b->computed.w = -1;
			b->computed.h = -1;
			flags |= N_TOO_LITTLE_SPACE;
		} else if (b->max_size < 0) {
			b->computed.w = r->w;
			b->computed.h = r->h;

			r->x += r->w;
			r->w  = 0;
		} else if (b->max_size < r->w) {
			b->computed.w = b->max_size;
			b->computed.h = r->h;

			r->x += b->max_size;
			r->w -= b->max_size;
		} else {
			b->computed.w = r->w;
			b->computed.h = r->h;

			r->x += r->w;
			r->w  = 0;
		}
	} else {
		if (b->min_size > r->h) {
			b->computed.x = -1;	// Too little height
			b->computed.y = -1;
			b->computed.w = -1;
			b->computed.h = -1;
			flags |= N_TOO_LITTLE_SPACE;
		} else if (b->max_size < 0) {
			b->computed.w = r->w;
			b->computed.h = r->h;

			r->y += r->h;
			r->h  = 0;
		} else if (b->max_size < r->h) {
			b->computed.w = r->w;
			b->computed.h = b->max_size;

			r->y += b->max_size;
			r->h -= b->max_size;
		} else {
			b->computed.w = r->w;
			b->computed.h = r->h;

			r->y += r->h;
			r->h  = 0;
		}
	}

	if (b->visible != b->old_visible) {
		flags |= N_VISIBLE_CHANGED;
	}
	if (rect_positions_differ (&b->computed, &b->old_computed)) {
		flags |= N_POSN_CHANGED;
	}
	if (rect_sizes_differ (&b->computed, &b->old_computed)) {
		flags |= N_SIZE_CHANGED;
	}

	if (flags != 0) {
		b->notify (b, flags);
	}
}

void
panel_free (Panel *b)
{
	if (!b)
		return;

	if ((b->count > 0) && b->children) {
		int i;
		for (i = 0; i < b->count; i++) {
			panel_free (b->children[i]);
		}
	}

	free (b->children);
	free (b);
}

void
panel_add_child (Panel *parent, Panel *b)
{
	if (!parent || !b)
		return;

	parent->count++;
	Panel **children = realloc (parent->children, (parent->count * sizeof (Panel*)));
	if (!children)
		exit (1);

	parent->children = children;
	parent->children[parent->count-1] = b;
}

Panel *
panel_new (const char *name, Panel *parent, Orientation orient, int visible, int min, int max)
{
	Panel *b = malloc (sizeof (Panel));
	if (!b)
		return NULL;

	b->name     = name;
	b->parent   = parent;
	b->children = NULL;
	b->count    = 0;
	b->orient   = orient;
	b->visible  = visible;
	b->min_size = min;
	b->max_size = max;

	b->notify   = cb_notify;

	b->computed.x = -1;
	b->computed.y = -1;
	b->computed.w = -1;
	b->computed.h = -1;

	b->old_visible  = visible;

	b->old_computed.x = -1;
	b->old_computed.y = -1;
	b->old_computed.w = -1;
	b->old_computed.h = -1;

	if (parent) {
		panel_add_child (parent, b);
	}

	return b;
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

