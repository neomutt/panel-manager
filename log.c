#include <stdio.h>
#include <stdarg.h>

#include "bool.h"

FILE *flog = NULL;

BOOL
log_open (const char *filename)
{
	if (!filename)
		return FALSE;

	flog = fopen (filename, "a");

	return (flog != NULL);
}

BOOL
log_close (void)
{
	if (!flog)
		return FALSE;

	int res = fclose (flog);

	return (res == 0);
}

int
log_message (const char *fmt, ...)
{
	if (!flog)
		return -1;

	va_list ap;
	int result;

	va_start (ap, fmt);
	result = vfprintf (flog, fmt, ap);
	va_end (ap);

	fflush (flog);

	return result;
}

