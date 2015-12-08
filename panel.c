#include <stdio.h>
#include <stdlib.h>

#include "panel.h"
#include "log.h"

void
cb_notify (Panel *b, Notification flags)
{
	if (!b || (flags == 0))
		return;

	b->redraws++;

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


void
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
delete_panel (Panel *b)
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
insert_panel (Panel *parent, Panel *b, int index)
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

	return TRUE;
}

void
dump_panels (Panel *b, int indent)
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
		dump_panels (b->children[i], indent + 1);
	}
}

void
set_size (Panel *b, Rect *r)
{
	if (!b || !r)
		return;

	Notification flags = 0;
	b->old_computed = b->computed;
	b->old_visible  = b->visible;

	b->computed.x = r->x;
	b->computed.y = r->y;

	// printf ("Rect: %d,%d %dx%d\n", r->x, r->y, r->w, r->h);
	if (!b->visible) {
		// printf ("\t%s HIDDEN min %d, max %d\n", b->name, b->min_size, b->max_size);
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
			// printf ("\t%s: filler = %d, total = %d, max = %d\n", b->name, filler, total, max);
		}

		Rect s = *r;
		// printf ("\t%s %d children\n", b->name, b->count);
		for (i = 0; i < b->count; i++) {
			if (i == filler) {
				// Temporarily lie about the remaining space
				if (b->orient == O_VERTICAL) {
					int old = s.h;
					int size = (total - max + b->min_size);
					s.h = size;
					// printf ("\tallowing %d rows\n", s.h);
					set_size (b->children[i], &s);
					s.h = (old - size);
					// printf ("\tremaining %d rows\n", s.h);
					s.y += size;
				} else {
					int old = s.w;
					int size = (total - max + b->min_size);
					s.w = size;
					// printf ("\tallowing %d columns\n", s.w);
					set_size (b->children[i], &s);
					s.w = (old - size);
					// printf ("\tremaining %d columns\n", s.w);
					s.x += size;
				}
			} else {
				set_size (b->children[i], &s);
			}
		}

		if (filler >= 0) {
			// printf ("ADJUST\n");
		}
	} else if (b->orient == O_VERTICAL) {
		// printf ("\t%s VERTICAL min %d, max %d\n", b->name, b->min_size, b->max_size);
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
		// printf ("\t%s HORIZONTAL min %d, max %d\n", b->name, b->min_size, b->max_size);
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

	// printf ("\t%s CHOSEN %d,%d %dx%d\n", b->name, b->computed.x, b->computed.y, b->computed.w, b->computed.h);
}

void
free_panel (Panel *b)
{
	if (!b)
		return;

	if ((b->count > 0) && b->children) {
		int i;
		for (i = 0; i < b->count; i++) {
			free_panel (b->children[i]);
		}
	}

	free (b->children);
	free (b);
}

void
add_child (Panel *parent, Panel *b)
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
new_panel (const char *name, Panel *parent, Orientation orient, int visible, int min, int max)
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

	b->redraws = 0;

	if (parent) {
		add_child (parent, b);
	}

	return b;
}

