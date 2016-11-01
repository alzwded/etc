call: CallCallback.cpp mydll.so
	g++ -g --std=gnu++11 -o call CallCallback.cpp -ldl

mydll.so: mydll.c
	gcc -shared -g -fPIC -o mydll.so mydll.c

clean:
	rm -f call call.exe mydll.so
