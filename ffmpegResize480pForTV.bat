@echo on

GOTO :xvid

:h264
ffmpeg -i "%~1" -s hd480 -c:v libx264 -crf 23 -c:s mov_text -c:a copy -strict -2 "mp4\%~n1.mp4"
GOTO :EOF

:xvid
REM to extract to srt file
REM ffmpeg -i "%~1" -c:v none -c:s srt "mp4\%~n1.srt"
set "dest=%RANDOM%%RANDOM%%RANDOM%.ass"
ffmpeg -i "%~1" -c:v none -c:s ass "%dest%"
REM ffmpeg -i "%~1"  -vf subtitles=%dest%:force_style='Fontsize=24' -s hd480  -c:v mpeg4 -b:v 512000 -c:a libmp3lame -q:a 4  -c:s mov_text  "mp4\%~n1.mp4"
ffmpeg -i "%~1"  -vf "subtitles=%dest%" -s hd480  -c:v mpeg4 -b:v 1M -c:a libmp3lame -q:a 4  -c:s mov_text  "mp4\%~n1.mp4"
del "%dest%"

GOTO :EOF
