#CPPOPTS = -g -I. --std=c++03
CPPOPTS = -g -I. --std=c++11

a.out: libl.so main.cpp
	g++ -L. $(CPPOPTS)  main.cpp -ll

libl.so: lib.cpp
	g++ -shared -fPIC $(CPPOPTS) -o libl.dll lib.cpp

clean:
	rm -f libl.so a.out
