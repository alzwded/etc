#include <sstream>
#include <cstdio>

struct IAttribute
{
    virtual ~IAttribute() = default;
};

struct IntAttribute : IAttribute
{
    void SetValue(int x)
    {
        printf("Value set on int attribute: %d\n", x);
    }
    ~IntAttribute() override = default;
};

struct FloatAttribute : IAttribute
{
    void SetValue(float x)
    {
        printf("Value set on float attribute: %g\n", x);
    }
    ~FloatAttribute() override = default;
};

struct GenericAttributeSink
{
    void SetGenericValue(std::string input)
    {
        std::stringstream ss;
        ss << input;
        void* target;
        ss >> target;
        IAttribute* a = reinterpret_cast<IAttribute*>(target);
        auto i = dynamic_cast<IntAttribute*>(a);
        auto f = dynamic_cast<FloatAttribute*>(a);
        if(i) 
        {
            int x;
            ss >> x;
            i->SetValue(x);
        }
        if(f)
        {
            float x;
            ss >> x;
            f->SetValue(x);
        }
    }
};

int main()
{
    FloatAttribute f;
    IntAttribute i;
    std::stringstream s1, s2;
    s1 << ((void*)&f) << " " << "3.14";
    s2 << ((void*)&i) << " " << "42";
    GenericAttributeSink sink;
    sink.SetGenericValue(s1.str());
    sink.SetGenericValue(s2.str());
}
