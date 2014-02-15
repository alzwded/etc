libfft.a: fft.c
	gcc -Wall -c --std=c99 -o fft.o -O4 fft.c
	ar rcs libfft.a fft.o

.PHONY: dist
dist: libfft.a test_archive fft.h
	mkdir -p dist/bin dist/lib dist/include dist/src
	cp libfft.a dist/lib
	cp test_archive dist/bin/test_libfft
	cp fft.h dist/include
	cp fft.c dist/src/
	cp test_archive.c dist/src/test_libfft.c
	tar cvjf libfft-jak.tbz dist/

test_archive: test_archive.c libfft.a
	gcc -o test_archive -g test_archive.c -L. -lfft -lm

dfttest: dft.c
	gcc --std=c99 -o dfttest dft.c -DTEST -lm

fftest: fft.c
	gcc -Wall --std=c99 -g -o fftest fft.c -DTEST -lm

.PHONY: run
run: dfttest fftest
	time -p ./dfttest 440 a > log.slow
	time -p ./fftest 440 a > log.fft

clean:
	rm -rf test log.* fftest log *.o *.a dist

clean-dist:
	rm -rf *.tbz
