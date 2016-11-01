#include <utility>
#include <functional>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <dlfcn.h>

template<typename FN>
struct Callback
{
    FN f;
    void* handle;

    template<typename... Args>
    auto operator()(Args... args) const -> decltype(f(args...))
    {
        if(!f) abort();
        return f(args...);
    }

    operator bool() const
    {
        return handle && f;
    }

    ~Callback()
    {
        if(handle) dlclose(handle);
    }
};

template<typename FN>
Callback<FN> GetCallback(std::string dll, std::string name)
{
    Callback<FN> rval;
    rval.handle = dlopen(dll.c_str(), RTLD_LAZY);

    void* sym = dlsym(rval.handle, name.c_str());

    if(!sym) abort();

    rval.f = reinterpret_cast<FN>(sym);

    return rval;
}

template<typename FN, typename... Args, FN dummy = nullptr>
auto CallCallback(std::string dll, std::string name, Args... args) -> decltype(dummy(args...))
{
    return GetCallback<FN>(dll, name)(args...);
}

int main()
{
    typedef int (*cb)(int, int);
    typedef void (*printint)(int);

    int i = GetCallback<cb>("mydll.so", "f")(2, 3);
    printf("%d\n", i);

    GetCallback<printint>("mydll.so", "printint")(1);
    i = CallCallback<cb>("mydll.so", "f", 2, 3);
    CallCallback<printint>("mydll.so", "printint", i);
}
