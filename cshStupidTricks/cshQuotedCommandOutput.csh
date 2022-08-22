#!/usr/bin/tcsh

[ -e 'a' ] && echo 'test precondition failed... please "rm a"' && exit 2

# create a file with spaces in its name
touch 'a a'

# a -> unquoted
# b -> :q quoted
# c -> " quoted

# 1a multiword output of command, unquoted
set a=`echo 'a a'`
echo 'set a=`echo '\''a a'\''` ; stat $a fails'
stat -c '%a' $a >& /dev/null && echo ok || echo failed
# 1b multiword output of command, :q quoted
set a=`echo 'a a'`
echo 'set a=`echo '\''a a'\''` ; stat $a:q fails'
stat -c '%a' $a:q >& /dev/null && echo ok || echo failed
# 1c multiword output of command, " quoted
set a=`echo 'a a'`
echo 'set a=`echo '\''a a'\''` ; stat "$a" ok'
stat -c '%a' "$a" >& /dev/null && echo ok || echo failed

echo ''

# 2a multiword output of command, " quoted into variable, unquoted
set a=`echo 'a a'`
set a="$a"
echo 'set a=`echo '\''a a'\'' ; set a="$a" ; stat $a fails'
stat -c '%a' $a >& /dev/null && echo ok || echo failed
# 2b multiword output of command, " quoted into variable, then :q quoted
set a=`echo 'a a'`
set a="$a"
echo 'set a=`echo '\''a a'\'' ; set a="$a" ; stat $a:q ok'
stat -c '%a' $a:q >& /dev/null && echo ok || echo failed
# 2c multiword output of command, " quoted into variable, then " quoted
set a=`echo 'a a'`
set a="$a"
echo 'set a=`echo '\''a a'\'' ; set a="$a" ; stat "$a" ok'
stat -c '%a' "$a" >& /dev/null && echo ok || echo failed

echo ''

# 3a multiword, unquoted
set a="a a"
echo 'set a="a a" ; stat $a fails'
stat -c '%a' $a >& /dev/null && echo ok || echo failed
# 3b multiword, :q quoted... ok?
set a="a a"
echo 'set a="a a" ; stat $a:q ok?'
stat -c '%a' $a:q >& /dev/null && echo ok || echo failed
# what?
# 3c multiword, " quoted
set a="a a"
echo 'set a="a a" ; stat "$a" ok'
stat -c '%a' "$a" >& /dev/null && echo ok || echo failed

echo ''

# 4a multiword array, unquoted
set a=(a a)
echo 'set a=(a a) ; stat $a fails'
stat -c '%a' $a >& /dev/null && echo ok || echo failed
# 4b multiword, :q quoted... ok?
set a=(a a)
echo 'set a=(a a) ; stat $a:q fails'
stat -c '%a' $a:q >& /dev/null && echo ok || echo failed
# what?
# 4c multiword, " quoted
set a=(a a)
echo 'set a=(a a) ; stat "$a" ok'
stat -c '%a' "$a" >& /dev/null && echo ok || echo failed

echo ''

# 5a quoted multiword output of command, unquoted
set a="`echo 'a a'`"
echo 'set a="`echo '\''a a'\''`" ; stat $a fails'
stat -c '%a' $a >& /dev/null && echo ok || echo failed
# 5b quoted multiword output of command, :q quoted
set a="`echo 'a a'`"
echo 'set a="`echo '\''a a'\''`" ; stat $a:q ok'
stat -c '%a' $a:q >& /dev/null && echo ok || echo failed
# 5c quoted multiword output of command, " quoted
set a="`echo 'a a'`"
echo 'set a="`echo '\''a a'\''`" ; stat "$a" ok'
stat -c '%a' "$a" >& /dev/null && echo ok || echo failed

echo ''
echo 'So setting a variable to the output of a command is like setting an unquoted array?'
