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
    global tabdila 
    tabdila = True
    print("fel callback",tabdila)
    permutation_priorite()
    print ('saret TABDILAAAAAAAAAAAA')

rospy.init_node('SequenceSghir')
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

def sequence_support1():
    Orientate(0,350)
    sendArduino("c")
    time.sleep(0.5)
    sendArduino("d")
    time.sleep(0.5)
    Asta3(680,850)
    xy(855,0,0)
    time.sleep(0.5)
    Move(-300,500)
    sendArduino("b")
    time.sleep(1)
    sendArduino("a")
    time.sleep(1)
    sendArduino("b")


def sequence_support0_vert():
    Orientate(-90,350)
    Asta3(700,200)
    xy(0,80,-90)
    Move(-85.5,350) 
    Orientate(0,350)
    sendArduino("y")
    time.sleep(0.5)
    Asta3(1000,130)
    xy(-95,0,0)
    Move(-55.5,300)
    Orientate(0,300)
    sendArduino("z")
    time.sleep(1)
    sendArduino("d")
    time.sleep(0.5)
    Move(20,300)
    sendArduino("b")
    time.sleep(1.5)
    #sendArduino("z")
    #time.sleep(2)
    #sendArduino("b")
    #time.sleep(1)
    Move(-300,500)
    sendArduino("a")
    time.sleep(1)
    sendArduino("b")


def sequence_support2_vert():
    Orientate(90,350)
    Asta3(700,150)
    xy(0,2920,90)
    Move(-81,300) 
    Orientate(0,350)
    sendArduino("x")
    time.sleep(1)
    Asta3(1000,100)
    xy(-95,0,0)
    Move(-59,300)
    Orientate(0,300)
    sendArduino("w")
    time.sleep(1.5)
    Move(20,300)
    sendArduino("d")
    time.sleep(0.5)
    sendArduino("b")
    time.sleep(1)
    Asta3(300,200)
    xy(-95,0,0)
    #sendArduino("z")
    #time.sleep(2)
    #sendArduino("b")
    #time.sleep(1)
    Move(-300,500)
    sendArduino("a")
    time.sleep(1)
    sendArduino("b")

def sequence_support0_bleu():
    Orientate(-90,350)
    Asta3(700,200)
    xy(0,80,-90)
    Move(-85.5,350) 
    Orientate(180,350)
    sendArduino("x")
    time.sleep(0.5)
    Asta3(1000,130)
    xy(95,0,180)
    Move(-57,300)
    Orientate(180,300)
    sendArduino("w")
    time.sleep(2)
  
    Move(20,300)
    sendArduino("b")
    time.sleep(1.5)
    #sendArduino("z")
    #time.sleep(2)
    #sendArduino("b")
    #time.sleep(1)
    Move(-300,500)
    sendArduino("a")
    time.sleep(2)
    sendArduino("b")


def sequence_support2_bleu():
    Orientate(90,350)
    Asta3(700,150)
    xy(0,2920,90)
    Move(-81,300) 
    Orientate(180,350)
    sendArduino("y")
    time.sleep(1)
    Asta3(1000,100)
    xy(95,0,180)
    Move(-59,300)
    Orientate(180,300)
    sendArduino("z")
    time.sleep(1.5)
    Move(20,300)
    sendArduino("b")
    time.sleep(1.5)
    Asta3(300,200)
    xy(95,0,180)
    #sendArduino("z")
    #time.sleep(2)
    #sendArduino("b")
    #time.sleep(1)
    Move(-300,500)
    sendArduino("a")
    time.sleep(2)
    sendArduino("b")

def sequence_support3():
    Orientate(180,350)
    sendArduino("c")
    time.sleep(0.5)
    sendArduino("d")
    time.sleep(1)
    Asta3(750,850)
    xy(-855,0,180)
    time.sleep(0.5)
    Move(-300,500) 
    sendArduino("b")
    time.sleep(1)
    sendArduino("a")
    time.sleep(1)
    sendArduino("b")




def hot_panier():
    Go(-850,300,500)
    Orientate(90,300)
    Asta3(-500,200)
    sendArduino("f")
    time.sleep(4)
    sendArduino("e")
    time.sleep(0.5)
    sendArduino("d")
    Move(200,500)
i=0
def sequence_ajneb():
    setPriorities(3,2,1,0)
    global currentSupport,tabdila,i,couleur,couleur_angle

    t0= datetime.now()
    try:
        xy(couleur*705,260,couleur_angle*179) #680-375-90
        sendArduino("b")
        time.sleep(0.5)
        while (cherryState != [1.,1.,1.,1.]).all:
            tabdila = False
            supportCoords,currentSupport = choiceCherries()
            print("cherriess : " , cherryState)
            print("coords of ", currentSupport , " : ",supportCoords)
            print(tabdila)
            i=np.sum(cherryState)
            # if currentSupport == 0  and (cherryState[currentSupport] == 0):
            #     Gobst(couleur*supportCoords[0],supportCoords[1],500)
            #     if tabdila and i <2:
            #         print("fok haw tabdila")
            #         continue
            #     if couleur==1:
            #         sequence_support0_bleu()
            #     else:
            #         sequence_support0_vert()
            #     cherryState[currentSupport]=1
            #     permutation_priorite()
            if currentSupport == 1  and (cherryState[currentSupport] == 0):
                if couleur ==-1:
                    Gobst(680,1800,500)
                Gobst(supportCoords[0],supportCoords[1],500)
                if tabdila and i < 2:
                    print("fok haw tabdila")
                    continue
                sequence_support1()
                cherryState[currentSupport]=1
                permutation_priorite()
                continue
            if currentSupport == 2  and (cherryState[currentSupport] == 0):
                Gobst(couleur*supportCoords[0],supportCoords[1],500)
                if tabdila and i<2:
                    print("fok haw tabdila")
                    continue
                if couleur==1:
                    sequence_support2_bleu()
                else: 
                    sequence_support2_vert()
                cherryState[currentSupport]=1
                permutation_priorite()
                continue
            if currentSupport == 3  and (cherryState[currentSupport] == 0):
                Gobst(supportCoords[0],supportCoords[1],500)
                if tabdila and i<2:
                    print("fok haw tabdila")
                    continue
                sequence_support3()
                cherryState[currentSupport]=1
                permutation_priorite()
                continue    
            if(i>2): break    
            permutation_priorite()

        #haz supporat l 3

        hot_panier()
        Gobst(750,1100,800)
    except KeyboardInterrupt:
        print("KeyboardInterrupt has been caught fel sequence .")
    print((datetime.now()-t0).total_seconds())

cherryState = np.zeros(4) #[0,0,0,0]

cherries = np.array([[680,1500,1,1], [250,2700,2,2], [-600,1500,3,3],[250,200,0,0]]) #[x,y,priority(0 is highest),number]
"""[    1/            [ 680 1500]
        2/            [ 250 2700]
        3/            [-600 1500]   
        0/            [ 250 200 ]           ] """

def setPriorities(c1 = 1, c2 = 2 , c3 = 3,c4=0):
    global cherries
    cherries[0][2] = c1
    cherries[1][2] = c2
    cherries[2][2] = c3
    cherries[3][2] = c4

def choiceCherries():
    global cherries
    min = cherries[0]
    for i in cherries:
        if(i[2]<min[2]):
            min=i
    print (min,"hedhi li bech yemchilha")        
    return [min[0],min[1]],min[3]        
k=0
def permutation_priorite():
    global cherries
    global k
    global couleur
    print("tabdiiiiiiiil :  " , i)
    if(k>3):
        k=0
    # Define the order of priorities for each support
    if couleur==1:
        priorities = [ [0,1,2,3], [3,0,1,2], [2,3,0,1],[1,2,3,0]]
    else:
        priorities = [ [2,3,0,1],[3,0,1,2],[0,1,2,3], [1,2,3,0]]

    # Get the current priorities for the selected support
    current_priorities = priorities[k]
    k+=1    
    # Update the priorities of the cherries based on the new order of priorities
    for j, priority in enumerate(current_priorities):
        cherry = cherries[j]
        cherry[2] = priority
    print(cherries)
    print(current_priorities)

# def permutation_priorite(i):
#     global cherries
#     print("tabdiiiiiiiil :  " , i)
#     if i==0: #tawkhir support 1
#         if cherries[1][2]<cherries[2][2]:
#             setPriorities(2,0,1)
#         else:
#             setPriorities(2,1,0)  
            
    
#     if i==1: #tawkhir support 2
#         if cherries[0][2]<cherries[2][2]: 
#             setPriorities(0,2,1)
#         else:
#             setPriorities(1,2,0)
    
#     if i==2: #tawkhir support 3
#         if cherries[0][2]<cherries[1][2]: 
#             setPriorities(1,2,3,0)
#         else:
#             setPriorities(2,1,3,0)
#     if i==3: #tawkhir support 4
#         if cherries[0][2]<cherries[1][]:
#             setPriorities()        
#     print(cherries)

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