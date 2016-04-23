#include <stdio.h>

#include "bool.h"
#include "panel.h"
#include "log.h"

static void
notify (Panel *panel, Notification n)
{
	if (!panel)
		return;

	log_message ("mail notify\n");
}

static Panel*
create_panels (Panel *parent)
{
	if (!parent)
		return NULL;

	Panel *contact   =      panel_new ("contact",  parent,  O_HORIZONTAL, FALSE,  -1, -1);

				panel_new ("groups",   contact, O_VERTICAL,   TRUE,   30, 30);
				panel_new ("contacts", contact, O_VERTICAL,   TRUE,   10, -1);

	contact->notify = notify;
	return contact;
}


BOOL
contact_init (Panel *parent)
{
	if (!parent)
		return FALSE;

	Panel *contact = create_panels (parent);
	if (!contact)
		return FALSE;

	return TRUE;
}

void
contact_shutdown (void)
{
}

