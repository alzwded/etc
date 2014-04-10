all: a.out 2048-cleaned.cpp

a.out: 2048.cpp
	g++ -g --std=gnu++11 2048.cpp

2048-cleaned.cpp: 2048.cpp
	sed 2048.cpp -e '/TALK/d' > 2048-cleaned.cpp

clean:
	rm -f a.out 2048-cleaned.cpp
