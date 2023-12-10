#!/usr/bin/env python3
import rospy
from std_msgs.msg import Int32
from std_msgs.msg import Int16MultiArray
import serial,time,os
import alfons_msgs 
from alfons_msgs.msg import ArucoInfo

"""portName=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
while len(portName)<11:
    portScreen=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
    print('\033[2;41;41m Trying to connect to Arduino Please check the port')

print("The port of Arduino is " +portName)
os.system('sudo chmod 777 '+portName)
"""
score = 0 

def panierCallback(msg): 
    if msg.data==1:
    	sendArduino('e') #Command the basket's arduino to elevate
    	print("Start Elevation")


def arucoCallback(msg):
    global x
    global y
    global ids
    x=msg.center_x_px
    y=msg.center_y_px
    ids= msg.marker_ids
    
    for i in ids :
        index=ids.index(i)
        if(int(i/10)==47 ):
            if(0<x[index]<800 and 1100<y[index]<1650):
                Gobst(-750,700,500)
            if(0<x[index]<900 and 250<y[index]<750):
                Gobst(-750,1500,500)
            if(2600<x[index]<3250 and 250<y[index]<750):
                Gobst(750,1500,500)
            if(2800<x[index]<3800 and 1100<y[index]<1650):
                Gobst(750,700,500)
    print("x : " , x , "|    y : " , y , "|     id :  " , ids ) 


def sendArduino(seq):
        try:
        #    x=input("aaaaaaaaaaa")
           arduino.flush()
           arduino.write(seq.encode("UTF-8"))
        #    arduino.write(x.encode("UTF-8"))
        except KeyboardInterrupt:
            print("KeyboardInterrupt has been caught.")   

def listener():
    rospy.init_node('paniereNode', anonymous=True)
    print("waiting for panier elevation ...")
    panierSub=rospy.Subscriber("/panier", Int32, panierCallback, queue_size=10)
    arucoSub=rospy.Subscriber("/camera/image_raw/aruco_list",ArucoInfo ,arucoCallback,queue_size=100)
    rospy.spin()
    


if __name__ == '__main__':
    listener()
