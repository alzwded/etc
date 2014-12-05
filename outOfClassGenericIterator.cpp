/*
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
* void init(T&), void next(T&), bool checkend(T&), diff(T&, T&) and Y val(T&) functions to be bound
  to the GenericIterator template instantiation. They do what you expect.
  The diff function is called to compare with whatever end() returns
  (or nothing, or something else entirely, it's up to you)
  If you don't know what to expect, imagine them being used as such:
  
      MyCollection c;
      for(init(c); checkend(c); next(c)) {
          auto&& val = val(c);
      }

  That is not any actual code. You can call it using whatever state variable you want. For example, you can create a class called something like

      struct IteratorImplState
      {
          MyCollection collection_;
          int someInt_;
          void* otherStuff_;

          // methods...
      };

  And use that instead of MyCollection directly. This is essentially the
  data that gets passed in to the four functors.
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
        //return false;
        return idx_ < 5;
    }
    int& Value()
    {
        if(idx_ >= 5) { std::cerr << "fail" << std::endl; }
        return data_[idx_];
    }
};

/* GENERIC ITERATOR */

template<typename T, typename FInit, typename FCheckEnd, typename FNext, typename FVal, typename FDiff>
class GenericIterator
{
    T data_;
    FCheckEnd fmore_;
    FNext fnext_;
    FVal fval_;
    FDiff fdiff_;
    typedef GenericIterator<T, FInit, FCheckEnd, FNext, FVal, FDiff> my_type;
public:
    typedef std::ptrdiff_t difference_type;
    typedef int value_type;
    typedef int* pointer;
    typedef int& reference;
    typedef std::forward_iterator_tag iterator_category;

private:
    GenericIterator(T data, FCheckEnd fmore, FNext fnext, FVal fval, FDiff fdiff)
        : data_(data)
        , fmore_(fmore)
        , fnext_(fnext)
        , fval_(fval)
        , fdiff_(fdiff)
    {} 

public:

    my_type operator++()
    {
        fnext_(data_);
        fmore_(data_);
        return *this;
    }

    static my_type First(T data, FInit finit, FCheckEnd fcheckend, FNext fnext, FVal fval, FDiff fdiff)
    {
        finit(data);
        fcheckend(data);
        my_type ret(data, fcheckend, fnext, fval, fdiff);
        return ret;
    }

    static my_type Middle(T data, FInit finit, FCheckEnd fcheckend, FNext fnext, FVal fval, FDiff fdiff)
    {
        fcheckend(data);
        my_type ret(data, fcheckend, fnext, fval, fdiff);
        return ret;
    }

    bool operator!=(my_type& o) { return fdiff_(data_, o.data_); }

    auto operator*() -> decltype(fval_(data_)) { return fval_(data_); }
};

/* IMPLEMENTING GENERIC ITERATOR FOR MYCOLLECTION */

namespace MyCollectionIteratorStuff {
    struct Enumerator
    {
        MyCollection& collection;
        bool last;

        Enumerator(MyCollection& c, bool l = false)
            : collection(c)
            , last(l)
        {}
    };
    void Next(Enumerator& c)
    {
#ifdef VERBOSE
        std::cerr << "next\n";
#endif
        c.collection.Next();
    }
    bool CheckEnd(Enumerator& c)
    {
        bool ret = c.collection.HasMore();
#ifdef VERBOSE
        std::cerr << "more?" << ret << "\n";
#endif
        if(!ret) c.last = true;
        return ret;
    }
    void Init(Enumerator& c)
    {
#ifdef VERBOSE
        std::cerr << "reset\n";
#endif
        c.collection.Reset();
    }
    int& Val(Enumerator& c)
    {
#ifdef VERBOSE
        std::cerr << "value\n";
#endif
        return c.collection.Value();
    }
    bool Diff(Enumerator& c, Enumerator& co)
    {
#ifdef VERBOSE
        std::cerr << "diff " << c.last << co.last << "\n";
#endif
        return c.last != co.last;
    }
}//namespace

typedef GenericIterator<MyCollectionIteratorStuff::Enumerator,
    decltype(&MyCollectionIteratorStuff::Init),
    decltype(&MyCollectionIteratorStuff::CheckEnd),
    decltype(&MyCollectionIteratorStuff::Next),    
    decltype(&MyCollectionIteratorStuff::Val),
    decltype(&MyCollectionIteratorStuff::Diff)
    > MyCollectionIterator;

MyCollectionIterator begin(MyCollection& c)
{
    MyCollectionIteratorStuff::Enumerator e(c);
    return MyCollectionIterator::First(e,
        &MyCollectionIteratorStuff::Init,
        &MyCollectionIteratorStuff::CheckEnd,
        &MyCollectionIteratorStuff::Next,
        &MyCollectionIteratorStuff::Val,
        &MyCollectionIteratorStuff::Diff);
}

MyCollectionIterator end(MyCollection& c)
{
    MyCollectionIteratorStuff::Enumerator e(c, true);
    return MyCollectionIterator::Middle(e,
        &MyCollectionIteratorStuff::Init,
        &MyCollectionIteratorStuff::CheckEnd,
        &MyCollectionIteratorStuff::Next,
        &MyCollectionIteratorStuff::Val,
        &MyCollectionIteratorStuff::Diff);

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
