CC = gcc
CFLAGS = -O3 -fPIC
#CFLAGS = -g -fPIC
MAKEDEPEND = $(CC) -MM
HACK_FIXDEPEND = sed -r -i $(@:.o=.d) -e '1 s.*:$@:'
OBJS = obj/encoder.o obj/decoder.o obj/main.o obj/common.o
LIBOBJS = obj/libmain.o obj/encoder.o obj/decoder.o obj/common.o
TESTSTUFF = test.csh test.in test2.in test3.in

lib/libjakcode.so: $(LIBOBJS)
	mkdir -p lib
	$(CC) -shared $(CFLAGS) -fvisibility=hidden -Wl,--no-as-needed,--strip-all,--version-script=src/libjakcode.version -o lib/libjakcode.so $(LIBOBJS)

.PHONY: dist
dist: lib/libjakcode.so
	mkdir -p dist
	cp -r lib ./dist/
	mkdir -p dist/inc
	cp src/libmain.h dist/inc/jakcode.h
	tar cvjf dist/libjakcode.tbz -C dist/ inc lib

.PHONY: dist-test-cpp
dist-test-cpp: dist test/test.cpp test/test.in test/test2.in test/test3.in
	cp test/test.cpp dist/
	g++ -g -o dist/test-cpp -fPIC -Ldist/lib/ -Idist/inc/ -Wl,--rpath=lib,--rpath-link=lib dist/test.cpp -ljakcode
	cp test/test.in dist/

.PHONY: dist-test-c
dist-test-c: dist test/test.c test/test.in test/test2.in test/test3.in
	cp test/test.c dist/
	gcc -g -o dist/test-c -fPIC -Ldist/lib/ -Idist/inc/ -Wl,--rpath=lib,--rpath-link=lib dist/test.c -ljakcode
	cp test/test.in dist/

bin/jakcode: $(OBJS)
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/jakcode $(OBJS)
	tar cv -C ./test $(TESTSTUFF) | tar xv -C ./bin

-include $(OBJS:.o=.d)

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) -c $(CFLAGS) -o $@ $<
	$(MAKEDEPEND) $< > $(@:.o=.d)
	$(HACK_FIXDEPEND)

clean:
	rm -rf obj bin dist lib
