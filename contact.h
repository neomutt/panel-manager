/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _CONTACT_H_
#define _CONTACT_H_

#include "bool.h"

typedef struct _panel Panel;

BOOL contact_init     (Panel *parent);
void contact_shutdown (void);

#endif /* _CONTACT_H_ */
