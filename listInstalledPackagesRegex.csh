#!/bin/csh

dpkg -l $*:q | grep '^ii'
