#include <deque>
#include <list>
#include <stdarg.h>
#include <cstdio>
#include <algorithm>
#include <set>
#include <cstring>
#include "header.h"


cell_t board[16] = {
    1, 0, 1, 2,
    1, 1, 1, 0,
    0, 0, 1, 0,
    2, 1, 0, 1
};

// resolves movement on cells
class Pack {
    // pointers to the actual cells that will be modified
    std::deque<cell_t*> pointers_;

    // perform actual shift removing any 0 values (empty cells)
    void shift_(std::list<cell_t>& cells, int& moved)
    {
        moved = 0;
        // copy values into results vector
        std::transform(
                pointers_.rbegin(),
                pointers_.rend(),
                std::inserter(cells, cells.begin()),
                [&](decltype(pointers_)::value_type const& p) { return *p; });


        for(auto cell = cells.begin(), next = cell;
                cell != cells.end() && ++next != cells.end();
                next = cell)
        {
            if(!*cell) { // special case I don't know how to get rid of: 0 on edge
                cells.erase(cell);
                cell = next;
                if(next != cells.end() && *next) moved = 1;
            } else if(!*next) { // eliminate empty cells
                cells.erase(next);
                next = cell;
                next++;
                if(next != cells.end() && *next) moved = 1;
            } else if(*cell == *(next)) { // merge cells of equal value
                moved = 1;
                ++*cell;
                cells.erase(next);
                ++cell;
            } else { // default, move to next cell
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
            pointers_.push_back(i);
        }
    }

    Pack(Builder const& bld)
    {
        std::copy(bld.begin(), bld.end(), std::inserter(pointers_, pointers_.begin()));
    }

    // shift cells left-to-right relative to the order the cells were
    //     passed into the constructor
    int shift()
    {
        std::list<cell_t> cells;
        int anythingAccomplished = 0;
        shift_(cells, anythingAccomplished);
        std::for_each(pointers_.rbegin(), pointers_.rend(), Translator(cells));
        return anythingAccomplished;
    }
};

// a line is defined as A*i + B*j + B0, for j = 0..3 and a given i
template<int A, int B0, int B>
int tshift()
{
    int ret = 0;
    for(size_t i = 0; i < 4; ++i) {
        Pack::Builder line;
        for(size_t j = 0; j < 4; ++j) {
            line.push_back(&board[A * i + B * j + B0]);
        }
        ret = Pack(line).shift() || ret;
    }
    return ret;
}

int nop() {}

shift_fn shift[5] = {
    &nop,
    &tshift<1, 12, -4>,
    &tshift<4, 3, -1>,
    &tshift<1, 0, 4>,
    &tshift<4, 0, 1>,
};

void addRandomTile()
{
    cell_t maxi = 1;
    std::set<cell_t> cells;
    for(size_t i = 0; i < 16; cells.insert(i++));

    static auto n = time(NULL);
    n = n * 3913 + 23;

    while(cells.size()) {
        auto i = cells.begin();
        std::advance(i, n % cells.size());
        if(!board[*i]) { board[*i] = maxi; break; }
        else { maxi = std::max(maxi, (cell_t)(board[*i] / 2)); }
        cells.erase(i);
    }
}

int main(int argc, char* argv[])
{
    memset(board, 0, 16); // TODO uncomment after actual game is implemented

    addRandomTile();
    //addRandomTile();

    init_display(&argc, argv);
    loop();

    return 0;
}
