CFLAGS = -v -funsigned-char -o clpromgen -funsigned-bitfields -fno-signed-bitfields -Q -foptimize-sibling-calls -fmodulo-sched -fmodulo-sched-allow-regmoves

clpromgen: clpromgen.c
	gcc -O3 $(CFLAGS) clpromgen.c

release: clpromgen.c
	gcc -O3 $(CFLAGS) clpromgen.c
.PHONY release:

debug: clpromgen.c
	gcc -g $(CFLAGS) clpromgen.c
.PHONY debug:

clean:
	rm -f clpromgen
