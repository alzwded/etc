#ifndef PACK_HXX
#define PACK_HXX

#include "header.h"
#include <deque>
#include <cstdarg>

// resolves movement on cells
class Pack {
    // pointers to the actual cells that will be modified
    std::deque<cell_t*> pointers_;

public:
    typedef std::deque<cell_t*> Builder;

    Pack(cell_t* first, ...);

    Pack(Builder const& bld);

    // shift cells left-to-right relative to the order the cells were
    //     passed into the constructor
    int shift();
};

#endif
