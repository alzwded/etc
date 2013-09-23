#!/bin/csh

set hup
onintr finish

while(1 == 1)
    clear
    ls -sh
    df -h .
    sleep 2
end

finish:
echo
exit 0
