package vector

import "fmt"
var _ = fmt.Println // TODO remove this

/*
   camelCase fields: private
   PascalCase: public
*/

type vectorlol struct {
    a []int
    next *vectorlol
}

type vectorlolIterator struct {
    p int
    vector *vectorlol
    current *vectorlol
    accumulated int
}

func NewVectorlol() *vectorlol {
    ret := new(vectorlol)
    ret.a = make([]int, 0, 2)
    return ret
}

func NewVectorlolCapacity(capacity int) *vectorlol {
    ret := new(vectorlol)
    ret.a = make([]int, 0, capacity)
    return ret
}

func (v *vectorlol) Get(i int) (ret interface{}) {
    if i >= cap(v.a) {
        if v.next == nil {
            v.next = new(vectorlol)
            v.next.a = make([]int, 0, cap(v.a) * 2)
        }
        ret = v.next.Get(i - cap(v.a))
        return
    } else if i >= len(v.a) {
        v.a = v.a[:i+1]
    }
    return v.a[i]
}

func (v *vectorlol) Set(i int, val interface{}) {
    if i >= cap(v.a) {
        if v.next == nil {
            v.next = new(vectorlol)
            v.next.a = make([]int, 0, cap(v.a) * 2)
        }
        v.next.Set(i - cap(v.a), val)
        return
    } else if i >= len(v.a) {
        v.a = v.a[:i+1]
    }
    v.a[i] = val.(int)
}

func (v *vectorlol) PushBack(val interface{}) {
    p := 0
    accumulated := 0
    ptr := v
    var iVal int = val.(int)
    for {
        if p - accumulated >= len(ptr.a) {
            if ptr.next != nil {
                accumulated += len(ptr.a)
                ptr = ptr.next
            } else {
                ptr.Set(p - accumulated, iVal)
                break
            }
        } else {
            p+= len(ptr.a)
        }
    }
}

func (v *vectorlol) PopBack() (ret interface{}) {
    p := 0
    accumulated := 0
    ptr := v
    var prev *vectorlol
    for {
        if p - accumulated >= len(ptr.a) {
            if ptr.next != nil {
                accumulated += len(ptr.a)
                prev = ptr
                ptr = ptr.next
            } else if len(ptr.a) <= 1 {
                ret = ptr.a[0]
                prev.next = nil
                return
            } else {
                ret = ptr.a[len(ptr.a) - 1]
                ptr.a = ptr.a[:len(ptr.a) - 1]
                return
            }
        } else {
            p+= len(ptr.a)
        }
    }
}

func (v *vectorlol) Iterator() Iterator {
    i := new(vectorlolIterator)
    i.vector = v
    i.p = 0
    i.accumulated = 0
    i.current = v
    return i
}

func (i *vectorlolIterator) Key() interface{} {
    return i.p
}

func (i *vectorlolIterator) Value() interface{} {
    return i.vector.Get(i.p)
}

func (i *vectorlolIterator) Next() bool {
    if !i.More() { return false }
    i.p++
    if i.p - i.accumulated >= len(i.current.a) {
        if i.current != nil {
            i.accumulated += len(i.current.a)
        }
        i.current = i.current.next
    }
    return true
}

func (i *vectorlolIterator) More() bool {
    if i.current == nil { return false }
    return i.p - i.accumulated < len(i.current.a)
}
