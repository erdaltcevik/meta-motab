#! /bin/sh

#!/bin/sh 
### BEGIN INIT INFO
# Provides:             kmscube demo
# Required-Start:
# Required-Stop:
# Default-Start:        2 3 4 5
# Default-Stop:
### END INIT INFO



DAEMON=/usr/bin/kmscube
PIDFILE=/var/run/kmscube.pid


if [ ! -d /sys/class/drm/card1 ]; then
    echo "gpu not detected"
    echo "kmscube will not run"
    exit 0;
fi


case "$1" in
  start)
    echo "Now starting kmscube..."
    start-stop-daemon --start --oknodo --pidfile $PIDFILE --startas $DAEMON
  	;;
  stop)
  	
	  ;;
  force-reload)

	  ;;
  restart)

	  ;;
  reload)

	  ;;
  status)

	  ;;
  *)
  	exit 1
	  ;;
esac

exit 0
