Crop pages in a PDF
===================

Required tools:

* `sejda-console` aka "pdfsam", the open source command line tool
* imagemagick

## 1. Extract TIFFs

```winbatch
sejda-console pdftomultipletiff -f book.pdf -o %imgDir% -s %dpi%
```

## 2. ImageMagick crop & convert to png

Apparently for black text on a white background, png has a better compression ratio. JPG wins on "random" images, though. For the best compression ratio, convert text pages to png and image pages to jpg.

Prefer to read files from a file list

```winbatch
REM file is in this format: 
REM     source.tif,WIDTHxHEIGHT+LEFT+TOP,destination.png
REM     source.tif,WIDTHxHEIGHT+LEFT+TOP,destination.jpg
REM     147_book.tif,640x900+20+30,147_book.png
REM     147_book.tif,640x900+20+30,147_book.jpg
for /F %i "delims=," in (conversion.csv) do @magick convert "%i" -crop %j "%k"
```

## 3. Create a million PDFs

```winbatch
for /F %i "tokens=3 delims=," in (conversion.csv) do @magic convert "%i" +repage "%~ni.pdf"
```

## 4. Bring everything back together

```winbatch
echo.>filelist.csv
for /F %i "tokens=3 delims=," in (conversion.csv) do echo %~ni.pdf>>filelist.csv
sejda-console merge -l filelist.csv -o book-cropped.pdf
```
