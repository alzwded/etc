// author:  Vlad Mesco
// date:    12/04/2016
// description:
//   Create a by-value type that requires an allocator for internal
//   allocations that uses the calling library's malloc, or any
//   allocator it might want to use via arguments

#ifndef HEAD_H
#define HEAD_H

typedef void* (*alloc_t)(size_t);
typedef void (*dealloc_t)(void*);

struct B
{
    void* (*alloc_)(size_t);
    void (*dealloc_)(void*);
    int* data_;
};

// using template arguments to get the correct malloc from the declaration context
template<alloc_t ALLOC = malloc, dealloc_t DEALLOC = free>
struct A : public B
{
    A() {
        alloc_ = ALLOC;
        dealloc_ = DEALLOC;
    }
    ~A()
    {
        dealloc_(data_);
    }
};

// remove the need to specify an empty template argument list
#define A_t A<>

#endif
