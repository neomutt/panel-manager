#include <stdio.h>
#include <stdlib.h>

#include "panel.h"

void
dump_boxes (Box *b, int indent)
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
		dump_boxes (b->children[i], indent + 1);
	}
}

void
set_size (Box *b, Rect *r)
{
	if (!b || !r)
		return;

	b->computed.x = r->x;
	b->computed.y = r->y;

	// printf ("Rect: %d,%d %dx%d\n", r->x, r->y, r->w, r->h);
	if (!b->visible) {
		// printf ("\t%s HIDDEN min %d, max %d\n", b->name, b->min, b->max);
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
			Box *child = b->children[i];
			if (!child->visible && (child->count == 0))
				continue;

			if (child->max < 0) {
				max += child->min;
				if (filler >= 0) {
					// ERROR, but allow it *some* space
					printf ("ERROR: multiple expanders: %s, %s\n", b->children[filler]->name, child->name);
					b->max = b->min;
				}
				filler = i;
			} else {
				max += child->max;
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
					int size = (total - max + b->min);
					s.h = size;
					// printf ("\tallowing %d rows\n", s.h);
					set_size (b->children[i], &s);
					s.h = (old - size);
					// printf ("\tremaining %d rows\n", s.h);
					s.y += size;
				} else {
					int old = s.w;
					int size = (total - max + b->min);
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
		// printf ("\t%s VERTICAL min %d, max %d\n", b->name, b->min, b->max);
		if (b->min > r->w) {
			b->computed.x = -1;	// Too little width
			b->computed.y = -1;
			b->computed.w = -1;
			b->computed.h = -1;
			return;
		}

		if (b->max < 0) {
			if (b->min > 0) {
				b->computed.w = b->min;
				r->x += b->min;
			} else {
				b->computed.w = 0;
			}
			b->computed.h = r->h;

			r->w  = 0;
		} else if (b->max < r->w) {
			b->computed.w = b->max;
			b->computed.h = r->h;

			r->x += b->max;
			r->w -= b->max;
		} else {
			b->computed.w = r->w;
			b->computed.h = r->h;

			r->x += r->w;
			r->w  = 0;
		}
	} else {
		// printf ("\t%s HORIZONTAL min %d, max %d\n", b->name, b->min, b->max);
		if (b->min > r->h) {
			b->computed.x = -1;	// Too little height
			b->computed.y = -1;
			b->computed.w = -1;
			b->computed.h = -1;
			return;
		}

		if (b->max < 0) {
			if (b->min > 0) {
				b->computed.h = b->min;
				r->y += b->min;
			} else {
				b->computed.h = 0;
			}
			b->computed.w = r->w;

			r->h  = 0;
		} else if (b->max < r->h) {
			b->computed.w = r->w;
			b->computed.h = b->max;

			r->y += b->max;
			r->h -= b->max;
		} else {
			b->computed.w = r->w;
			b->computed.h = r->h;

			r->y += r->h;
			r->h  = 0;
		}
	}

	// printf ("\t%s CHOSEN %d,%d %dx%d\n", b->name, b->computed.x, b->computed.y, b->computed.w, b->computed.h);
}

void
free_box (Box *b)
{
	if (!b)
		return;

	if ((b->count > 0) && b->children) {
		int i;
		for (i = 0; i < b->count; i++) {
			free_box (b->children[i]);
		}
	}

	free (b->children);
	free (b);
}

void
add_child (Box *parent, Box *b)
{
	if (!parent || !b)
		return;

	parent->count++;
	Box **children = realloc (parent->children, (parent->count * sizeof (Box*)));
	if (!children)
		exit (1);

	parent->children = children;
	parent->children[parent->count-1] = b;
}

Box *
new_box (const char *name, Box *parent, Orientation orient, int visible, int min, int max)
{
	Box *b = malloc (sizeof (Box));
	if (!b)
		return NULL;

	b->name     = name;
	b->parent   = parent;
	b->children = NULL;
	b->count    = 0;
	b->orient   = orient;
	b->visible  = visible;
	b->min      = min;
	b->max      = max;

	b->computed.x = -1;
	b->computed.y = -1;
	b->computed.w = -1;
	b->computed.h = -1;

	if (parent) {
		add_child (parent, b);
	}

	return b;
}

int
main ()
{
	Box *top      =       new_box ("top",      NULL,    O_VERTICAL,   0,    1,  -1);

	/* Box *helpline = */ new_box ("helpline", top,     O_HORIZONTAL, 1,    1,   1);
	Box *middle   =       new_box ("middle",   top,     O_HORIZONTAL, 0,    1,  -1);
	/* Box *status   = */ new_box ("status",   top,     O_HORIZONTAL, 1,    1,   1);

	Box *sidebar  =       new_box ("sidebar",  middle,  O_VERTICAL,   1,   20,  20);
	Box *right    =       new_box ("right",    middle,  O_VERTICAL,   0,    1,  -1);

	/* Box *index    = */ new_box ("index",    right,   O_HORIZONTAL, 1,   10,  10);
	/* Box *pager    = */ new_box ("pager",    right,   O_HORIZONTAL, 1,    1,  -1);
	/* Box *helppage = */ new_box ("helppage", right,   O_HORIZONTAL, 0,    1,  -1);

	Rect space = { 0, 0, 140, 30 };
	set_size (top, &space);

	dump_boxes (top, 0);
	printf ("\n");

	// index->visible = 0;
	// set_size (top, &space);
	// dump_boxes (top, 0);
	// printf ("\n");

	sidebar->visible = 0;
	set_size (top, &space);
	dump_boxes (top, 0);
	printf ("\n");

	sidebar->visible = 1;
	set_size (top, &space);
	dump_boxes (top, 0);
	printf ("\n");

	free_box (top);
	return 0;
}

