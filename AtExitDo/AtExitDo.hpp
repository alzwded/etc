#ifndef AT_EXIT_DO_HPP
#define AT_EXIT_DO_HPP

// include & compile with g++ --std=gnu++14

// VS: apparently can only be compiled with VS2015
//     there's a bug related to variadic template parameter
//     expansion in 2013

#include <functional>

template<typename T, typename... TArg>
struct AtExit_s {
    std::function<void(void)> func_;

    AtExit_s(T&& func, TArg&&... data)
    : func_([&]() { func(data...); })
    {}

    ~AtExit_s()
    {
        func_();
    }
    AtExit_s(AtExit_s&) = delete;
    AtExit_s(AtExit_s&& other)
    : func_(other.func_)
    {
        other.func_ = [](){};
    }
};


template<typename T, typename... TArg>
using AtExit_t = volatile AtExit_s<T, TArg...>;

struct AtExit {
    template<typename T, typename... TArg>
    static inline AtExit_t<T, TArg...> Do(T&& func, TArg&&... data)
    {
        return std::move(AtExit_t<T, TArg...>(std::forward<decltype(func)>(func), std::forward<decltype(data)>(data)...));
    }
};

#endif
