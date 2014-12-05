@echo off

set JAKMARKPATH=C:\Projects\JakMark\JakMark\bin\Release\JakMark.exe
set WKHTMLTOPDFPATH=C:\Program Files\wkhtmltopdf\bin\wkhtmltopdf.exe

echo Building DOC.html
"%JAKMARKPATH%" --toc DOC.jm DOC.html

echo Building DOC.pdf
"%WKHTMLTOPDFPATH%" --header-line --header-left [title] --header-right [page]/[toPage] -T 2cm  --enable-javascript --disable-smart-shrinking --zoom 1.5 DOC.html DOC.pdf
