#!/bin/sh

### BEGIN INIT INFO
# Provides: MOTAB Demo
### END INIT INFO

APP=/usr/bin/motabdemo

PIDFILE=/var/run/motabdemo.pid
LOGFILE=/var/log/motabdemo.log

start() {
    if [ -f /var/run/$PIDNAME ] && kill -0 $(cat /var/run/$PIDNAME); then
        echo 'MOTAB Demo already running' >&2
        return 1
    fi
    echo 'Starting MOTAB Demo ..' >&2

#    if test -z "$XDG_RUNTIME_DIR"; then
#        export XDG_RUNTIME_DIR=/tmp/`id -u`-runtime-dir
#        if ! test -d "$XDG_RUNTIME_DIR"; then
#            mkdir --parents $XDG_RUNTIME_DIR
#            chmod 0700 $XDG_RUNTIME_DIR
#        fi
#    fi

    # wait for weston
#    while [ ! -e  $XDG_RUNTIME_DIR/wayland-0 ] ; do sleep 0.1; done
#    export DISPLAY=:0.0

#    local CMD="$APP &> $LOGFILE & echo \$!"
#    eval $CMD > $PIDFILE
    motabdemo -f 1
    echo 'MOTAB Demo started' >&2
}

stop() {
    if [ ! -f "$PIDFILE" ] || ! kill -0 $(cat "$PIDFILE"); then
        echo 'MOTAB Demo not running anyway' >&2
        return 1
    fi
    echo 'Stopping MOTAB Demo ..' >&2
    kill -15 $(cat "$PIDFILE") && rm -f "$PIDFILE"
    echo 'MOTAB Demo stopped' >&2
}

case "$1" in
    start )
        start
    ;;
    stop )
        stop
    ;;
    restart )
        stop
        start
    ;;
    * )
        echo "Usage: $0 {start|stop|restart}"
esac

