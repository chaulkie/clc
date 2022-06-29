.POSIX:
# CONFIG

PREFIX = /usr/local

# TARGETS

clc: clc.c
	gcc -o clc clc.c

clean:
	rm -f clc clc.o

install: clc
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f clc $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/clc
	
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/clc

.PHONY: clean install uninstall
