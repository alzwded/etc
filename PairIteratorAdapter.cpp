#if 0
Copyright 2018 Vlad Mesco

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#endif
#include <vector>
#include <algorithm>
#include <cstdio>
#include <iterator>
#include <memory>


template<class IT, class T = IT::value_type>
class PairIteratorAdapter
{
public:
    typedef std::random_access_iterator_tag iterator_category;
    struct value_type
    {
        T* dupa, *dupb;
        T &mea, &meb;
        value_type(T& a, T& b)
            : dupa(nullptr), dupb(nullptr)
            , mea(a), meb(b)
        {}
        value_type(value_type&& other)
            : dupa(new T(std::move(other.mea))), dupb(new T(std::move(other.meb)))
            , mea(*dupa), meb(*dupb)
        {}
        value_type& operator=(value_type&& other) 
        {
            mea = std::move(other.mea);
            meb = std::move(other.meb);
            return *this;
        }
        ~value_type() { delete dupa; delete dupb; }
        bool operator<(value_type const& other) const
        {
            return mea < other.mea;
        }

    };
    typedef value_type reference;
    typedef value_type const const_reference;
    typedef value_type pointer;
    typedef std::ptrdiff_t difference_type;
    typedef size_t size_type;
private:
    IT m_ptr;

public:
    PairIteratorAdapter(IT ptr) : m_ptr(ptr) {}

    PairIteratorAdapter(const PairIteratorAdapter& it) :
        m_ptr{ it.m_ptr }
    {
    }

    bool operator<(PairIteratorAdapter const& other) const
    {
        return m_ptr < other.m_ptr;
    }

    PairIteratorAdapter& operator=(const PairIteratorAdapter& it)
    {
        m_ptr = it.m_ptr;
        return *this;
    }

    explicit operator bool() const
    {
        return (m_ptr != nullptr);
    }

    bool operator==(const PairIteratorAdapter& it) const
    {
        return m_ptr == it.m_ptr;
    }

    bool operator!=(const PairIteratorAdapter& it) const
    {
        return m_ptr != it.m_ptr;
    }

    PairIteratorAdapter& operator+=(const std::ptrdiff_t& movement)
    {
        m_ptr += movement;
        m_ptr += movement;
        return *this;
    }

    PairIteratorAdapter& operator-=(const std::ptrdiff_t& movement)
    {
        m_ptr -= movement;
        m_ptr -= movement;
        return *this;
    }

    PairIteratorAdapter& operator++()
    {
        ++m_ptr;
        ++m_ptr;
        return *this;
    }

    PairIteratorAdapter& operator--()
    {
        --m_ptr;
        --m_ptr;
        return *this;
    }

    PairIteratorAdapter operator++(int)
    {
        auto temp = *this;
        ++m_ptr;
        ++m_ptr;
        return temp;
    }

    PairIteratorAdapter operator--(int)
    {
        auto temp = *this;
        --m_ptr;
        --m_ptr;
        return temp;
    }

    PairIteratorAdapter operator+(const std::ptrdiff_t& movement)
    {
        auto dup = *this;
        dup.m_ptr += movement;
        dup.m_ptr += movement;
        return dup;
    }

    PairIteratorAdapter operator-(const std::ptrdiff_t& movement)
    {
        auto dup = *this;
        dup.m_ptr -= movement;
        dup.m_ptr -= movement;
        return dup;
    }

    reference operator*()
    {
        return value_type( *m_ptr, *(m_ptr+1) );
    }

    std::ptrdiff_t operator-(const PairIteratorAdapter& it) const
    {
        return std::distance(it.m_ptr, m_ptr) / 2;
    }

    void swap(PairIteratorAdapter& it)
    {
        std::iter_swap(m_ptr, it.m_ptr);
        std::iter_swap(m_ptr + 1, it.m_ptr + 1);
    }
};

template<class IT, class T = IT::value_type>
auto make_pair_iterator(IT it) -> typename PairIteratorAdapter<IT, T>
{
    return PairIteratorAdapter<IT, T>(it);
}

template<typename IT, typename T = IT::value_type>
void swap(typename PairIteratorAdapter<IT, T>::value_type& a, typename PairIteratorAdapter<IT, T>::value_type& b)
{
    std::swap(a.mea, b.mea);
    std::swap(a.meb, b.meb);
}

int main()
{
    std::vector<int> values{ 5,50,4,40,1,10,3,30,2,20 };


    auto it1 = make_pair_iterator(values.begin());
    auto it2 = std::next(it1);
    //std::iter_swap(it1, it2);

    printf("overwrite 5 with 4\n");
    *it1 = *it2;

    for(auto&& i : values) printf("%d\n", i);

    printf("sort ascending\n");

    std::sort(make_pair_iterator(values.begin()), make_pair_iterator(values.end()));

    for(auto&& i : values) printf("%d\n", i);

    printf("sort reverse\n");

    std::sort(make_pair_iterator(values.begin()), make_pair_iterator(values.end()), [](PairIteratorAdapter<decltype(values)::iterator>::value_type const& a, PairIteratorAdapter<decltype(values)::iterator>::value_type const& b) -> bool {
        return a.mea > b.mea;
    });

    for(auto&& i : values) printf("%d\n", i);

    printf("unique pointers\n");
    int aaa[] = { 3, 30, 2, 100 };
    std::vector<std::unique_ptr<int>> ptrs;
    for(auto&& a : aaa) ptrs.emplace_back(new int(a));
    for(auto&& p : ptrs) printf("%d\n", *p);
    typedef PairIteratorAdapter<decltype(ptrs)::iterator>::value_type const& AaaType;
    std::sort(make_pair_iterator(ptrs.begin()), make_pair_iterator(ptrs.end()), [](AaaType a, AaaType b) -> bool {
        return *(a.mea) < *(b.mea);
    });
    for(auto&& p : ptrs) printf("%d\n", *p);

    return 0;
}
