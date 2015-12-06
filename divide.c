#include <stdio.h>
#include <stdlib.h>

#include "panel.h"

void
set_size (Box *b, Rect *r)
{
	if (!b || !r)
		return;

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
new_box (Box *parent, Orientation orient, Visibility visible, int min, int max)
{
	Box *b = malloc (sizeof (Box));
	if (!b)
		return NULL;

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
	Box *top      = new_box (NULL,   O_VERTICAL,   V_HIDDEN,   -1,  -1);

	// Box *helpline =
			new_box (top,    O_HORIZONTAL, V_VISIBLE,   1,   1);
	Box *middle   = new_box (top,    O_HORIZONTAL, V_HIDDEN,   -1,  -1);
	// Box *status   =
			new_box (top,    O_HORIZONTAL, V_VISIBLE,   1,   1);

	// Box *sidebar  =
			new_box (middle, O_VERTICAL,   V_VISIBLE,  30,  30);
	Box *right    = new_box (middle, O_VERTICAL,   V_HIDDEN,   -1,  -1);

	// Box *index    =
			new_box (right,  O_HORIZONTAL, V_VISIBLE,  10,  10);
	// Box *pager    =
			new_box (right,  O_HORIZONTAL, V_VISIBLE,  -1,  -1);
	// Box *helppage =
			new_box (right,  O_HORIZONTAL, V_HIDDEN,   -1,  -1);

	Rect space = { 0, 0, 140, 30 };
	set_size (top, &space);

	free_box (top);
	return 0;
}

