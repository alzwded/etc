#include <deque>
#include <list>
#include <stdarg.h>
#include <cstdio>
#include <algorithm>

#define ONLY_IF_TALKING // debug stuff
static bool TALKATIVE = 0;
#define TALK if(TALKATIVE) printf

typedef enum { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 } direction_t;
typedef unsigned char cell_t;
cell_t board[16] = {
    1, 0, 1, 2,
    0, 1, 1, 1,
    0, 0, 1, 0,
    2, 0, 1, 1,
};

// resolves movement on cells
class Pack {
    // pointers to the actual cells that will be modified
    std::deque<cell_t*> pointers_;

    // perform actual shift removing any 0 values (empty cells)
    void shift_(std::list<cell_t>& cells)
    {
        TALK("new shift\n");
        // copy values into results vector
        std::transform(
                pointers_.rbegin(),
                pointers_.rend(),
                std::inserter(cells, cells.begin()),
                [&](decltype(pointers_)::value_type const& p) { return *p; });

        ONLY_IF_TALKING for(auto i = cells.begin(); i != cells.end(); ++i) {
            TALK("%d ", *i);
        ONLY_IF_TALKING }
        TALK("\n");

        for(auto cell = cells.begin(), next = cell;
                cell != cells.end() && ++next != cells.end();
                next = cell)
        {
            TALK("I am %d\n", *cell);
            ONLY_IF_TALKING for(auto cell = cells.begin(); cell != cells.end(); ++cell) {
                TALK("%d ", *cell);
            ONLY_IF_TALKING }
            TALK("\n");
            if(!*cell) { // special case I don't know how to get rid of: 0 on edge
                TALK("removing self (0)\n");
                cells.erase(cell);
                cell = next;
            } else if(!*next) { // eliminate empty cells
                TALK("removing buddy (0)\n");
                cells.erase(next);
            } else if(*cell == *(next)) { // merge cells of equal value
                TALK("merging with buddy (%d)\n", *cell);
                ++*cell;
                cells.erase(next);
                ++cell;
            } else { // default, move to next cell
                TALK("moving along\n");
                ++cell;
            }
        }
    }

    struct Translator {
        std::list<cell_t> const& cells_;
        std::list<cell_t>::const_iterator i_;
        Translator(std::list<cell_t> const& cells)
            : cells_(cells)
            , i_(cells.begin())
        {}

        void operator()(cell_t* cell)
        {
            if(i_ != cells_.end()) *cell = *i_++;
            else *cell = 0;
        }
    };

public:
    typedef std::deque<cell_t*> Builder;

    Pack(cell_t* first, ...)
    {
        va_list p;
        va_start(p, first);
        for(cell_t* i = first; i; i = va_arg(p, cell_t*)) {
            TALK("pushing %d\n", *i);
            pointers_.push_back(i);
        }
    }

    Pack(Builder const& bld)
    {
        for(Builder::const_iterator i = bld.begin();
                i != bld.end(); ++i)
        {
            TALK("pushing %d\n", **i);
            pointers_.push_back(*i);
        }
    }

    // shift cells left-to-right relative to the order the cells were
    //     passed into the constructor
    void shift()
    {
        std::list<cell_t> cells;
        shift_(cells);
        std::for_each(pointers_.rbegin(), pointers_.rend(), Translator(cells));
    }
};

// a line is defined as A*i + B*j + B0, for i = 0..3 and j = 0..3
template<int A, int B0, int B>
void tshift()
{
    for(size_t i = 0; i < 4; ++i) {
        Pack::Builder bld;
        for(size_t j = 0; j < 4; ++j) {
            bld.push_back(&board[A * i + B * j + B0]);
        }
        Pack p(bld);
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
