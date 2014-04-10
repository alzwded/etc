#include <deque>
#include <list>
#include <cstdlib>
#include <stdarg.h>
#include <cstdio>
#include <algorithm>

static bool TALKATIVE = 0;
#define TALK if(TALKATIVE) printf

typedef enum { UP, LEFT, DOWN, RIGHT } direction_t;
typedef unsigned char cell_t;
cell_t board[16] = {
    1, 0, 1, 2,
    0, 1, 1, 1,
    0, 0, 1, 0,
    2, 0, 1, 1,
};

class Packer {
    std::deque<cell_t*> a_;
    std::list<cell_t> r_;

    void shift_()
    {
        TALK("new shift\n");
        std::transform(
                a_.rbegin(),
                a_.rend(),
                std::inserter(r_, r_.begin()),
                [&](decltype(a_)::value_type const& p) {
                    return *p;
                });

        for(auto i = r_.begin(); i != r_.end(); ++i) {
            TALK("%d ", *i);
        }
        TALK("\n");

        for(auto i = r_.begin(); i != r_.end();) {
            TALK("I am %d\n", *i);
            for(auto i = r_.begin(); i != r_.end(); ++i) {
                TALK("%d ", *i);
            }
            TALK("\n");
            decltype(i) next = i;
            ++next;
            if(next == r_.end()) break;
            if(!*i) {
                TALK("removing self (0)\n");
                ++i;
                decltype(i) toDelete = i;
                toDelete--;
                r_.erase(toDelete);
            } else if(!*next) {
                TALK("removing buddy (0)\n");
                r_.erase(next);
            } else if(*i == *(next)) {
                TALK("merging with buddy (%d)\n", *i);
                ++*i;
                r_.erase(next);
                ++i;
            } else {
                TALK("moving along\n");
                ++i;
            }
        }
    }
public:
    Packer(cell_t* first, ...)
    {
        va_list p;
        va_start(p, first);
        for(cell_t* i = first; i; i = va_arg(p, cell_t*)) {
            TALK("pushing %d\n", *i);
            a_.push_back(i);
        }
    }

    Packer(cell_t** vcells)
    {
        for(cell_t** i = vcells; *i; ++i) {
            TALK("pushing %d\n", **i);
            a_.push_back(*i);
        }
    }

    void unpack()
    {
        shift_();

        auto pa = a_.rbegin();
        for(auto pr = r_.begin(); pr != r_.end(); ++pr, ++pa) {
            **pa = *pr;
        }
        for(; pa != a_.rend(); ++pa) {
            **pa = 0;
        }
    }
};

template<int A, int B0, int B>
void tshift()
{
    for(size_t i = 0; i < 4; ++i) {
        int bs[] = { B0, B0 + B, B0 + B + B, B0 + B + B + B };
        Packer p(
                &board[A * i + bs[0]],
                &board[A * i + bs[1]],
                &board[A * i + bs[2]],
                &board[A * i + bs[3]],
                NULL);
        p.unpack();
    }
}

typedef void (*shift_fn)();
shift_fn shift[4] = {
    &tshift<1, 12, -4>,
    &tshift<4, 3, -1>,
    &tshift<1, 0, 4>,
    &tshift<4, 0, 1>,
    //&shift_up,
    //&shift_left,
    //&shift_down,
    //&shift_right,
};

int main(int argc, char* argv[])
{
    //memset(board, 0, 16);

    printf("before:");
    for(size_t i = 0; i < 16; ++i) {
        if(i % 4 == 0) printf("\n");
        printf("%4d ", board[i]);
    }
    printf("\n");

    shift[RIGHT]();

    printf("after:");

    for(size_t i = 0; i < 16; ++i) {
        if(i % 4 == 0) printf("\n");
        printf("%4d ", board[i]);
    }
    printf("\n");

    shift[LEFT]();

    printf("after more:");
    for(size_t i = 0; i < 16; ++i) {
        if(i % 4 == 0) printf("\n");
        printf("%4d ", board[i]);
    }
    printf("\n");

    shift[UP]();

    printf("after up:");
    for(size_t i = 0; i < 16; ++i) {
        if(i % 4 == 0) printf("\n");
        printf("%4d ", board[i]);
    }
    printf("\n");

    shift[DOWN]();

    printf("after down:");
    for(size_t i = 0; i < 16; ++i) {
        if(i % 4 == 0) printf("\n");
        printf("%4d ", board[i]);
    }
    printf("\n");
}
