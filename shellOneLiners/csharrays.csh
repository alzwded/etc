#!/usr/bin/tcsh

# experimenting with what you can do with arrays in tcsh

set y='b c d'

set z=(e f 'g h')

set x=(a $y:q $z)

echo $x:q
echo $#x elements
set idx=1
foreach i ($x:q)
    echo $idx':' $i:q
    set element=($i:x)
    if($#element > 1) then
        set idx2=1
        foreach j ($element:q)
            echo $idx'.'$idx2':' $j
            @ idx2 ++
        end
    endif
    @ idx ++
end

echo
echo array of pointers derefed as arrays
echo "set a=(y z)"
echo "eval set element='(''"'$'"'"'$'"i')'"
set a=(y z)
foreach i ($a)
    eval set element='(''$'$i')'
    echo $i '= (' $element:q ')'
    foreach j ($element:q)
        echo "   " $j
    end
end
echo array of pointers derefed as quoted strings
echo "set a=(y z)"
echo "eval set element='"'"'"''"'$'"'"'$'"i'"'"'"'" # crazy quoting! W/O!
foreach i ($a)
    eval set element='"''$'$i'"'
    echo $i '= "'$element:q'"'
    foreach j ($element:q)
        echo "   " $j
    end
end
