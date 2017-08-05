#!/bin/csh

# did some convert -monitor a.png -crop WxH+x+y b.png and convert -monitor a.png -bordercolor white -border 50 b.png before this

#convert -monitor `cat ukulele.lst` `\ls -b -q bordered/*.png` -quality 100 -units PixelsPerInch -density 72x72 -page A4 book.pdf

# this one's ok, but I want more resolution
#convert -monitor -page A4 -quality 100 -units PixelsPerInch -density 110 `cat ukulele.lst` `\ls -b -q bordered/*.png` book.pdf

#convert -debug all -monitor -limit memory 2gb -limit disk 1500mb -page A4 -quality 100 -units PixelsPerInch -density 144 `cat ukulele.lst` `\ls -b -q bordered/*.png` book.pdf
#convert -debug all -monitor -limit disk 1600mb -page A4 -quality 100 -units PixelsPerInch -density 134 `cat ukulele.lst` `\ls -b -q bordered/*.png` book.pdf

mkdir -p pdf
foreach i (bordered/*.png)
    echo $i to pdf/$i:t:r.pdf
    convert -monitor -limit memory 1000MB -limit area 256MP -limit map 1000MB -limit width 32KP -limit height 32KP -limit disk 1500MB -quality 100 -units PixelsPerCentimeter \
        -density 200 \
        $i \
        -filter LanczosSharp -resize 4200x5940^! -repage 0x0 \
        pdf/$i:t:r.pdf || exit 2
    echo sleeping to not keep CPU at 100% too much...
    sleep 2 # cpu stays too much at 100%...
end

echo cat-ing
pdftk `cat ukulele.lst` `\ls -b -q pdf/*.pdf` cat output book.pdf
