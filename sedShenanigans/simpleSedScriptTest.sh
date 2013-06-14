#!/bin/sh
# ./simpleSedScriptTest.sh

cat "$0"
echo '<end of '$0'>'
cat <<EOT | sed -f simpleSedScript.sed
qwe
ala
ala
ala
qwe
bala
bala
qwe
EOT
