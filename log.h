/* Copyright (c) 2015 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _LOG_H_
#define _LOG_H_

#include "bool.h"

BOOL log_open    (const char *filename);
BOOL log_close   (void);
int  log_message (const char *fmt, ...);

#endif /* _LOG_H_ */
