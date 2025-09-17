CC ?= $(TARGET_PREFIX)gcc
CFLAGS ?= -O2 -Wall -Wextra
LDFLAGS ?=

all: fb0fill

fb0fill: fb0fill.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

install:
	install -d $(DESTDIR)/usr/bin
	install -m 0755 fb0fill $(DESTDIR)/usr/bin/fb0fill

clean:
	rm -f fb0fill
