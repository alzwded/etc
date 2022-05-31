USR_LOCAL_BIN = /usr/local/bin

sumnumbers: sumnumbers.c
	gcc -O2 -o sumnumbers sumnumbers.c -lm

clean:
	rm -f sumnumbers

install:
	install -m 755 -t $(USR_LOCAL_BIN) sumnumbers

uninstall:
	[ -f $(USR_LOCAL_BIN)/sumnumbers ] && rm -f $(USR_LOCAL_BIN)/sumnumbers
