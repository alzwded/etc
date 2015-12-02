<?php
// So I was lazy one day and didn't want to download the sqlite3
// command line client. So I wrote my own in php.
// In hindsight, I should have probably downloaded the client.
$stdin = fopen('php://stdin', 'r');
$dbh = new SQLite3(':memory:');
$dbName = ':memory:';
echo '> ';
while($l = fgets($stdin)) {
    $l = trim($l);
    $matches = Array();
    try {
        // using regexes, dreaming of a better, more perl, world
        if(preg_match("/^\.open\s*(.+)\s*$/", $l, $matches)) {
            $dbh = new SQLite3($matches[1]);
            $dbName = $matches[1];
            echo "\n> ";
            continue;
        } else if(preg_match("/^\s*\.tables\s*$/", $l)) {
            $l = "select name from sqlite_master where type like 'table' and name not like 'sqlite_%';";
        } else if(preg_match("/^\s*\.schema\s+(.+)\s*$/", $l, $matches)) {
            $l = "select sql from sqlite_master where name like '$matches[1]' or tbl_name like '$matches[1]';";
        } else if(preg_match("/^\s*\.close\s*$/", $l)) { // not sure if this one exists in sqlite3.exe
            $dbh = new SQLite3(":memory:");
            $dbName = ':memory:';
            echo "\n> ";
            continue;
        } else if(preg_match("/^\s*\.db\s*$/", $l)) { // nor this one
            echo "$dbName\n\n> ";
            continue;
        } else if(preg_match("/^\s*\.help\s*$/", $l)) {
            echo <<<EOF
sqlite3 php immitation. Author: Vlad Mesco, 2015
I release this beast into the public domain and disclaim all copyright.

.open <dbname>        open said database
.close                close current database
.db                   print the filename of the current database
.tables               list table names in the current database
.schema <tblname>     print the schema of the specified table
.dump                 dump database as sql
.help                 print this message
.quit                 exit

EOF;
            echo "\n> ";
            continue;
        } else if(preg_match("/^\s*\.quit\s*$/", $l)) {
            break;
        } else if(preg_match("/^\s*\.dump\s*$/", $l)) {
            // basically do a .tables
            // foreach table
            //     echo .schema table;
            //     execute select * from table;
            //     foreach record as k,v
            //         echo INSERT INTO table (k...) VALUES (v...);
            echo "not implemented.\n";
            echo "\n> ";
            continue;
        } else if(preg_match("/^\s*\./", $l)) {
            echo "unknown command.\n";
            echo "\n> ";
            continue;
        }
        $r = $dbh->query($l);
        $num = $dbh->changes();
        if($r !== false && $r->numColumns() > 0) {
            $num = 0;
            $nfirst = false;
            for($i = 0; $i < $r->numColumns(); ++$i) {
                if($nfirst) echo ',';
                else $nfirst = true;
                echo $r->columnName($i);
            }
            echo PHP_EOL;
            while($record = $r->fetchArray(SQLITE3_NUM)) {
                $nfirst = false;
                foreach($record as $k => $v) {
                    if($nfirst) echo ',';
                    else $nfirst = true;
                    echo "$v";
                }
                echo PHP_EOL;
                ++$num;
            }
        }
        if($r !== false) {
            echo "$num rows.\n\n> ";
        } else {
            echo "query failed: " . $dbh->lastErrorMsg();
            echo "\n\n> ";
        }
    } catch(Exception $ex) {
        var_dump($ex);
    }
}
?>
