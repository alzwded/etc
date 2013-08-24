#include <cstdio>
#include <cstdlib>

#include <set>

std::set<int> limbo;
void expectToRemoveValue(int const x) { printf("expecting %d to go away\n", x); limbo.insert(x); }
void releaseFromLimbo(int const x) { printf("%d went away\n", x); limbo.erase(limbo.find(x)); }

struct Alloc {
    void* allocate(size_t size) { return malloc(size); }
    void deallocate(void* ptr);
};

template<class ALLOCA>
struct Node {
        typedef Node<ALLOCA> myType;
        static ALLOCA allocator;
        int value;
        myType* next;
        Node() : value(0xDEADBEEF), next(NULL) {}
        Node(int const val) : value(val), next(NULL) {}

        ~Node();

        void* operator new(size_t s) { return allocator.allocate(s); }
        void operator delete(void* p) { allocator.deallocate(p); }
};

void Alloc::deallocate(void* ptr)
{
    if(ptr) {
        int value = *((int*)(ptr));
        releaseFromLimbo(value);
        free(ptr);
    }
}

void populate(Node<Alloc>& l, int* const values, int const size, signed int const loopAt)
{
    Node<Alloc>* i = &l;
    signed int count = loopAt;
    Node<Alloc>* loop(NULL);

    for(int k = 0; k < size; ++k) {
        i->next = new Node<Alloc>(values[k] += 10);
        expectToRemoveValue(values[k]);
        i = i->next;
        if(count-- == 0) loop = i;
    }
    i->next = loop;
}

template<class ALLOC>
ALLOC Node<ALLOC>::allocator;

template<class ALLOC>
Node<ALLOC>::~Node()
{
    if(value ^ 0xDEADBEEF) printf("~Node: %d\n", value);
    else printf("~Node: destroying list head\n");
    myType* n1 = this->next;
    myType* n2 = (this->next) ? (this->next->next) : NULL;
    if(!n1) {
        return;
    }
    if(!n2) {
        allocator.deallocate(n1);
        return;
    }
    int ll1 = 0;
    while(n1 != n2) {
        ll1++;
        n1 = n1->next;
        if(!n2 || !(n2 = n2->next)) {
            for(myType* i = next; i;) {
                myType* next = i->next;
                allocator.deallocate(i);
                i = next;
            }
            return;
        }
        n2 = n2->next;
    }
    
    int sizeOfLoop = 0;
    for(Node* i = n2->next; i != n2; i = i->next) {
        sizeOfLoop++;
    }

    n1 = next;
    for(int count = sizeOfLoop; count >= 0; count--) {
        n1 = n1->next;
    }

    n2 = next;
    while(n1 != n2) {
        n1 = n1->next;
        n2 = n2->next;
    }

    for(int count = sizeOfLoop; count >= 1; count--) {
        n1 = n1->next;
    }
    n1->next = NULL;

    for(n1 = next; n1;) {
        n2 = n1;
        n1 = n1->next;
        allocator.deallocate(n2);
    }
}

#define TEST(vals, loop) do{\
    limbo.clear(); \
    int k = __COUNTER__; \
    printf("test #%d\n", k); \
    { \
        Node<Alloc> list; \
        populate(list, vals, sizeof(vals) / sizeof(vals[0]), loop); \
    } \
    printf("checking everything went away %s\n", (limbo.size() == 0) ? "OK" : "FAIL"); \
    printf("end test#%d\n\n", k); \
}while(0)

int main()
{
    static int v1[] = { 1, 2, 3, 4, 5 };
    static int v2[] = { 11, 12, 13, 14, 15, 16 };

    TEST(v1, -1);
    TEST(v1, 2);
    TEST(v2, 2);
    TEST(v2, 0);
    TEST(v2, 5);
    TEST(v2, 1);

    return 0;
}
