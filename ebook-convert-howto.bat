@echo off

set CALIBREPATH=C:\Program Files\Calibre2
set CONV="%CALIBREPATH%\ebook-convert.exe"
set DEBUG=0
IF %DEBUG% GTR 0 ECHO ON

for /r %%i in (*.lit) do (
    echo Processing %%i
    cmd /b /s /c "%CONV% "%%i" "%%~dpi\%%~ni.epub" --margin-left=0.1 --margin-top=0.1 --margin-right=0.1 --margin-bottom=1"
    IF %DEBUG% GTR 0 (
        timeout /t 30 /nobreak
        if errorlevel 1 goto :EOF
    )
)
