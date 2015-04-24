#ifndef AT_EXIT_DO_HPP
#define AT_EXIT_DO_HPP

// include & compile with g++ --std=gnu++14

#include <functional>

template<typename T, typename... TArg>
struct AtExit_s {
    std::function<void(void)> func_;
    AtExit_s(T& func, TArg&... data)
    : func_([&]() { func(data...); })
    {}
    AtExit_s(T&& func, TArg&&... data)
    : func_([&]() { func(data...); })
    {}
    ~AtExit_s()
    {
        func_();
    }
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
        return std::move(AtExit_t<T, TArg...>(func, data...));
    }

    template<typename T, typename... TArg>
    static inline AtExit_t<T, TArg...> Do(T& func, TArg&... data)
    {
        return std::move(AtExit_t<T, TArg...>(func, data...));
    }
};

#endif
