CC ?= $(TARGET_PREFIX)gcc
CFLAGS ?= -O2 -Wall -Wextra
LDFLAGS ?=

all: touchread

touchread: touchread.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

install:
	install -d $(DESTDIR)/usr/bin
	install -m 0755 touchread $(DESTDIR)/usr/bin/touchread

clean:
	rm -f touchread
