#!/bin/sh

sed -e 's/http/\nhttp/g' TOGET | sed -e '/^$/d' | xargs ./get.sh
