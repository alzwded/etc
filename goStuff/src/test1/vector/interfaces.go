package vector

/*
   camelCase fields: private
   PascalCase: public
*/

type Addressable interface {
    Get(i int) interface{}
    Set(i int, val interface{})
}

type Iterator interface {
    Key() interface{}
    Value() interface{}
    Next() bool
    More() bool
}

type Iteratable interface {
    Iterator() Iterator
}

type Insertable interface {
    InsertBefore(Iterator, interface{})
    InsertAfter(Iterator, interface{})
}

type Appendable interface {
    PushBack(interface{})
    PopBack() interface{}
}

type Prependable interface {
    PushFront(interface{})
    PopFront() interface{}
}

type Erasable interface {
    Erase(Iterator)
}

type Vector interface {
    Addressable
    Iteratable
    Appendable
}

type LinkedList interface {
    Iteratable
    Appendable
    Prependable
    Erasable
    Insertable
}
