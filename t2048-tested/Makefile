all: a.out 2048-cleaned.cpp

a.out: 2048.o display.o pack.o
	g++ -g 2048.o pack.o display.o -lSDL -lSDL_ttf

2048.exe: 2048.obj display.obj pack.obj
	i686-w64-mingw32-g++ -static-libgcc -static-libstdc++ -o 2048.exe 2048.obj pack.obj display.obj -Lvendor/SDL-1.2.15/bin -Lvendor/SDL_ttf-2.0.11/lib/x86 -lSDL -lSDL_ttf
	cp vendor/SDL-1.2.15/bin/SDL.dll .
	cp vendor/SDL_ttf-2.0.11/lib/x86/SDL_ttf.dll .
	cp vendor/SDL_ttf-2.0.11/lib/x86/libfreetype-6.dll .
	cp vendor/SDL_ttf-2.0.11/lib/x86/zlib1.dll .

2048.obj: 2048.cpp
	i686-w64-mingw32-g++ --std=gnu++0x -g -o 2048.obj -c 2048.cpp

pack.obj: pack.cpp
	i686-w64-mingw32-g++ --std=gnu++0x -g -o pack.obj -c pack.cpp

display.obj: display.c
	i686-w64-mingw32-gcc --std=gnu99 -g -c -o display.obj -g --std=gnu99 -c -Ivendor/include display.c

display.o: display.c
	gcc -g --std=gnu99 -c display.c

2048.o: 2048.cpp
	g++ -g --std=gnu++11 -c 2048.cpp -o 2048.o

pack.o: pack.cpp
	g++ -g --std=gnu++11 -c pack.cpp -o pack.o

2048-cleaned.cpp: 2048.cpp
	sed 2048.cpp -e '/TALK/d' > 2048-cleaned.cpp

test.out: pack.cpp
	g++ -g --std=gnu++11 -DTEST_PACK -o test.out pack.cpp
	./run-tests.csh

clean:
	rm -f a.out 2048-cleaned.cpp *.o 2048.exe *.obj *.dll *.out
