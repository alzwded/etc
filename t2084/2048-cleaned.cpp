#include <deque>
#include <list>
#include <cstdlib>
#include <stdarg.h>
#include <cstdio>
#include <algorithm>


typedef enum { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 } direction_t;
typedef unsigned char cell_t;
cell_t board[16] = {
    1, 0, 1, 2,
    0, 1, 1, 1,
    0, 0, 1, 0,
    2, 0, 1, 1,
};

// resolves movement on line
class Pack {
    // pointers to the actual cells that will be modified
    std::deque<cell_t*> a_;

    // perform actual shift removing any 0 values (empty cells)
    void shift_(std::list<cell_t>& r_)
    {
        // copy values into results vector
        std::transform(
                a_.rbegin(),
                a_.rend(),
                std::inserter(r_, r_.begin()),
                [&](decltype(a_)::value_type const& p) {
                    return *p;
                });


        for(auto i = r_.begin(), next = i;
                i != r_.end() && ++next != r_.end();
                next = i)
        {
            if(!*i) { // special case I don't know how to get rid of: 0 on edge
                r_.erase(i);
                i = next;
            } else if(!*next) { // eliminate empty cells
                r_.erase(next);
            } else if(*i == *(next)) { // merge cells of equal value
                ++*i;
                r_.erase(next);
                ++i;
            } else { // default, move to next cell
                ++i;
            }
        }
    }
public:
    Pack(cell_t* first, ...)
    {
        va_list p;
        va_start(p, first);
        for(cell_t* i = first; i; i = va_arg(p, cell_t*)) {
            a_.push_back(i);
        }
    }

    Pack(cell_t** vcells)
    {
        for(cell_t** i = vcells; *i; ++i) {
            a_.push_back(*i);
        }
    }

    // shift cells left-to-right relative to the order the cells were
    //     passed into the constructor
    void shift()
    {
        std::list<cell_t> R;

        shift_(R);

        auto pa = a_.rbegin();
        for(auto pr = R.begin(); pr != R.end(); ++pr, ++pa) {
            **pa = *pr;
        }
        for(; pa != a_.rend(); ++pa) {
            **pa = 0;
        }
    }
};

// a line is defined as A*i + B*j + B0, for i = 0..3 and j = 0..3
template<int A, int B0, int B>
void tshift()
{
    for(size_t i = 0; i < 4; ++i) {
        int bs[] = { B0, B0 + B, B0 + B + B, B0 + B + B + B };
        Pack p(
                &board[A * i + bs[0]],
                &board[A * i + bs[1]],
                &board[A * i + bs[2]],
                &board[A * i + bs[3]],
                NULL);
        p.shift();
    }
}

typedef void (*shift_fn)();
shift_fn shift[4] = {
    &tshift<1, 12, -4>,
    &tshift<4, 3, -1>,
    &tshift<1, 0, 4>,
    &tshift<4, 0, 1>,
};

int main(int argc, char* argv[])
{
    // FIXME implement actual game
    //memset(board, 0, 16); // TODO uncomment after actual game is implemented

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
