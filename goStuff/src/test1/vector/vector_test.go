package vector

/*
   you can do import . "name" also
*/

import (
    "testing"
)

func TestSetGet(t *testing.T) {
    x := NewVectorlol()
    extent := 36
    
    for i := 0 ; i < extent ; i++ {
        x.Set(i, i)
    }
    for i := 0 ; i < extent ; i++ {
        if x.Get(i) != i {
            t.Errorf("SetGet(%d) = %d, want $d", i, x.Get(i), i)
        }
    }

}
