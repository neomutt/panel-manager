/* Copyright (c) 2015-2016 Richard Russon <rich@flatcap.org>
 * Released under the GPLv3 -- see LICENSE.md for details */

#ifndef _HELP_H_
#define _HELP_H_

typedef struct _panel Panel;

int  help_init     (Panel *parent);
void help_shutdown (void);

#endif /* _HELP_H_ */
