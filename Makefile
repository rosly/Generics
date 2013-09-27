# This file is a part of Generics project
# Copyright (c) 2013, Radoslaw Biernaki <radoslaw.biernacki@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1) Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2) Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3) No personal names or organizations' names associated with the 'Generics'
#    project may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE RADOS PROJET AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

ECHO = /bin/echo -e

#by defining ARCH enviroment variable, user can compile for different architectures
#if no ARCH variable is defined, architecture is defined by target.mk
ifeq ($(ARCH),)
include target.mk
endif
#each architecture have its own target.mk file where CC, ARCHSOURCES, CFLAGS
#variables are defined
include arch/$(ARCH)/target.mk

SOURCEDIR = .
BUILDDIR = build/$(ARCH)
INCLUDEDIR = . arch/$(ARCH)
SOURCES = \
	circfifo.c \
	crc.c \
	$(ARCHSOURCES)

#in target.mk for each source the optimal optimization level (CFLAGS = -Ox) is defined
#but here we add CFLAGS += -g if debug build
ifeq ($(DEBUG),)
CFLAGS += -g
endif
#regardles architecture we use highest warning level
CFLAGS += -Wall -Wextra -Werror
LDFLAGS +=

vpath %.c $(SOURCEDIR) arch/$(ARCH)
vpath %.o $(BUILDDIR)
vpath %.elf $(BUILDDIR)
DEPEND = $(addprefix $(BUILDDIR)/, $(SOURCES:.c=.d))
OBJECTS = $(addprefix $(BUILDDIR)/, $(SOURCES:.c=.o))
LISTINGS = $(addprefix $(BUILDDIR)/, $(SOURCES:.c=.lst))
BUILDTARGET = $(BUILDDIR)/libgenerics.a

all: $(BUILDTARGET) size
lst: $(LISTINGS)

$(BUILDTARGET): $(OBJECTS)
	@$(ECHO) "[AR]\t$@"
	@$(RM) $@
	@$(AR) -cq $@ $^

$(BUILDDIR)/%.o: %.c
	@$(ECHO) "[CC]\t$<"
	@$(CC) -c $(CFLAGS) -o $@ $(addprefix -I, $(INCLUDEDIR)) $<

$(BUILDDIR)/%.lst: %.o
	@$(ECHO) "[LST]\t$<"
	@$(OBJDUMP) -dStw $< > $@

size: $(BUILDTARGET)
	@$(ECHO) "[SIZE]\t$^"
	@$(SIZE) -t $^

# include the dependencies unless we're going to clean, then forget about them.
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPEND)
endif
# dependencies file
# includes also considered, since some of these are our own
# (otherwise use -MM instead of -M)
$(BUILDDIR)/%.d: %.c
	@$(ECHO) "[DEP]\t$<"
	@$(CC) -M ${CFLAGS} $(addprefix -I, $(INCLUDEDIR)) $< >$@

.PHONY: clean test testrun lst size

clean:
	@$(RM) $(BUILDTARGET); $(ECHO) "[RM]\t$(BUILDTARGET)"
	@$(RM) $(OBJECTS); $(ECHO) "[RM]\t$(OBJECTS)"
	@$(RM) $(DEPEND); $(ECHO) "[RM]\t$(DEPEND)"
	@$(RM) $(LISTINGS); $(ECHO) "[RM]\t$(LISTINGS)"
#	@$(MAKE) --no-print-directory -C test clean

#test: $(BUILDTARGET)
#	@$(MAKE) --no-print-directory -C test

#testrun: test
#	@$(MAKE) --no-print-directory -C test testrun

