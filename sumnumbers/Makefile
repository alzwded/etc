USR_LOCAL_BIN = /usr/local/bin

sumnumbers: sumnumbers.c
	gcc -O2 -Wall -o sumnumbers sumnumbers.c -lm

debug: sumnumbers.c
	rm -f sumnumbers && \
	    gcc -g -Wall -o sumnumbers sumnumbers.c -lm

.PHONY: debug

clean:
	rm -f sumnumbers

install:
	install -m 755 -t $(USR_LOCAL_BIN) sumnumbers

uninstall:
	[ -f $(USR_LOCAL_BIN)/sumnumbers ] && rm -f $(USR_LOCAL_BIN)/sumnumbers
