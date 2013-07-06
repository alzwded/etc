#!/bin/csh

#set echo

set outdir=/media/Data/bach
set rarexe=unrar

echo "output will be sent to $outdir"
echo

foreach i (*.rar)
    echo "processing $i:q"
    set dir=`mktemp -d -p /tmp bachXXXXXXX`
    echo ". temp will be $dir:q"
    echo ". copying rar file"
    cp $i:q $dir:q
    if( { csh -c "cd '$dir' && $rarexe:q x '$dir/$i:q'" } ) then
        echo ". removing rar file from temp dir"
        rm -f $dir:q/$i:q
        #echo ". removing rar file source"
        #rm -f $i:q
        echo ". renaming files"
        csh rename.csh $dir:q
        echo ". moving files to outdir"
        mv -v $dir:q/* $outdir:q
        if($status == 0) then
            echo ". removing temp dir"
            rm -rf $dir:q
        else
            echo ".. failed"
        endif
    else
        echo ". failed"
    endif
end

echo "renaming output folders"
foreach i ($outdir:q/Bach\ 2000\ *)
    echo ". $i:q"
    mv $i:q $i:ags/Bach 2000 //:q
end
