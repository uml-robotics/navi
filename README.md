# About Navi
[Navi](http://zelda.wikia.com/wiki/Navi) is an arduino based safety light and buzzer for use on [NASA's Valkyrie robot](http://www.nasa.gov/feature/nasa-looks-to-university-robotics-groups-to-advance-latest-humanoid-robot).  It monitors the high voltage state published by smtcore on ```/power/pdbmti/HV_State``` and turns on an emergency vehicle light as well as plays an audible alarm
during the transition from high voltage being off and high voltage being on.  Navi is intended to stay connected to the operator computer at all times but has a manual override switch to turn on the light in the case of a failure (the switch with a power indicator LED).  It also has a mode switch to change the blink pattern of the light.

# Installation
 * Make sure you are installing this on a working operator station where you can run smttopic.
 * Plug Navi's usb serial port into one of the usb ports on the back of your computer.
 * Plug a 12V 1A barrel connector power supply to the plug on Navi to power the light.
 * ```sudo git clone https://github.com/uml-robotics/navi.git /usr/local/bin/navi```
 *  If the arduino changed edit the navi file and change the line ```serial.Serial("/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AI03RLDW-if00-port0", 9600)```
 to whatever the current device id is.   Also change the line ```SERIAL_DEVICE=/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AI03RLDW-if00-port0``` in the navi.init.d file
 * If you username is not odin change the line ```DAEMON_USER=odin``` in navi.init.d
 * Add ```/usr/local/bin/navi/scripts``` to your path in your bashrc
 * ```sudo cp /usr/local/bin/navi/navi.init.d /etc/init.d/navi```
 * ```sudo update-rc.d navi defaults```
 * ```sudo service navi start```
 * Check that navi started: ```sudo service navi status```


# Usage
The navi daemon should automatically start on system boot and turn on and off the light automatically.  If you want to manually change the state
until the next power state update or debug Navi you can run ```navi {on|off}``` from your terminal.  This can be handy to find out if there
actually is a serial connection to Navi.

Upon detecting a power state change to having motor power enabled Navi will go through a 5 second long beep and turn on the relay/ light on.
Upon detecting that motor power is off it will do three short beeps and shut the light off.

# How to Build a New One
The arduino sketch can be found in ```/usr/local/bin/navi/arduino```.

The circuit is extreamely simply and is just an arduino nano attached to a relay whose signal lines are attached to pin 3 and ground, and a peizo buzzer with an internal oscillator attached to pin 2.  The circuit diagram can also be found in the arduino folder.