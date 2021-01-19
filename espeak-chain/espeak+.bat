@echo off
SETLOCAL

SET "ESPEAK_BIN=c:\bin\eSpeak\command_line\espeak.exe"
SET "FFMPEG_BIN=ffmpeg"

REM espeak+.bat
REM this is very very old and doesn't work very well at all
SET NOWAIT=0

:START
IF "%~1"=="/nw" (
    SET NOWAIT=1
    SHIFT
)
IF "%~1"=="" GOTO NOPARAMS
IF "%~1"=="/?" GOTO HELP
IF "%~1"=="-?" GOTO HELP
IF "%~1"=="/h" GOTO HELP
IF "%~1"=="-h" GOTO HELP
IF "%~1"=="/o" GOTO EXTRAPARAS
IF "%~1"=="/s" GOTO SPLITPARAS
IF "%~1"=="/v" GOTO VERBOSE
GOTO PARSEFILE

:VERBOSE
ECHO ON
SHIFT
GOTO START

:NOPARAMS
SET /P INPUTTED=Input the file name of the file you wish spoken: 
SET /P EXTRAPARAS=Type in the extra parameters:
GOTO RUNESPEAK

:SPLITPARAS
SET EXTRAPARAS=--split="60"
SHIFT
GOTO START

:EXTRAPARAS
SET /P EXTRAPARAS=Type in the extra parameters:
SHIFT
GOTO START

:PARSEFILE
SET INPUTTED=%~1
SET WAVFILE=%~dpn1.wav
SET ENCODEDFILE=%~dpn1.mp3

:RUNESPEAK
SET COMMANDLINESTRING=%ESPEAK_BIN% -s 264 -p 90 -v english-north -l 50 -b 1 -f "%INPUTTED%" -w "%WAVFILE%"
REM --split="60"
IF "%EXTRAPARAS%"=="" GOTO NOEXTRAPARAS
GOTO EXTRAPARAS

REM -l 20 lines less than 20 will be full stop
REM -f read from file
REM -w output to wave file
REM -b 

:NOEXTRAPARAS
%COMMANDLINESTRING%
GOTO CHECKFORERRORS

:EXTRAPARAS
%COMMANDLINESTRING% %EXTRAPARAS%
GOTO CHECKFORERRORS


:CHECKFORERRORS
IF %ERRORLEVEL%==0 GOTO SUCCESS

:FAIL
ECHO conversion failed, error code %ERRORLEVEL%
ECHO 255 - INTERNAL_ERROR, 1 - BUFFER_FULL, 2 - NOT_FOUND
GOTO END

:SUCCESS
ECHO conversion finished.
where %FFMPEG_BIN%>NUL 2>&1
IF ERRORLEVEL 1 GOTO END
ECHO converting to mp3.
ffmpeg -i "%WAVFILE%" -acodec libmp3lame -ab 48000 "%ENCODEDFILE%"
IF ERRORLEVEL 1 GOTO :FIN
DEL /q "%WAVFILE%"
:FIN
echo %ERRORLEVEL%
GOTO END

:HELP
ECHO usage: espeak+  [switches] filename
ECHO.     /nw - don't pause at the end
ECHO.     /? - this message
ECHO      /o - for extra options
ECHO      /s - tell espeak to split the file
ECHO      /v - verbose (?) mode
ECHO      the switches cannot be used all at once
GOTO END

:END
REM CLEAN STUFF
SET EXTRAPARAS=
SET INPUTTED=
IF %NOWAIT% GTR 0 GOTO :EOF
PAUSE
