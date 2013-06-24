#ifndef _MINIFLOAT_H
#define _MINIFLOAT_H

#define MF_INF_INT      (0x7FFFFFFF)
#define MF_NEG_INF_INT  (0x80000001)
#define MF_NAN_INT      (0x80000000)
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
        if(IsInfinity()) if(_data.sign) return MF_NEG_INF_INT;
                         else return MF_INF_INT;
        if(IsNaN()) return MF_NAN_INT;
        unsigned int magic = 
            (((((_data.exponent ^ 0x0) == 0x0) ? 0x0 : 0x8)
              | _data.mantissa)
                 << ((_data.exponent) ? _data.exponent - 1 : 0));
        if(_data.sign) return -magic;
        else return magic;
    }

    Minifloat operator+(Minifloat o) const
    {
        if(IsNaN() || o.IsNaN()) return Minifloat(NAN);
        if(IsInfinity() && !o.IsInfinity()) {
            return *this;
        }
        if(!IsInfinity() && o.IsInfinity()) {
            return o;
        }
        if(IsInfinity() && o.IsInfinity()) {
            if(_data.sign ^ o._data.sign) return Minifloat(NAN);
            else return *this;
        }

        if(_data.sign && !o._data.sign) return o.operator-(-*this);
        if(!_data.sign && o._data.sign) return operator-(o);

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
        if(rExponent < lExponent) {
            rExponent ^= lExponent, lExponent ^= rExponent, rExponent ^= lExponent;
            right ^= left, left ^= right, right ^= left;
        }
        while(rExponent > lExponent) {
            rExponent--;
            rCarry <<= 1;
            rCarry |= (0x80 & right) != 0;
            right <<= 1;
        }

        Minifloat ret;

        rCarry += (left & 0x80) && (right & 0x80);
        left += right;
        ret._data.sign = _data.sign;

        // remove guard and adapt exponent
        if(rCarry) lExponent++;
        /*unsigned char diff = _data.exponent - o._data.exponent;
        if(diff < 0) diff = -diff;
        if(diff >= MF_GUARD_BITS) lExponent++;*/
        unsigned char shifted = 0;
        while((0x1 & left) == 0x0 && (left ^ 0xFF) != 0x0 && shifted < MF_GUARD_BITS) {
            left >>= 1;
            left |= (rCarry & 0x1) << 7;
            rCarry >>= 1;
            shifted++;
        }
        if(shifted < MF_GUARD_BITS) {
            lExponent -= (MF_GUARD_BITS - 1) - shifted;
            while(lExponent < 0) lExponent++, left <<= 1;
        }

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
        if(IsNaN() || o.IsNaN()) return Minifloat(NAN);
        if(IsInfinity() && !o.IsInfinity()) {
            return *this;
        }
        if(!IsInfinity() && o.IsInfinity()) {
            return o;
        }
        if(o._data.sign) return operator+(-o);
        else if(_data.sign) return operator+(-o);

        if(IsInfinity() && o.IsInfinity()) {
            return Minifloat(NAN);
        }
        
        Minifloat ret;
        unsigned char left = _data.mantissa;
        unsigned char right = o._data.mantissa;
        unsigned char rExponent = o._data.exponent;
        unsigned char lExponent = _data.exponent;

        if(lExponent > rExponent) {
            if(lExponent > 0) {
                left |= 0x8;
                lExponent--;
            }
            if(rExponent > 0) {
                right |= 0x8;
                rExponent--;
            }
            left <<= MF_GUARD_BITS;
            right <<= MF_GUARD_BITS;
            while(rExponent < lExponent) {
                right >>= 1;
                rExponent++;
            }

            unsigned char mask = 0x80;
            rExponent = lExponent;
            while(!(mask & left)) {
                mask >>= 1;
            }
            left -= right;
            while(!(mask & left) && mask > 0x7 && rExponent) {
                mask >>= 1;
                rExponent--;
            }

            ret._data.sign = _data.sign;

            unsigned char shifted = 0;
            if(MF_GUARD_BITS > (lExponent - rExponent)) {
                for(; shifted < MF_GUARD_BITS
                        && left != 0 && !(0x1 & left);
                        shifted++)
                {
                    left >>= 1;
                    mask >>= 1;
                }
                // normalize?
                while(rExponent && mask > 0x8) {
                    mask >>= 1;
                    rExponent--;
                }
                while(rExponent && mask <= 0x8) {
                    mask <<= 1;
                    left <<= 1;
                }
            } else if(MF_GUARD_BITS <= (lExponent - rExponent)) {
                if(left < 0xF && left > 0x7) rExponent++;
            }
            ret._data.exponent = rExponent;
        } else {
            return Minifloat(NAN);
        }

#define MF_RETURN_STATEMENT return ret
#define MF_DATA ret._data
#define MF_X left
# include "minifloat_reduce.h"
        return ret;
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
