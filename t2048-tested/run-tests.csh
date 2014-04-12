#!/bin/csh

set tests=`./test.out -l`

foreach test ($tests)
    ./test.out $test || echo "$test FAILED"
end
