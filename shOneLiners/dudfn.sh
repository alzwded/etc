#!/bin/sh
echo num:"	"`find . | wc -l` ; echo size:"	"`du -sm | cut -f 1`M ; df -h .
