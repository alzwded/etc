# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = /home/jakkal/Projects/gtest-1.7.0/
GMOCK_DIR = /home/jakkal/Projects/gmock-1.7.0/

# Where to find user code.
USER_DIR = ./

# put the names of your tests here in <filenameToBeTested>_unittest format
TESTS = 

all : _Makefile
	make -f _Makefile -d TESTS="$(TESTS)"

_Makefile: Makefile.atom.template Makefile.template
	./genMakeFile.sh $(TESTS)

clean :
	rm -f $(TESTS) _Makefile gtest.a gtest_main.a *.o gmock.a gmock_main.a

