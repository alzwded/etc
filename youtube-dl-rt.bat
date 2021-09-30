@echo off
setlocal ENABLEDELAYEDEXPANSION

for /F "usebackq" %%i in ("%~1") do (
    ECHO Handling URL %%i
    CALL :JOB "%%i"
)
GOTO :EOF

:JOB
    SET "fmt="
    ECHO getting formats
    for /f "usebackq delims=" %%i in (`youtube-dl -F %1`) do (
        ECHO investigating line "%%i"
        for /f "tokens=1,3" %%j in ("%%i") do (
            if [%%k] == [960x540] (
                if [!fmt!] == [] (
                    ECHO using %%j+hls-audio-1-en__Main_
                    SET "fmt=%%j"
                )
            )
        )
    )
    ECHO downloading
    youtube-dl -t -c -f %fmt%+hls-audio-1-en__Main_ %1
    GOTO :EOF
