# GPL License - see http://opensource.org/licenses/gpl-license.php
# Copyright 2006 *nixCoders team - don't forget to credits us

CC = gcc

VERSION = $(shell cat version.def)

CFLAGS = -Wall -fPIC -march=i686 -fno-strict-aliasing -fvisibility=hidden
LDFLAGS = -ldl -lm -shared
# Release/Debug flags
ifdef RELEASE
	CFLAGS += -O3
	LDFLAGS += -s
else
	CFLAGS += -ggdb -DETH_DEBUG
endif


OBJS = aimbot.o commands.o drawtools.o engine.o eth.o g_functions.o hook.o \
	hud.o medias.o medicbot.o menu.o net.o spycam.o visuals.o windows.o
HEADERS = eth.h offsets.h hud.h menu.h net.h types.h windows.h
PROG = libETH.so

# libghf
LIBGHF_VERSION = 0.4-1
LIBGHF_FOLDER = ghf
LIBGHF_FILE = ghf-$(LIBGHF_VERSION).tar.gz
LIBGHF_DOWNLOAD = ./get-ghf.sh
LIBGHF = $(LIBGHF_FOLDER)/libghf.a
LIBGHF_LDFLAGS = -lz -L$(LIBGHF_FOLDER) -lghf
# Release/Debug flags
ifdef RELEASE
	LIBGHF_LDFLAGS += /usr/lib/libelf.a /usr/lib/libbfd.a /usr/lib/libopcodes.a
else
	LIBGHF_LDFLAGS += -lelf -lbfd -lopcodes
endif

# pk3
PK3_FOLDER = pk3
PK3_FILE = eth.pk3

# auto-generate shaders file
SHADERS_SCRIPT = pk3/scripts/eth.shader
SHADERS_DEFINE = shaders.h
HEADERS += $(SHADERS_DEFINE)
SHADERS_MAKER = makeshaders.sh

# sdk
SDK_FOLDER = sdk
SDK_URL = ftp://ftp.mirror.nl/pub/mirror/idsoftware/idstuff/et/sdk/
SDK_FILE = et-linux-2.60-sdk.x86.run

DIST_FILES += $(OBJS:.o=.c) $(HEADERS) $(PROG) $(LIBGHF_FILE) $(PK3_FILE) $(SDK_FOLDER) $(PK3_FOLDER) \
	$(SHADERS_MAKER) CHANGELOG CREDITS INSTALL LICENSE Makefile README run.sh version.def
DIST_FOLDER = eth-$(VERSION)

# Private
ifdef PRIVATE
	CFLAGS += -DETH_PRIVATE
	OBJS += private.o
	HEADERS += private.h
endif

# Default
all: $(SDK_FOLDER) $(LIBGHF) $(SHADERS_DEFINE) $(PK3_FILE) $(PROG)

# Compile
%.o: %.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -DETH_PK3_FILE=\"$(PK3_FILE)\" -DETH_VERSION=\"$(VERSION)\" -c -o $@ $<
q_math.o: sdk/src/game/q_math.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

# Link
$(PROG): $(OBJS) q_math.o
	 $(CC) $(OBJS) q_math.o -o $(PROG) $(LDFLAGS) $(LIBGHF_LDFLAGS) 

# libghf
$(LIBGHF): $(LIBGHF_FILE)
	tar xzf $(LIBGHF_FILE)
	make -C $(LIBGHF_FOLDER) clean all $(if $(RELEASE), RELEASE=1)
$(LIBGHF_FILE):
	@test -f $(LIBGHF_DOWNLOAD) || (echo "Can't find $(LIBGHF_FILE)" && /bin/false)
	$(LIBGHF_DOWNLOAD)

# sdk
$(SDK_FOLDER):
	test -f $(SDK_FILE) || wget $(SDK_URL)$(SDK_FILE)
	mkdir $(SDK_FOLDER)
	/bin/sh $(SDK_FILE) --tar xfC $(SDK_FOLDER)
	@# Cleanup sdk
	find $(SDK_FOLDER) -not -name "q_math.c" -not -iname "*.h" -exec rm -f {} \; 2> /dev/null || /bin/true
	find $(SDK_FOLDER) -type d -exec rmdir -p {} \; 2> /dev/null || /bin/true
	rm -rf $(SDK_FOLDER)/src/botai
	@# Change 'Window' var name for no conflict with xlib
	sed -e "s/Window/eth_Window/" $(SDK_FOLDER)/src/ui/ui_shared.h -i

# pk3
$(PK3_FILE): $(SHADERS_SCRIPT)
	cd $(PK3_FOLDER) && zip -q9r ../$(PK3_FILE) . -x '*.svn*' 

# auto-generate shaders files
$(SHADERS_DEFINE) $(SHADERS_SCRIPT): $(SHADERS_MAKER)
	/bin/sh $(SHADERS_MAKER)

clean:
	rm -rf $(PROG) $(OBJS) $(LIBGHF_FOLDER) $(PK3_FILE) $(SDK_FOLDER) \
	 $(SHADERS_SCRIPT) $(SHADERS_DEFINE) q_math.o private.o

dist:
	@test ! -f eth-$(VERSION).tar.gz || (echo eth-$(VERSION).tar.gz already exist. && /bin/false)
	make clean all RELEASE=1
	mkdir $(DIST_FOLDER)
	cp -R $(DIST_FILES) $(DIST_FOLDER)
	find $(DIST_FOLDER) -type d -name ".svn" -exec rm -rf {} \; 2> /dev/null || /bin/true
	tar --numeric-owner -czf eth-$(VERSION).tar.gz $(DIST_FOLDER)
	rm -rf $(DIST_FOLDER)

help:
	@echo "For this Makefile options see README"
