#!/bin/sh

find . -maxdepth 2 \( -name '*.cpp' -o -name '*.h' -o -name '*.y' \) -exec ./addLic.sh '{}' ';'
