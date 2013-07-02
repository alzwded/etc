package main

/*
   you can do import . "name" also
*/

import (
    "fmt"
    "test1/vector"
)

func main() {
    //y := vector.NewVectorlol()
    //var x vector.Addressable = y //vector.Addressable(y)
    var x vector.Vector = vector.NewVectorlol()

    for i := 0 ; i < 10 ; i++ {
        x.Set(i, i)
    }
    for i := 0 ; i < 10 ; i++ {
        fmt.Println(x.Get(i).(int) + 1)
    }
    for i := x.Iterator() ; i.More() ; i.Next() {
        fmt.Printf("%d) %d\n", i.Key(), i.Value())
    }
    x.PushBack(13)
    for i := x.Iterator() ; i.More() ; i.Next() {
        fmt.Printf("%d) %d\n", i.Key(), i.Value())
    }
    fmt.Printf("popped: %d, %d\n", x.PopBack(), x.PopBack())
    for i := x.Iterator() ; i.More() ; i.Next() {
        fmt.Printf("%d) %d\n", i.Key(), i.Value())
    }
}
