#!/bin/bash
#
#example miniterm.py --parity N -e --rts 0 --dtr 0 /dev/ttyUSB0 115200 
BAUD=$1
if [ -z "$1" ]
then
    echo expected baud rate
    exit 1
fi
BAUD=$(echo $1 | sed -e "s/UL//")
PORT=$2
if [ -z "$PORT" ]
then
    echo expected port
    exit 1
fi



echo python3  -m serial.tools.miniterm --parity N --rts 0 --dtr 0 $(PORT) $(BAUD)
python3  -m serial.tools.miniterm --parity N --rts 0 --dtr 0 $(PORT) $(BAUD)

#usage: miniterm.py [-h] [--parity {N,E,O,S,M}] [--rtscts] [--xonxoff]
#                   [--rts RTS] [--dtr DTR] [--ask] [-e] [--encoding CODEC]
#                   [-f NAME] [--eol {CR,LF,CRLF}] [--raw] [--exit-char NUM]
#                   [--menu-char NUM] [-q] [--develop]
#                   [port] [baudrate]
#
#Miniterm - A simple terminal program for the serial port.
#
#positional arguments:
#  port                  serial port name ('-' to show port list)
#  baudrate              set baud rate, default: 9600
#
#optional arguments:
#  -h, --help            show this help message and exit
#
#port settings:
#  --parity {N,E,O,S,M}  set parity, one of {N E O S M}, default: N
#  --rtscts              enable RTS/CTS flow control (default off)
#  --xonxoff             enable software flow control (default off)
#  --rts RTS             set initial RTS line state (possible values: 0, 1)
#  --dtr DTR             set initial DTR line state (possible values: 0, 1)
#  --ask                 ask again for port when open fails
#
#data handling:
#  -e, --echo            enable local echo (default off)
#  --encoding CODEC      set the encoding for the serial port (e.g. hexlify,
#                        Latin1, UTF-8), default: UTF-8
#  -f NAME, --filter NAME
#                        add text transformation
#  --eol {CR,LF,CRLF}    end of line mode
#  --raw                 Do no apply any encodings/transformations
#
#hotkeys:
#  --exit-char NUM       Unicode of special character that is used to exit the
#                        application, default: 29
#  --menu-char NUM       Unicode code of special character that is used to
#                        control miniterm (menu), default: 20
#
#diagnostics:
#  -q, --quiet           suppress non-error messages
#  --develop             show Python traceback on error
#
