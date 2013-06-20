#ifndef _MINIFLOAT_H
#define _MINIFLOAT_H

class Minifloat {
    struct {
        unsigned sign:1;
        unsigned exponent:4;
        unsigned mantissa:3;
    } _data;
public:
    Minifloat(int x)
    {
        if(x < 0) {
            _data.sign = 1;
            x = -x;
        } else {
            _data.sign = 0;
        }
        _data.exponent = 0;
        while(x > 0xF) {
            x >>= 1;
            if((_data.exponent ^ 0xF) == 0x0) {
                _data.mantissa = 0;
                return;
            }
            _data.exponent++;
        }
        _data.mantissa = 0x7 & x;
    }

    bool IsInfinity()
    {
        return (_data.exponent ^ 0xF) == 0x0 && (_data.mantissa ^ 0x0) == 0x0;
    }

    bool IsNaN()
    {
        return (_data.exponent ^ 0xF) == 0x0 && (_data.mantissa ^ 0x0) != 0x0;
    }

    bool operator<(const Minifloat& other)
    {
    }

    operator int()
    {
        /* n = (sign ? -1 : 1) * 2^exp * 1.(mantissa); */
        /* subnormal number: mantissa is extended with 0. instead */
        /*
           0 0000 001 = 0.001 * 2^3 = 1
           0 0000 111 = 0.111 * 2^3 = 7
           0 0010 001 = 1.001 * 2^4 = 18
           */
        unsigned int magic = 
            ((_data.sign) ? 0x80000000 : 0x0) |
            (((((_data.exponent ^ 0x0) == 0x0) ? 0x0 : 0x8)
              | _data.mantissa) << (_data.exponent));

        return magic;
    }

    //friend ostream& operator<<(ostream& o, const Minifloat&);

};

#endif
