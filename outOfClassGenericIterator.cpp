/*
Copyright 2014 Vlad Mesco

DISCLAIMER: I take no responsibility if using this code
            in any environment results in your cat exploding.
            This is for expositional purposes only.
            
WARNING: This code has many issues. It is a good last resort
         for supporting C++11 range based for on legacy collections
         that are too dumb to let you support it in any more
         meaningful way.
         
To use it you need to implement 6 things:
* a GenericIterator<...> begin(T&) function which in the same namespace as T,
  T being your collection; this is used by the compiler when trying to understand
  for(auto&& i : c)
* a GenericIterator<...> end(T&) function, idem
* void init(T&), void next(T&), bool more(T&) and Y val(T&) functions to be bound
  to the GenericIterator template instantiation. They do what you expect.
  If you don't know what to expect, imagine them being used as such:
  
      MyCollection c;
      for(init(c); more(c); next(c)) {
          auto&& val = val(c);
      }

  That is not any actual code. You can call it using whatever state variable you want.
  For example, you can create a class called something like

      struct IteratorImplState
      {
          MyCollection collection_;
          int someInt_;
          void* otherStuff_;

          // methods...
      };

  And use that instead of MyCollection directly. This is essentially the
  data that gets passed in to the four functors.
  
  There are two factory methods on GenericIterator: First and End.
  The only subtle difference is that FInit is only called for First
  and not for End. I couldn't think of any more meaningful names for
  the two constructors.
*/

#include <algorithm>
#include <utility>
#include <iterator>
#include <iostream>

/* A STUPID COLLECTION */

class MyCollection
{
    int *data_;
    int idx_ = 0;
public:
    /* end shit I had to add */
    MyCollection()
        : idx_(0)
    {
        data_ = new int[5];
        data_[0] = -1 + (data_[1] = -1 + (data_[2] = -1 + (data_[3] = -1 + (data_[4] = 5))));
    }
    ~MyCollection()
    {
        delete[] data_;
    }
    void Reset()
    {
        idx_ = 0;
    }
    void Next()
    {
        idx_++;
    }
    void Prev()
    {
        idx_--;
    }
    bool HasMore()
    {
        return idx_ < 5;
    }
    int& Value()
    {
        if(idx_ >= 5) { std::cerr << "fail" << std::endl; }
        return data_[idx_];
    }
};

/* GENERIC ITERATOR */

template<typename T, typename FInit, typename FMore, typename FNext, typename FVal>
class GenericIterator
{
    T& data_;
    FMore fmore_;
    FNext fnext_;
    FVal fval_;
    bool last_;
    typedef GenericIterator<T, FInit, FMore, FNext, FVal> my_type;
public:
    typedef std::ptrdiff_t difference_type;
    typedef int value_type;
    typedef int* pointer;
    typedef int& reference;
    typedef std::forward_iterator_tag iterator_category;

private:
    GenericIterator(T& data, FMore fmore, FNext fnext, FVal fval, bool last = false)
        : data_(data)
        , fmore_(fmore)
        , fnext_(fnext)
        , fval_(fval)
        , last_(last)
    {}

public:

    my_type operator++()
    {
        fnext_(data_);
        if(fmore_(data_)) {
            return *this;
        } else {
            last_ = true;
            return *this;
        }
    }

    static my_type First(MyCollection& data, FInit finit, FMore fmore, FNext fnext, FVal fval)
    {
        my_type ret(data, fmore, fnext, fval);
        finit(data);
        return ret;
    }

    static my_type End(MyCollection& data, FInit finit, FMore fmore, FNext fnext, FVal fval)
    {
        my_type ret(data, fmore, fnext, fval, true);
        return ret;
    }

    bool operator!=(my_type& o) { return last_ != o.last_; }

    auto operator*() -> decltype(fval_(data_)) { return fval_(data_); }
};

/* IMPLEMENTING GENERIC ITERATOR FOR MYCOLLECTION */

namespace MyCollectionIteratorStuff {
    void Next(MyCollection& c)
    {
#ifdef VERBOSE
        std::cerr << "next\n";
#endif
        c.Next();
    }
    bool More(MyCollection& c)
    {
#ifdef VERBOSE
        std::cerr << "more?\n";
#endif
        return c.HasMore();
    }
    void Init(MyCollection& c)
    {
#ifdef VERBOSE
        std::cerr << "reset\n";
#endif
        c.Reset();
    }
    int& Val(MyCollection& c)
    {
#ifdef VERBOSE
        std::cerr << "value\n";
#endif
        return c.Value();
    }
}//namespace

typedef GenericIterator<MyCollection,
    decltype(&MyCollectionIteratorStuff::Init),
    decltype(&MyCollectionIteratorStuff::More),
    decltype(&MyCollectionIteratorStuff::Next),    
    decltype(&MyCollectionIteratorStuff::Val)
    > MyCollectionIterator;

MyCollectionIterator begin(MyCollection& c)
{
    return MyCollectionIterator::First(c,
        &MyCollectionIteratorStuff::Init,
        &MyCollectionIteratorStuff::More,
        &MyCollectionIteratorStuff::Next,
        &MyCollectionIteratorStuff::Val);
}

MyCollectionIterator end(MyCollection& c)
{
    return MyCollectionIterator::End(c,
        &MyCollectionIteratorStuff::Init,
        &MyCollectionIteratorStuff::More,
        &MyCollectionIteratorStuff::Next,
        &MyCollectionIteratorStuff::Val);
}

/* END IMPLEMENTATION OF GENERIC ITERATOR IMPLEMENTATION FOR MYCOLLECTION */

int main()
{
    MyCollection c;
    for(auto&& i : c) {
        std::cout << i;
    }
    std::cout << std::endl;
    for(auto&& i : c) {
        std::cout << i;
    }
    std::cout << std::endl;
    for(c.Reset(); c.HasMore(); c.Next()) {
        std::cout << c.Value();
    }
    std::cout << std::endl;
    return 0;
}
