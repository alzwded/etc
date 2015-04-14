#!/bin/sh

ls "$@" | sed -e 's/.*\(\.[^.]*\)/' | sort | uniq
