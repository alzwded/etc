#!/usr/bin/tcsh

groff -Tpdf -mman -fC $1:q > $1:r:q.pdf
groff -Txhtml -mman $1:q > $1:r:q.xhtml
