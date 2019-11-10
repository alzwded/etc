#!/bin/tcsh

alias quote 'echo \"\!*:q\"'
echo cmd `quote param\ 1` `quote 'param 2'`
set x = aa
echo `quote $x`
set x = (aa bb cc)
echo `quote $x`

alias quote 'echo \"\!*\"'
echo cmd `quote param\ 1` `quote 'param 2'`
set x = aa
echo `quote $x`
set x = (aa bb cc)
echo `quote $x`
