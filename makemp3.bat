@ECHO OFF
SETLOCAL ENABLEEXTENSIONS

REM this batch is more complicated than it needs to be
REM     because you apparently can't GOTO inside a ()
REM     block; so I need to CALL to a subroutine, get rid
REM     of quotes, set variables to all the path expansions
REM     I need and only then have some logic

IF EXIST mp3 GOTO :MAINLOOP
MD mp3

:MAINLOOP
FOR %%i IN (*.m4a) DO (
    CALL :MAKEMP3PAYLOAD "%%i"
)

ENDLOCAL
GOTO :EOF

:MAKEMP3PAYLOAD
    SETLOCAL ENABLEEXTENSIONS
    SET k=%~1
    FOR /F "delims=" %%i IN ("%k%") DO (
        SET infile="%%i"
        SET outfile="mp3\%%~ni.mp3"
        SET guardfile="mp3\%%~ni.mp3.inprogress"
    )

    ECHO %infile% to %outfile%
    IF EXIST %outfile% GOTO :MAYBE
    GOTO :CONVERT
:MAYBE
    IF EXIST %guardfile% GOTO :CONVERT2
    GOTO :SKIP
:CONVERT
    ECHO 1 > %guardfile%
:CONVERT2
    avconv -i %infile% -y -ab 128000 %outfile%
    DEL /Q %guardfile%
:SKIP
    ECHO done.
    ENDLOCAL
    GOTO :EOF
