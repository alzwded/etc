#!/bin/bash

if [[ `id -u` -gt 0 ]] ; then
    echo please run as root
    exit 2
fi

# technically I don't know what happens if the seat is connected
export DISPLAY=:0


Xorg :0 &
PID=$!

USER=root
if [[ -n ${SUDO_USER:+x} ]] ; then
    USER=$SUDO_USER
fi
echo User is $USER
UHOME=`eval echo ~$USER`
echo UHOME is $UHOME

sleep 3

# port forwarding batch file windows
# REM ssh -t -L 5900:localhost:5900 192.168.0.42 "/bin/sh -c 'x11vnc -ncache 10 -localhost -display :0 `sudo /usr/local/bin/pathToXAuthority`'"
# ssh -t -L 5900:localhost:5900 192.168.0.42 "/bin/sh -c 'x11vnc -ncache 10 -localhost -display :0 '"
# pause

echo VNX tip: CTRL+ALT+SHIFT+F toggles fullscreen and captures keyboard
echo Openbox tip: C-A-t, A-F1, A-F3
echo connect via TightVNC, then press enter
# otherwise xrandr and xinitrc won't stick
read

#xrandr --fb 1733x1024
xrandr --fb 1920x1080

# this bit doesn't seem to work right
if [[ -x "$UHOME/.xinitrc" ]] ; then
    # run stuff like setxkb, xsetroot, LC_CTYPE, openbox
    sudo -u $USER $UHOME/.xinitrc
else
    sudo -u $USER xterm
fi


kill -TERM $PID
