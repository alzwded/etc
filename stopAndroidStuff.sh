#!/bin/sh
for d in ~/Projects/*/gradlew ; do
    $d --stop
done
~/android/sdk/platform-tools/adb kill-server
