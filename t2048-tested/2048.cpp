#include <deque>
#include <list>
#include <cstdarg>
#include <cstdio>
#include <algorithm>
#include <set>
#include <cstring>
#include <ctime>
#include "header.h"
#include "pack.hxx"

cell_t board[16] = {
    1, 0, 1, 2,
    1, 1, 1, 0,
    0, 0, 1, 0,
    2, 1, 0, 1
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
    addRandomTile();

    init_display(&argc, argv);
    loop();

    return 0;
}
