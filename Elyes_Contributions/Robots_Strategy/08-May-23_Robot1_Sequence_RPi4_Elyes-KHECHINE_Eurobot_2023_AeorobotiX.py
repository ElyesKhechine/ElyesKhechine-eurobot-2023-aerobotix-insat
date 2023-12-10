#!/usr/bin.env python3
import serial,time,os,math
import rospy
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
from std_msgs.msg import Bool
from std_msgs.msg import String
import re
from reel_euro2021.msg import c
from reel_euro2021.srv import jdRequest
from reel_euro2021.srv import jd
from datetime import datetime
import numpy as np
still = True
currentSupport = 0
tabdila = False
portName=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
while len(portName)<11:
    portScreen=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
    print('\033[2;41;41m Trying to connect to Arduino Please check the port')

print("The port of Arduino is " +portName)
os.system('sudo chmod 777 '+portName)

print("initializing")
i=0
couleur=-1
if couleur==1:
    couleur_angle=1
else:
    couleur_angle=0
def sending_callback(msg):
    print("dkhalet fel callback")

rospy.init_node('SequenceKbir')
rospy.wait_for_service('/StmCommand')
#rospy.wait_for_service('/StmPose')
stmService = rospy.ServiceProxy('/StmCommand',jd)
sending_sub=rospy.Subscriber("/sending",Bool,sending_callback,queue_size=5)
time.sleep(0.5)
def xy(x,y,phi):
    req = jdRequest()
    req.function = "xy"
    req.x = x
    req.y=y
    req.phi=(phi*math.pi)/180
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)
def WheelSpacing(spacing_wheel):
    global still
    if still == True:
        req = jdRequest()
        req.vitesse = spacing_wheel
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def EncoderSpacing(spacing_encoder):
    global still
    if still == True:
        req = jdRequest()
        req.function = "EncoderSpacing"
        req.vitesse = spacing_encoder
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def RightRadius(right_radius):
    global still
    if still == True:
        req = jdRequest()
        req.function = "RightRadius"
        req.vitesse = right_radius
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def LeftRadius(left_radius):
    global still
    if still == True:
        req = jdRequest()
        req.function = "LeftRadius"
        req.vitesse = left_radius
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def Go(goal_x, goal_y, speed):
    global still
    if still == True:
        req = jdRequest()
        req.function = "Go"
        req.vitesse = speed
        req.x = goal_x
        req.y = goal_y
        try:
            resp = stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def Gobst(goal_x, goal_y, speed):
    global still
    if still == True:
        req = jdRequest()
        req.function = "Gobst"
        req.vitesse = speed
        req.x = goal_x
        req.y = goal_y
        try:
            resp = stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def GoMultiCurv(m, n, speed):
    global still
    if still == True:
        req = jdRequest()
        req.function = "GoMultiCurv"
        req.c = m
        req.nombre = n
        req.vitesse = speed
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def GoCurv(x, y, phi, speed):
    global still
    if still == True:
        req = jdRequest()
        req.function = "GoCurv"
        req.x = x
        req.y = y
        req.phi = phi
        req.vitesse = speed
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def Asta3(distance,speed):
    global still
    if still == True:
        req=jdRequest()
        req.function="Asta3"
        req.vitesse=speed
        req.x=distance
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)



def Asta3L(distance,speed):
    global still
    if still == True:
        req=jdRequest()
        req.function="Asta3L"
        req.vitesse=speed
        req.x=distance
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def Move(distance,speed):
    global still
    if still == True:
        req=jdRequest()
        req.function="Move"
        req.vitesse=speed
        req.x=distance
        try:
            resp=stmService(req)
            #rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def Orientate(angle,speed):
    global still
    if still == True:
        req=jdRequest()
        req.function="Orientate"
        req.vitesse=speed
        req.phi= angle
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)

def Rotate(angle,speed):
    global still
    if still == True:
        req=jdRequest()
        #if(angle<0):
            #req.start_time.secs=1
        req.function="Rotate"
        req.speed=speed
        req.x= angle
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def c_callback(msg:c):
    global x
    global y 
    global phi 
    x = msg.x
    y = msg.y
    phi = msg.phi
    print(phi)


def sendArduino(seq):
        try:
           arduino.flush()
           arduino.write(seq.encode("UTF-8"))
        except KeyboardInterrupt:
            print("KeyboardInterrupt has been caught.")


def sequence_R1_pickup_BP_GP():
    Gobst(-couleur*775,415,500)
    Orientate(90,100)
    xy(-couleur*775,415,90)
    sendArduino("b")
    time.sleep(3)
    Move(200,200)
    xy(-couleur*775,615,90)
    sendArduino("HIZ")
    time.sleep(3)
    Orientate((couleur == 1) * 135 + (couleur != 1) * 45,100)
    xy(-couleur*775,615,(couleur == 1) * 135 + (couleur != 1) * 45)
    Gobst(-couleur*435,965,500)
    xy(-couleur*435,965,(couleur == 1) * 135 + (couleur != 1) * 45)
    sendArduino("HIZ")
    time.sleep(3)

def sequence_R1_pickup_BSR_GSL():
    Gobst(couleur*775,2585,500)
    Orientate(-90,100)
    xy(couleur*775,2585,-90)
    sendArduino("HIZ")
    time.sleep(3)
    Move(200,200)
    xy(couleur*775,2385,-90)
    sendArduino("HIZ")
    time.sleep(3)
    Orientate((couleur == 1) * (-45) + (couleur != 1) * (-135),100)
    xy(couleur*775,2385,(couleur == 1) * (-45) + (couleur != 1) * (-135))
    Gobst(couleur*435,2035,500)
    xy(couleur*435,2035,(couleur == 1) * (-45) + (couleur != 1) * (-135))
    sendArduino("HIZ")
    time.sleep(3)
    
def sequence_R1_drop_BP_GP():
    Gobst(-couleur*850,225,500)
    Orientate(90(1 - couleur),100)
    Asta3(-500,850)
    Move(270,500)
    xy(-couleur*730,225,90(1 - couleur))
    sendArduino("TRI 3 GATEAUX")
    time.sleep(86)
    xy(-couleur*730,225,90(1 - couleur))
    Move(-50,500)
    Orientate( 180 * (1 - couleur),100)
    xy(-couleur*680,225,180 * (1 - couleur))
    Move(75,500)
    sendArduino("Drop cerise")
    time.sleep(3)
    Move(-75,500)
    xy(-couleur*680,225,180 * (1 - couleur))


def sequence_R1_drop_BSR_GSL(): 
    Gobst(couleur*775,2775,500)
    Orientate(180 * (couleur == 1),100)
    Asta3(500,850)
    Move(-250,500)
    xy(couleur*750,2775,180 * (couleur == 1))
    sendArduino("TRI 3 GATEAUX")
    time.sleep(86)
    xy(couleur*750,2775,180 * (couleur == 1))
    Move(-50,500)
    Orientate(180 * (1 - couleur),100)
    xy(couleur*750,2775,180 * (1 - couleur))
    Move(75,500)
    sendArduino("Drop cerise")
    time.sleep(3)
    Move(-75,500)
    xy(couleur*750,2775,180 * (1 - couleur))

def sequence_R1_drop_BSL_GSR(): 
    Gobst(-couleur*275,2810,500)
    Orientate(90,100)
    Asta3(400,600)
    Move(-290,500)
    xy(couleur*275,2710,90)
    sendArduino("TRI 3 GATEAUX")
    time.sleep(86)
    xy(couleur*275,2710,90)
    Move(-50,500)
    Orientate(-90,100)
    xy(couleur*275,2660,-90)
    Move(75,500)
    sendArduino("Drop cerise")
    time.sleep(3)
    Move(-75,500)
    xy(couleur*275,2660,180 * (1 - couleur))

def sequence_R1_drop_BL_GR(): 
    Gobst(-couleur*710,1875,500)
    Orientate(180 * (1 - couleur),100)
    Asta3(400,600)
    Move(-290,500)
    xy(-couleur*710,1875,180 * (1 - couleur),350)
    sendArduino("TRI 3 GATEAUX")
    time.sleep(86)
    xy(-couleur*710,1875,180 * (1 - couleur),350)
    Move(-50,500)
    Orientate(180 * (couleur==1),100)
    xy(-couleur*660,1875,180 * (couleur==1))
    Move(75,500)
    sendArduino("Drop cerise")
    time.sleep(3)
    Move(-75,500)
    xy(-couleur*660,1875,180 * (couleur==1))

def sequence_R1_drop_BR_GL(): 
    Gobst(couleur*710,1125,500)
    Orientate(180 * (couleur == 1),100)
    Asta3(400,600)
    Move(-290,500)
    xy(couleur*710,1125,180 * (couleur == 1),350)
    sendArduino("TRI 3 GATEAUX")
    time.sleep(86)
    xy(couleur*710,1125,180 * (couleur == 1),350)
    Move(-50,500)
    Orientate(180 * (1 - couleur),100)
    xy(couleur*660,1125,180 * (1-couleur))
    Move(75,500)
    sendArduino("Drop cerise")
    time.sleep(3)
    Move(-75,500)
    xy(couleur*660,1125,180 * (1-couleur))

def sequence_tri_():
    global currentSupport,i,couleur,couleur_angle

    t0= datetime.now()
    try:
       
    except KeyboardInterrupt:
        print("KeyboardInterrupt has been caught fel sequence .")
    print((datetime.now()-t0).total_seconds())





print("Begining..............")
with serial.Serial(portName, 9600, timeout=3,bytesize=serial.EIGHTBITS) as arduino:
    while not arduino.isOpen():
        print("Waiting for arduino Serial")
    print("{} connected!".format(arduino.port))
    try:

        time.sleep(0.5)
        print("in spin")
        sequence_ajneb()
        rospy.spin()
	 #   print("after spin")
    except KeyboardInterrupt:
        print("KeyboardInterrupt has been caught.")