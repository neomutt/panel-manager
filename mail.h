/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _MAIL_H_
#define _MAIL_H_

#include "bool.h"

typedef struct _panel Panel;

BOOL mail_init     (Panel *parent);
void mail_shutdown (void);

#endif /* _MAIL_H_ */
