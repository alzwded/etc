#ifndef _MINIFLOAT_H
#define _MINIFLOAT_H

#define MAX_INT (0x7FFFFFFF)

class Minifloat {
    struct {
        unsigned sign:1;
        unsigned exponent:4;
        unsigned mantissa:3;
    } _data;
public:
    enum SPECIAL { NAN, INF, NEG_INF };
    Minifloat(const Minifloat::SPECIAL& flags)
    {
        switch(flags) {
            case NEG_INF:
                _data.sign = 1;
                _data.exponent = 0xF;
                _data.mantissa = 0x0;
                break;
            case INF:
                _data.sign = 0;
                _data.exponent = 0xF;
                _data.mantissa = 0x0;
                break;
            case NAN:
                _data.sign = 0;
                _data.exponent = 0xF;
                _data.mantissa = 0x7;
        }
    }
    Minifloat(int x)
    {
        if(x < 0) {
            _data.sign = 1;
            x = -x;
        } else {
            _data.sign = 0;
        }
        if(x <= 7) {
            _data.exponent = 0;
            _data.mantissa = x;
            return;
        }
        _data.exponent = 1;
        while(x > 0xF) {
            x >>= 1;
            _data.exponent++;
            if((_data.exponent ^ 0xF) == 0x0) {
                _data.mantissa = 0;
                return;
            }
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
        if(IsInfinity()) if(_data.sign) return -MAX_INT;
                         else return MAX_INT;
        if(IsNaN()) return 0;
        unsigned int magic = 
            (((((_data.exponent ^ 0x0) == 0x0) ? 0x0 : 0x8)
              | _data.mantissa)
                 << ((_data.exponent) ? _data.exponent - 1 : 0));
        if(_data.sign) return -magic;
        else return magic;
    }

    //friend ostream& operator<<(ostream& o, const Minifloat&);

};

#endif
