// WARNING: I forget what they're called, but this is like those
//     C include-macros from yore
// minifloat_reduce
// include with MF_RETURN_STATEMENT, MF_DATA and MF_X defined
// parameters:
//     IN
//        MF_RETURN_STATEMENT
//                   method of returning out of the current function
//                   e.g.: `return' , `return ret'
//     IN
//        MF_DATA
//                   a data_t structure in which we will store the result
//     IN
//        MF_X
//                   the number to normalize

#ifndef MF_RETURN_STATEMENT
# error "MF_RETURN_STATEMENT not defined!"
#endif
#ifndef MF_X
# error "MF_X not defined!"
#endif
#ifndef MF_DATA
# error "MF_DATA not defined!"
#endif

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
