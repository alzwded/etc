#!/bin/csh
set me=`readlink -f $0:q`
source $me:h:q/die.csh

set msg="message"

true || die "This shouldn't happen ($msg)"
false || die "This should exit ($msg)"
true || die "This shouldn't be reached!"
