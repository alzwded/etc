@echo off
REM espeak+.bat
REM this is very very old and doesn't work very well at all

:START
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

:RUNESPEAK
SET COMMANDLINESTRING="c:\bin\eSpeak\command_line\espeak.exe" -l 20 -b 1 -f "%INPUTTED%" -w "%INPUTTED:~0,-3%wav"
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
GOTO END

:HELP
ECHO usage: espeak+  [switches] filename
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
PAUSE
