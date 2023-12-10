#! /bin/env python3

import serial,time,os


if __name__ == '__main__':
    portScreen=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
    while len(portScreen)<11:
        portScreen=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
        print('\033[2;41;41m Trying to connect to Arduino Please check the port')

    print("The port of Arduino is " +portScreen)
    os.system('sudo chmod 777 '+portScreen)
    with serial.Serial(portScreen, 9600, timeout=3,bytesize=serial.EIGHTBITS) as arduino:
        time.sleep(0.1) #wait for serial to open
        if arduino.isOpen():
            print("{} connected!".format(arduino.port))
            try:
                while True:
                    arduino.flush()
                    cmd=input("Enter command : ")
                    cmd=cmd.rstrip()
                    arduino.write(cmd[0].encode("UTF-8"))
            except KeyboardInterrupt:
                print("KeyboardInterrupt has been caught.")
