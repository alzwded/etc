#ifndef _MINIFLOAT_H
#define _MINIFLOAT_H

#define MF_INF_INT (0x7FFFFFFF)
#define MF_GUARD_BITS 3

class Minifloat {
public:
    enum SPECIAL { NAN, INF, NEG_INF, ZERO, NEG_ZERO };
private:
    typedef struct {
        unsigned sign:1;
        unsigned exponent:4;
        unsigned mantissa:3;
    } data_t;
    data_t _data;
private:
    Minifloat() {}

    void Assign(const int X)
    {
        int x = X;
        if(x < 0) {
            _data.sign = 1;
            x = -x;
        } else {
            _data.sign = 0;
        }
        _data.exponent = 0;
#define MF_RETURN_STATEMENT return
#define MF_DATA _data
#define MF_X x
#include "minifloat_reduce.h"
    }
    void Assign(const Minifloat::SPECIAL& flag)
    {
        switch(flag) {
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
                break;
            case ZERO:
                _data.sign = 0;
                _data.exponent = 0x0;
                _data.mantissa = 0x0;
                break;
            case NEG_ZERO:
                _data.sign = 1;
                _data.exponent = 0x0;
                _data.mantissa = 0x0;
                break;
        }
    }
public:
    Minifloat(const Minifloat::SPECIAL& flag)
    {
        Assign(flag);
    }
    Minifloat(const int x)
    {
        Assign(x);
    }

    bool IsInfinity() const
    {
        return (_data.exponent ^ 0xF) == 0x0 && (_data.mantissa ^ 0x0) == 0x0;
    }

    bool IsInf() const
    {
        return (_data.sign == 0) && (_data.exponent ^ 0xF) == 0x0 && (_data.mantissa ^ 0x0) == 0x0;
    }

    bool IsNegInf() const
    {
        return (_data.sign == 1) && (_data.exponent ^ 0xF) == 0x0 && (_data.mantissa ^ 0x0) == 0x0;
    }

    bool IsNaN() const
    {
        return (_data.exponent ^ 0xF) == 0x0 && (_data.mantissa ^ 0x0) != 0x0;
    }

    bool operator<(const Minifloat& other)
    {
    }

    operator int() const
    {
        /* n = (sign ? -1 : 1) * 2^exp * 1.(mantissa); */
        /* subnormal number: mantissa is extended with 0. instead */
        /*
           0 0000 001 = 0.001 * 2^3 = 1
           0 0000 111 = 0.111 * 2^3 = 7
           0 0010 001 = 1.001 * 2^4 = 18
           */
        if(IsInfinity()) if(_data.sign) return -MF_INF_INT;
                         else return MF_INF_INT;
        if(IsNaN()) return 0;
        unsigned int magic = 
            (((((_data.exponent ^ 0x0) == 0x0) ? 0x0 : 0x8)
              | _data.mantissa)
                 << ((_data.exponent) ? _data.exponent - 1 : 0));
        if(_data.sign) return -magic;
        else return magic;
    }

    // TODO investigate this can be implemented in any other way
    Minifloat operator+(Minifloat o) const
    {
        if(IsNaN() || o.IsNaN()) return Minifloat(NAN);
        if(IsInfinity() && !o.IsInfinity()) {
            if((int)o == 0) return Minifloat(NAN);
            return *this;
        }
        if(!IsInfinity() && o.IsInfinity()) {
            if((int)(*this) == 0) return Minifloat(NAN);
            return o;
        }
        if(IsInfinity() && o.IsInfinity()) {
            if(_data.sign ^ o._data.sign) return Minifloat(NAN);
            else return *this;
        }
        //return Minifloat(((int)(*this)) + ((int)o));
        unsigned char left = _data.mantissa;
        unsigned char right = o._data.mantissa;
        unsigned char rExponent = o._data.exponent;
        unsigned char lExponent = _data.exponent;
        unsigned char rCarry = 0;
        if(lExponent > 0) left |= 0x8, lExponent--;
        if(rExponent > 0) right |= 0x8, rExponent--;
        left <<= MF_GUARD_BITS; // guard
        right <<= MF_GUARD_BITS; // guard
        while(rExponent < lExponent) rExponent++, right >>= 1;
        while(rExponent > lExponent) {
            rExponent--;
            rCarry = 0x80 & right;
            right <<= 1;
        }

        Minifloat ret;

        if(!o._data.sign && !_data.sign) (rCarry ^= (0x80 & left) & (0x80 & right)), left += right, ret._data.sign = 0;
        else if(_data.sign && o._data.sign) (rCarry ^= (0x80 & left) & (0x80 & right)), left += right, ret._data.sign = 1;
        else if(left > right) {
            left -= right;
            if(_data.sign) ret._data.sign = 1;
            else ret._data.sign = 0;
        } else {
            left = right - left;
            if(_data.sign) ret._data.sign = 0;
            else ret._data.sign = 1;
        }

        // remove guard and adapt exponent
        unsigned char shifted = 0;
        while((0x1 & left) == 0x0 && (left ^ 0xFF) != 0x0 && shifted < MF_GUARD_BITS) {
            left >>= 1;
            left |= rCarry;
            rCarry = 0;
            shifted++;
        }
        if(shifted < MF_GUARD_BITS) lExponent -= MF_GUARD_BITS - shifted - 1;
        while(lExponent < 0) lExponent++, left <<= 1;

        ret._data.exponent = lExponent;
#define MF_RETURN_STATEMENT return ret
#define MF_DATA ret._data
#define MF_X left
# include "minifloat_reduce.h"
        return ret;
    }

    Minifloat operator-() const
    {
        Minifloat ret;
        ret._data.sign = ~_data.sign;
        ret._data.exponent = _data.exponent;
        ret._data.mantissa = _data.mantissa;
        return ret;
    }

    Minifloat operator-(Minifloat o) const
    {
        return operator+(-o);
    }

    Minifloat operator*(Minifloat o) const
    {
        if(IsNaN() || o.IsNaN()) return Minifloat(NAN);
        if(IsInfinity() && !o.IsInfinity()) {
            if((int)o == 0) return Minifloat(NAN);
            return Minifloat( ((_data.sign ^ o._data.sign) ? NEG_INF : INF) );
        }
        if(!IsInfinity() && o.IsInfinity()) {
            if((int)(*this) == 0) return Minifloat(NAN);
            return Minifloat( ((_data.sign ^ o._data.sign) ? NEG_INF : INF) );
        }
        if(IsInfinity() && o.IsInfinity()) {
            if(_data.sign ^ o._data.sign) return Minifloat(NAN);
            else return *this;
        }
        Minifloat ret(((int)(*this)) * ((int)o));
        ret._data.sign = _data.sign ^ o._data.sign;
        return ret;
    }

    Minifloat operator/(Minifloat o) const
    {
        if(IsNaN() || o.IsNaN()) return Minifloat(NAN);
        if(IsInfinity() && !o.IsInfinity()) {
            if((int)o == 0) return Minifloat(NAN);
            return Minifloat( ((_data.sign ^ o._data.sign) ? NEG_INF : INF) );
        }
        if(!IsInfinity() && o.IsInfinity()) {
            if((int)(*this) == 0) return Minifloat(NAN);
            return Minifloat( ((_data.sign ^ o._data.sign) ? NEG_ZERO : ZERO) );
        }
        if(IsInfinity() && o.IsInfinity()) {
            if(_data.sign ^ o._data.sign) return Minifloat(NAN);
            else return Minifloat( ((_data.sign ^ o._data.sign) ? NEG_ZERO : ZERO) );
        }
        if((_data.exponent ^ 0x0) == 0x0 && (_data.mantissa ^ 0x0) == 0x0
                && (o._data.exponent ^ 0x0) == 0x0
                && (o._data.mantissa ^ 0x0) == 0x0)
        {
            return Minifloat(NAN);
        }
        if((o._data.exponent ^ 0x0) == 0x0
                && (o._data.mantissa ^ 0x0) == 0x0)
        {
            return Minifloat( ((_data.sign ^ o._data.sign) ? NEG_INF : INF) );
        }

        Minifloat ret(((int)(*this)) / ((int)o));
        ret._data.sign = _data.sign ^ o._data.sign;
        return ret;
    }

    Minifloat& operator=(const int x)
    {
        Assign(x);
        return *this;
    }

    Minifloat& operator=(const Minifloat::SPECIAL& flag)
    {
        Assign(flag);
        return *this;
    }


    // TODO also take into account #inf and #-inf and #nan
    //friend ostream& operator<<(ostream& o, const Minifloat&);
    //friend istream& operator>>(ostream& o, const Minifloat&);

};

typedef Minifloat minifloat_t;

#endif
