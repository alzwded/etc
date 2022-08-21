#!/bin/csh

cat <<EOF
    (quotations.backticks)
 1. You must be in QUOTE (") (1.0)
 2. Enter backtick (1.1)
 3. Escape first QUOTE (0.1)
 4. Backslash quote backtick (0.2)
 5. Re-enter first QUOTE (1.2)
 6. Exit again (0.2)
 7. Backslash quoted closing backtick (0.1)
 8. Go back to first quote (1.1)
 9. Exit backtick (1.0)
10. End QUOTE (0.0)
EOF

cat - <<EOF
Will run echo "\`echo "\\\`"echo hi"\\\`"\`"
EOF

set echo
echo "`echo "\`"echo hi"\`"`"
