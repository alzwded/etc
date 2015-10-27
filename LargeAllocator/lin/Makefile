test_MM: ../MM.cpp ../MM.hxx ../FileUtils.h ../FileUtils_lin_gcc.c
	gcc -g -I.. -c ../FileUtils_lin_gcc.c
	g++ -g -DMM_TEST -I.. --std=gnu++11 -c ../MM.cpp
	g++ -o test_MM FileUtils_lin_gcc.o MM.o

clean:
	rm -f test_MM test_MM.exe FileUtils_lin_gcc.o MM.o *.tmp
