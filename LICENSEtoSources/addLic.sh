#!/bin/sh

tmp=`mktemp -p /tmp jakXXXXXX`
cat LICENSE "$1" > "$tmp"
sed -i "$tmp" -f addQuotes.sed
cat "$tmp" > "$1"
rm "$tmp"
