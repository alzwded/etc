#include <stdio.h>
#include <stl/stlCWrapper.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// assert is annoying
#ifdef assert
#undef assert
#endif

// LOG FILE =============
FILE *f;
// RESULT ===============
int failed = 0;

// ASSERT ===============
//#define assert(X, NAME) assert_f(X, __LINE__, NAME)
#define assert(X) assert_f(X, __LINE__, TEST_NAME)

void assert_f(int x, int lineno, char* testName) {
    if(!x) {
        if(f) {
            fprintf(f, "Failed @%d\n", lineno);
            fclose(f);
        } else {
            printf("Failed @%d\n", lineno);
        }
        if(testName) {
            printf("%s failed\n", testName);
        }
        exit(1);
    } 
}

// TEST DEALLOC =========
int remove_called = 0;

void expect_remove() {
    remove_called = 0;
}

void remove_a(void** x)
{
    //fprintf(f, "remove_a called %p\n", *x);
    remove_called = 1;
    free(*x);
}

// TESTS ================
typedef void (*test_t)(void);

void testMap();
void testStringMap();
void testQueue();
void testVector();
//TODO
//void testList();
//void testVector();
//void testSet();
//void testBitSet();
//void testStack();

test_t tests[] = { 
    /*0*/ &testMap, 
    /*1*/ &testStringMap, 
    /*2*/ &testQueue,
    /*3*/ &testVector,
    /*4*/ (test_t)0 /*&testSet*/,
    /*5*/ (test_t)0 /*&testList*/,
    /*6*/ (test_t)0 /*&testBitSet*/,
    /*7*/ (test_t)0 /*&testStack*/,
    /*end*/ (test_t)0};

int children = 0;
void handle(int sig)
{
    int status;
    pid_t who;
    do {
        who = waitpid(-1, &status, WNOHANG);
        if(who > 0) children --;
    }while(who > 0);

    if(status) {
        failed++;
    }
}

// TEST IMPLEMENTATIONS =
#include "test.map.i"
#include "test.stringMap.i"
#include "test.queue.i"
#include "test.vector.i"

// MAIN =================
int waitChildren()
{
    sigset_t set;
    sigset_t all;
    struct sigaction sag;

    sigemptyset(&all);
    
    sag.sa_handler = &handle;
    sag.sa_flags = SA_SIGINFO;
    sag.sa_mask = all;
    sigaction(SIGCHLD, &sag, NULL);

    sigfillset(&set);
    sigdelset(&set, SIGCHLD);
    while(children > 0) sigsuspend(&set);

    if(!failed) {
        printf("all tests pass.\n");
        return 0;
    } else {
        printf("something failed. check logs \n");
        return 1;
    }
}

int main(int argc, char* argv[])
{
    int i;
    int size = children;

    for(i = 0; tests[i]; ++i) {
        children++;
        if(fork()) {
            continue;
        } else {
            tests[i]();
            return 0;
        }
    }

    return waitChildren();
}
