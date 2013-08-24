#include <cstdio>
#include <cstdlib>

struct Node {
        int value;
        struct Node* next;
        Node() : value(0xDEADBEEF), next(NULL) {}
        Node(int const val) : value(val), next(NULL) {}

        ~Node();

        void* operator new(size_t s) { return malloc(s); }
        void operator delete(void* p)
        {
            if(p) {
                printf("eliminating %d\n", static_cast<Node*>(p)->value);
                free(p);
            }
        }
};

void populate(Node& l, int* const values, int const size, signed int const loopAt)
{
    Node* i = &l;
    signed int count = loopAt;
    Node* loop(NULL);

    for(int k = 0; k < size; ++k) {
        i->next = new Node(values[k] += 10);
        i = i->next;
        if(count-- == 0) loop = i;
    }
    i->next = loop;
}

Node::~Node()
{
    if(value ^ 0xDEADBEEF) printf("destroying %d\n", value);
    else printf("destroying list head\n");
    Node* n1 = this->next;
    Node* n2 = (this->next) ? (this->next->next) : NULL;
    if(!n1) {
        return;
    }
    if(!n2) {
        free(n1);
        return;
    }
    int ll1 = 0;
    while(n1 != n2) {
        ll1++;
        n1 = n1->next;
        if(!n2 || !(n2 = n2->next)) {
            for(Node* i = next; i;) {
                Node* next = i->next;
                printf("eliminating %d\n", i->value);
                free(i);
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
        printf("eliminating %d\n", n2->value);
        free(n2);
    }
}

#define TEST(vals, loop) do{\
    int k = __COUNTER__; \
    printf("test #%d\n", k); \
    { \
        Node list; \
        populate(list, vals, sizeof(vals) / sizeof(vals[0]), loop); \
    } \
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
