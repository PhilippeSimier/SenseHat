#!/bin/bash

## Fill in name of program here.
PROG="measureToDB"
PROG_PATH="/home/jdellaria/Desktop/SenseHat/Projects/MeasurementDatabase" ## Not need, but sometimes helpful (if $PROG resides in /opt for example).
PROG_ARGS="10000000" 
PID_PATH="/var/run/"

start() {
# Check if gedit is running
if pgrep $PROG > /dev/null
then
	echo "$PROG is already Running"
else
	echo "$PROG is starting"
	cd $PROG_PATH
##	$PROG_PATH/startMusicServer.sh
	nohup $PROG_PATH/measureToDB &
##	sleep 5
## 	$PROG_PATH/playauto.sh
## 	echo "$PROG started with autoplay"
fi
}

stop() {
	cd $PROG_PATH
	$PROG_PATH/exit.sh
        echo "$PROG stopped"
}


check() {
# Check if gedit is running
if pgrep $PROG > /dev/null
then
    echo "$PROG Running"
else
    echo "$PROG Stopped"
fi
}

## Check to see if we are running as root first.
## Found at http://www.cyberciti.biz/tips/shell-root-user-check-script.html
## if [ "$(id -u)" != "0" ]; then
##     echo "This script must be run as root" 1>&2
##     exit 1
## fi

case "$1" in
    start)
        start
        exit 0
    ;;
    stop)
        stop
        exit 0
    ;;
    check)
        check
        exit 0
    ;;
    reload|restart|force-reload)
        stop
        start
        exit 0
    ;;
    **)
        echo "Usage: $0 {start|stop|reload}" 1>&2
        exit 1
    ;;
esac
