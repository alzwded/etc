#include "pack.hxx"
#include "header.h"
#include <cstdio>
#include <algorithm>
#include <deque>
#include <list>
#include <cstdarg>

namespace { // internal
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
} // namespace

// perform actual shift removing any 0 values (empty cells)
static void shift_(std::list<cell_t>& cells, int& moved)
{
    moved = 0;


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

Pack::Pack(cell_t* first, ...)
{
    va_list p;
    va_start(p, first);
    for(cell_t* i = first; i; i = va_arg(p, cell_t*)) {
        pointers_.push_back(i);
    }
    va_end(p);
}

Pack::Pack(Builder const& bld)
{
    std::copy(bld.begin(), bld.end(), std::inserter(pointers_, pointers_.begin()));
}

// shift cells left-to-right relative to the order the cells were
//     passed into the constructor
int Pack::shift()
{
    std::list<cell_t> cells;
    int anythingAccomplished = 0;
    // copy values into results vector
    std::transform(
            pointers_.rbegin(),
            pointers_.rend(),
            std::inserter(cells, cells.begin()),
            [&](cell_t* p) { return *p; });
    shift_(cells, anythingAccomplished);
    std::for_each(pointers_.rbegin(), pointers_.rend(), Translator(cells));
    return anythingAccomplished;
}

#ifdef TEST_PACK
# include "test.hxx"

TEST_BUNDLE_BEGIN(tests) {
    TEST_BEGIN(EmptyCellIsRemoved) {
        std::list<cell_t> line;
        line.push_back(0);
        line.push_back(0);
        std::list<cell_t> ref;
        ref.push_back(0); // particularity of the implementation, a value already on the edge is left as is since it would not affect the board results in any way (which pads with 0s anyway)
        int moved(0);
        shift_(line, moved);
        assert(line == ref);
    } TEST_END(EmptyCellIsRemoved)
    TEST_BEGIN(ValueCellEatsEmptyCell) {
        std::list<cell_t> line;
        line.push_back(0);
        line.push_back(1);
        std::list<cell_t> ref;
        ref.push_back(1);
        int moved(0);
        shift_(line, moved);
        assert(line == ref);
    } TEST_END(ValueCellEatsEmptyCell)
    TEST_BEGIN(EmptyCellEatenByValueCell) {
        std::list<cell_t> line;
        line.push_back(1);
        line.push_back(0);
        std::list<cell_t> ref;
        ref.push_back(1);
        int moved(0);
        shift_(line, moved);
        assert(line == ref);
    } TEST_END(EmptyCellEatenByValueCell)
    TEST_BEGIN(EqualCellsAreMerged) {
        cell_t randomValue = 42;
        std::list<cell_t> line;
        line.push_back(randomValue);
        line.push_back(randomValue);
        std::list<cell_t> ref;
        ref.push_back(randomValue + 1);
        int moved(0);
        shift_(line, moved);
        assert(line == ref);
    } TEST_END(EqualCellsAreMerged)
    TEST_BEGIN(NonEqualCellsAreNotMerged) {
        cell_t randomValue1 = 42;
        cell_t randomValue2 = 3;
        std::list<cell_t> line;
        line.push_back(randomValue1);
        line.push_back(randomValue2);
        std::list<cell_t> ref;
        ref.push_back(randomValue1);
        ref.push_back(randomValue2);
        int moved(0);
        shift_(line, moved);
        assert(line == ref);
    } TEST_END(NonEqualCellsAreNotMerged)
    TEST_BEGIN(OnlyRightMostCellsAreMerged) {
        cell_t randomValue = 42;
        std::list<cell_t> line;
        line.push_back(randomValue);
        line.push_back(randomValue);
        line.push_back(randomValue);
        std::list<cell_t> ref;
        ref.push_back(randomValue + 1);
        ref.push_back(randomValue);
        int moved(0);
        shift_(line, moved);
        assert(line == ref);
    } TEST_END(OnlyRightMostCellsAreMerged)

    TEST_BEGIN(OnlyEmptyCellsDoesntCountAsAMove) {
        std::list<cell_t> line;
        line.push_back(0);
        line.push_back(0);
        int moved(0);
        shift_(line, moved);
        assert(!moved);
    } TEST_END(OnlyEmptyCellsDoesntCountAsAMove)
    TEST_BEGIN(ValueCellEatsEmptyCellCountsAsAMove) {
        cell_t randomValue = 42;
        cell_t randomValue2 = 3;
        std::list<cell_t> line;
        line.push_back(randomValue);
        line.push_back(0);
        line.push_back(randomValue2);
        int moved(0);
        shift_(line, moved);
        assert(moved);
    } TEST_END(ValueCellEatsEmptyCellCountsAsAMove)
    TEST_BEGIN(MergingCellsCountsAsAMove) {
        cell_t randomValue = 42;
        std::list<cell_t> line;
        line.push_back(randomValue);
        line.push_back(randomValue);
        int moved(0);
        shift_(line, moved);
        assert(moved);
    } TEST_END(MergingCellsCountsAsAMove)
    TEST_BEGIN(NotMergingCellsDoesntCountAsAMove) {
        cell_t randomValue = 42;
        cell_t randomValue2 = 3;
        std::list<cell_t> line;
        line.push_back(randomValue);
        line.push_back(randomValue2);
        int moved(0);
        shift_(line, moved);
        assert(!moved);
    } TEST_END(NotMergingCellsDoesntCountAsAMove)
    TEST_BEGIN(EmptyCellAtTipDoesntCountAsAMove) {
        cell_t randomValue = 42;
        cell_t randomValue2 = 3;
        std::list<cell_t> line;
        line.push_back(randomValue);
        line.push_back(randomValue2);
        line.push_back(0);
        int moved(0);
        shift_(line, moved);
        assert(!moved);
    } TEST_END(EmptyCellAtTipDoesntCountAsAMove)
    TEST_BEGIN(IOfZeroBeingEatenByValueCellCountsAsAMove) {
        cell_t randomValue = 42;
        std::list<cell_t> line;
        line.push_back(0);
        line.push_back(randomValue);
        int moved(0);
        shift_(line, moved);
        assert(moved);
    } TEST_END(IOfZeroBeingEatenByValueCellCountsAsAMove)

    TEST_BEGIN(EatingACellDoesNotMergeNewNeighbours) {
        cell_t randomValue = 42;
        std::list<cell_t> line;
        line.push_back(randomValue);
        line.push_back(randomValue);
        line.push_back(randomValue + 1);
        int moved(0);
        shift_(line, moved);
        assert(moved);
    } TEST_END(EatingACellDoesNotMergeNewNeighbours)
    TEST_BEGIN(EatingACellDoesNotMergeNewNeighboursRevisited) {
        cell_t randomValue = 42;
        std::list<cell_t> line;
        line.push_back(randomValue + 1);
        line.push_back(randomValue);
        line.push_back(randomValue);
        int moved(0);
        shift_(line, moved);
        assert(moved);
    } TEST_END(EatingACellDoesNotMergeNewNeighbours)
    TEST_BEGIN(EatingACellDoesNotMergeNewNeighboursReRevisited) {
        cell_t randomValue = 42;
        std::list<cell_t> line;
        line.push_back(randomValue);
        line.push_back(0);
        line.push_back(randomValue);
        int moved(0);
        shift_(line, moved);
        assert(moved);
    } TEST_END(EatingACellDoesNotMergeNewNeighbours)
    TEST_BEGIN(EatingACellDoesNotMergeNewNeighboursReReRevisited) {
        cell_t randomValue = 42;
        std::list<cell_t> line;
        line.push_back(randomValue + 1);
        line.push_back(0);
        line.push_back(randomValue);
        line.push_back(randomValue);
        int moved(0);
        shift_(line, moved);
        assert(moved);
    } TEST_END(EatingACellDoesNotMergeNewNeighbours)
} TEST_BUNDLE_END(tests);

int main(int argc, char* argv[]) {
    int ret(0);
    TEST_MAIN(tests, ret, argc, argv);
    return ret;
}

#endif
