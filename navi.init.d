#!/bin/bash

### BEGIN INIT INFO
# Provides:          navi
# Required-Start:    $remote_fs $syslog $network
# Required-Stop:     $remote_fs $syslog $network
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Navi the annoying blinkin light
# Description:       Navi the safety light driver that automatically runs the light and buzzer when valkyrie has motor power on
### END INIT INFO

# Change the next 3 lines to suit where you install your script and what you want to call it
DIR=/usr/local/bin/navi
DAEMON=$DIR/navi
DAEMON_NAME=navi

# serial device that maps to the arduino
SERIAL_DEVICE=/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AI03RLDW-if00-port0

# This next line determines what user the script runs as.
DAEMON_USER=odin

# Add any command line options for your daemon here
DAEMON_OPTS="--user $DAEMON_USER --serial_dev $SERIAL_DEVICE"

# The process ID of the script when it runs is stored here:
PIDFILE=/var/run/$DAEMON_NAME.pid


. /lib/lsb/init-functions

do_start () {
    log_daemon_msg "Starting $DAEMON_NAME daemon"
#     stty -F $SERIAL_DEVICE 9600
    start-stop-daemon --start --background --pidfile $PIDFILE --make-pidfile --user $DAEMON_USER --chuid $DAEMON_USER --startas $DAEMON -- $DAEMON_OPTS
    log_end_msg $?
}
do_stop () {
    log_daemon_msg "Stopping $DAEMON_NAME daemon"
    start-stop-daemon --stop --pidfile $PIDFILE --retry 10
    log_end_msg $?
}

case "$1" in

    start|stop)
        do_${1}
        ;;

    restart|reload|force-reload)
        do_stop
        do_start
        ;;

    status)
        status_of_proc "$DAEMON_NAME" "$DAEMON" && exit 0 || exit $?
        ;;

    *)
        echo "Usage: /etc/init.d/$DAEMON_NAME {start|stop|restart|status}"
        exit 1
        ;;

esac
exit 0
