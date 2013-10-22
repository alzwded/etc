#!/bin/csh
set CAT=`which xxd >>& /dev/null && echo xxd || echo cat`

./jakcode -ftest.in,test.out
./jakcode -Ftest.out,test.reout
$CAT test.in
$CAT test.reout
diff -u test.in test.reout
echo --------------------
if($status != 0) then
    echo test failed
else
    echo test successed
endif
echo --------------------
./jakcode -ftest2.in,test2.out
./jakcode -Ftest2.out,test2.reout
$CAT test2.in
$CAT test2.reout
diff -u test2.in test2.reout
echo --------------------
if($status != 0) then
    echo test failed
else
    echo test successed
endif
echo --------------------
./jakcode -ftest3.in,test3.out
./jakcode -Ftest3.out,test3.reout
$CAT test3.in
$CAT test3.reout
diff -u test3.in test3.reout
echo --------------------
if($status != 0) then
    echo test failed
else
    echo test successed
endif
echo --------------------
./jakcode -ftest2.reout,test2.out
./jakcode -Ftest2.out,test2.reout
./jakcode -ftest2.reout,test2.out
./jakcode -Ftest2.out,test2.reout
$CAT test2.in
$CAT test2.reout
diff -u test2.in test2.reout
echo --------------------
if($status != 0) then
    echo test failed
else
    echo test successed
endif
echo --------------------
