// minifloat_reduce
// include with MF_RETURN_STATEMENT, MF_DATA and MF_X define

        if(MF_X <= 7 && MF_DATA.exponent < 1) { 
            MF_DATA.exponent = 0; 
            MF_DATA.mantissa = MF_X; 
            MF_RETURN_STATEMENT ; 
        } 
        if(MF_DATA.exponent < 1) MF_DATA.exponent = 1;
        while(MF_X > 0xF) { 
            MF_X >>= 1; 
            MF_DATA.exponent++; 
            if((MF_DATA.exponent ^ 0xF) == 0x0) { 
                MF_DATA.mantissa = 0; 
                MF_RETURN_STATEMENT ; 
            } 
        } 
        MF_DATA.mantissa = 0x7 & MF_X; 

#undef MF_DATA
#undef MF_RETURN_STATEMENT
#undef MF_X
