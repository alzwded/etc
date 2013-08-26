SOURCES = huffman.cpp sources/*

a.out: huffman.cpp
	g++ -O3 huffman.cpp

debug: huffman.cpp
	g++ -DDEBUG_HF -g huffman.cpp

clean:
	rm -f a.out a.log

run: a.out
	cat $(SOURCES) | ./a.out > a.log

.PHONY: clean
.PHONY: optimized
