# Copyright (c) 2015 Richard Russon <rich@flatcap.org>
# Released under the GPLv3 -- see LICENSE.md for details

V	?= 0
CC	?= gcc
RM	= rm -fr
MKDIR	= mkdir -p

DEPDIR	= .dep
OBJDIR	= .obj

OUT	= panel

SRC	+= contact.c gfx.c log.c mail.c main.c panel.c rect.c signal.c
HDR	+= bool.h gfx.h log.h notify.h panel.h rect.h signal.h

OBJ	= $(SRC:%.c=$(OBJDIR)/%.o)

CFLAGS	+= -Wall
CFLAGS	+= -Wextra
CFLAGS	+= -Wpedantic
CFLAGS	+= -g

LDFLAGS	+= -pthread
LDFLAGS	+= -rdynamic

CFLAGS	+= -fno-omit-frame-pointer
CFLAGS	+= -fno-optimize-sibling-calls

PACKAGES += ncurses

CFLAGS	+= $(shell pkg-config --cflags $(PACKAGES))
LDFLAGS	+= $(shell pkg-config --libs   $(PACKAGES))

all:	$(OBJDIR) $(DEPDIR) $(OBJ) $(OUT) tags

# ----------------------------------------------------------------------------

ifneq ($(V),1)
Q	:= @
endif
QUIET_CC      = $(Q:@=@echo 'CC      '$<;)
QUIET_LINK    = $(Q:@=@echo 'LD      '$@;)
QUIET_TAGS    = $(Q:@=@echo 'TAGS    '$@;)

ifneq ($(filter s% -s%,$(MAKEFLAGS)),)
	QUIET_CC      =
	QUIET_LINK    =
	QUIET_TAGS    =
endif

# ----------------------------------------------------------------------------

$(OBJDIR)/%.o: %.c
	$(QUIET_CC)$(CC) $(CFLAGS) -c $< -o $@ && (										\
	$(CC) -MM $(CFLAGS) -c $< | sed 's/.*:/'$(OBJDIR)'\/\0/' > $(DEPDIR)/$*.d;						\
	cp -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp;										\
	sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d;		\
	rm -f $(DEPDIR)/$*.d.tmp)

# ----------------------------------------------------------------------------

$(OUT):	$(OBJ)
	$(QUIET_LINK)$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(DEPDIR) $(OBJDIR):
	$(Q)$(MKDIR) $@

# ----------------------------------------------------------------------------

tags:	$(SRC) $(HDR)
	$(QUIET_TAGS)ctags -R .

clean:
	$(Q)$(RM) $(OUT) $(OBJ)

distclean: clean
	$(Q)$(RM) $(DEPDIR) $(OBJDIR) tags

force:

-include $(SRC:%.c=$(DEPDIR)/%.d)

.PHONY:

