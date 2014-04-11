all: a.out 2048-cleaned.cpp

a.out: 2048.o display.o
	g++ -g 2048.o display.o -lSDL -lSDL_ttf

display.o: display.c
	gcc -g --std=gnu99 -c display.c

2048.o: 2048.cpp
	g++ -g --std=gnu++11 -c 2048.cpp -o 2048.o

2048-cleaned.cpp: 2048.cpp
	sed 2048.cpp -e '/TALK/d' > 2048-cleaned.cpp

clean:
	rm -f a.out 2048-cleaned.cpp *.o
