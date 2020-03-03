Crop pages in a PDF
===================

Required tools:

* `sejda-console` aka "pdfsam", the open source command line tool
* ImageMagick
* `cmd` if it's still around. Otherwise, good luck reading CSV!

## 1. Extract TIFFs

```winbatch
sejda-console pdftomultipletiff -f book.pdf -o %imgDir% -s %dpi%
```

## 2. ImageMagick crop & convert to png

Apparently for black text on a white background, png has a better compression ratio. JPG wins on "random" images, though. For the best compression ratio, convert text pages to png and image pages to jpg.

Prefer to read files from a file list, because pdfsam doesn't pad numbers with 0s, and it's easier to track which should be turned into png and which jpeg in a spreadsheet.

```winbatch
REM file is in this format: 
REM     source.tif,WIDTHxHEIGHT+LEFT+TOP,destination_extension
REM examples:
REM     147_book.tif,640x900+20+30,png
REM     147_book.tif,640x900+20+30,jpg
for /F %i "tokens=* delims=," in (conversion.csv) do @magick convert "%i" -crop %j "%~ni.%k"
```

`Vim`/`ed`/[`jaked` (😃)](https://github.com/alzwded/JakED) magic to group lines by their numeric prefix length and do a first run:

```ed
f conversion.csv
r!dir /b
g/^\d_/m$
g/^\d\d_/m$
g/^\d\d\d_/m$
# etc
%s/^.*$/\&,580x900+20+30,png/
w
# jaked only
! for /F %i "tokens=* delims=," in ($) do @magick convert "%i" -crop %j "%~ni.%k"
# vim only
! for /F \%i "tokens=* delims=," in (%) do @magick convert "\%i" -crop \%j "\%~ni.\%k"
q
```

## 3. Create a million PDFs

```winbatch
for /F %i "tokens=* delims=," in (conversion.csv) do @magic convert "%~ni.%k" +repage "%~ni.pdf"
```

## 4. Bring everything back together

```winbatch
echo.>filelist.csv
for /F %i "tokens=1 delims=," in (conversion.csv) do echo %~ni.pdf>>filelist.csv
sejda-console merge -l filelist.csv -o book-cropped.pdf
```
