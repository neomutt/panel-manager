/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#include <stdio.h>
#include <string.h>

#include "notify.h"

char *
notify_flags (Notification n)
{
	static char buffer[10];

	memset (buffer, 0, sizeof (buffer));
	if (n == 0)
		return buffer;

	sprintf (buffer, "(%s%s%s%s%s%s%s)",
		(n & N_SIZE_CHANGED)     ? "S" : "",
		(n & N_POSN_CHANGED)     ? "P" : "",
		(n & N_VISIBLE)          ? "V" : "",
		(n & N_HIDDEN)           ? "H" : "",
		(n & N_DELETED)          ? "D" : "",
		(n & N_REPAINT)          ? "R" : "",
		(n & N_TOO_LITTLE_SPACE) ? "<" : "");

	return buffer;
}

