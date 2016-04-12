// author:  Vlad Mesco
// date:    12/04/2016
// description:
//   Create a by-value type that requires an allocator for internal
//   allocations that uses the calling library's malloc, or any
//   allocator it might want to use via arguments

#ifndef HEAD_H
#define HEAD_H

#ifndef ALLOCATOR
# define ALLOCATOR malloc
#endif

#ifndef DEALLOCATOR
# define DEALLOCATOR free
#endif

struct B
{
    typedef void* (*alloc_t)(size_t);
    typedef void (*dealloc_t)(void*);
    alloc_t alloc_;
    dealloc_t dealloc_;
    int* data_;
    B(alloc_t alloc, dealloc_t dealloc)
        : alloc_(alloc)
        , dealloc_(dealloc)
        , data_((int*)0)
    {}

    virtual ~B()
    {
        dealloc_(data_);
    }
};

// using dummy template arguments to get the correct malloc from the declaration context
template<typename = void>
struct A : public B
{
    A(B::alloc_t a = ALLOCATOR, B::dealloc_t b = DEALLOCATOR)
        : B(a, b)
    {}
};

// remove the need to specify an empty template argument list
#define A A<>

#endif
