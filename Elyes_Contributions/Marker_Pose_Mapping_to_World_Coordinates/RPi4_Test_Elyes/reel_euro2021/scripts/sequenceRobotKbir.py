#! /bin/env python3

import ast
from shutil import move
import serial,time,os
import rospy
from  gazebo_msgs.srv import ApplyJointEffortRequest
from  gazebo_msgs.srv import ApplyJointEffort
from std_msgs.msg import Bool
from std_msgs.msg import Int8
from std_msgs.msg import Int32


from geometry_msgs.msg import Point
from gazebo_msgs.srv import GetLightPropertiesRequest
from gazebo_msgs.srv import GetLightPropertiesResponse
from gazebo_msgs.srv import GetLightProperties
import evitementCmd
# import screenReciever
# import evitementCmd


waitInGO=False
color="Violet"
strategy=2



portName=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
while len(portName)<11:
    portScreen=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
    print('\033[2;41;41m Trying to connect to Arduino Please check the port')

print("The port of Arduino is " +portName)
os.system('sudo chmod 777 '+portName)



def testKbir_Callback(msg):
    sendArduino('i')
    
def color_Callback(msg):
    print("color")

    global color
    if msg.data==1:
        color="Yellow"
    else:
        color="Violet"
def strategy_Callback(msg):
    print("strategy")
    global strategy
    strategy=msg.data
    
def confirm_Callback(msg):
    global color
    global strategy
    print("waaaaa")
    if color=="Violet" and strategy==1:
        Lanci()
        sendArduino("J")
        emchiLelktaya3Louta()
        lemmKa3betInc()
        HottFelGalerie()
        EmchiIncBahdhaCamp()
        EkherPlaceFelGalerie()
        FinalRetourCamp()
        go(1050,1200,700)
        Orientate(600,-90)              
        

                
    elif color=="Violet" and strategy==2:
        Lanci()
        sendArduino("J")
        emchiLelktaya3Louta()
        lemmKa3betInc()
        HottFelGalerie()
        EmchiIncBahdhaCamp()
        EkherPlaceFelGalerie()
        FinalRetourCamp()
        # go(1050,1200,700)
        # Orientate(600,-90) 


                

    elif color=="Violet" and strategy==3:
                   
        strategy3V()
        scoreToScreen(300)

              
                

                

    elif color=="Violet" and strategy==4:
                  
        strategy4V()
        scoreToScreen(400)

                

    # play(color,strategy)

def scoreToScreen(score):
    x=Int32()
    x.data=int(score)
    score_pub.publish(x)


    
def vitrine():
    x=Int32()
    x.data=1
    vitrine_pub.publish(x)
    
def RKO():
    x=Int32()
    x.data=2
    vitrine_pub.publish(x)
    

rospy.init_node('SequenceKbir')
rospy.wait_for_service('/StmCommand')
rospy.wait_for_service('/StmPose')
stmService = rospy.ServiceProxy('/StmCommand',ApplyJointEffort)
stmPose = rospy.ServiceProxy('/StmPose',GetLightProperties)

testKbir=rospy.Subscriber("/testKbir",Int8,testKbir_Callback,queue_size=100)
colorSub=rospy.Subscriber("/color",Int8,color_Callback,queue_size=100)
strategySub=rospy.Subscriber("/strategy",Int8,strategy_Callback,queue_size=100)
confirmSub=rospy.Subscriber("/confirm",Bool,confirm_Callback,queue_size=100)
score_pub=rospy.Publisher("/score",Int32,queue_size=100)
vitrine_pub=rospy.Publisher("/vitrine",Int32,queue_size=100)



# inGo_sub=rospy.Subscriber("/ingo",Bool,inGo_calllback,queue_size=100)



print("initializing")


def Curv (x,y,phi,speed):
    req=ApplyJointEffortRequest()
    req.joint_name="Curv"
    req.start_time.secs=x
    req.start_time.nsecs=y
    req.duration.secs=phi
    req.duration.nsecs=speed
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)



def Orientate(speed,angle):
    req=ApplyJointEffortRequest()
    if(angle<0):
        req.start_time.secs=1
    req.joint_name="Orientate"
    req.effort=speed
    req.duration.secs=abs(angle)
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)

def Lanci():
    req=ApplyJointEffortRequest()
    req.joint_name="Lanci"
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)
def Move(speed,distance):
    req=ApplyJointEffortRequest()
    if(distance<0):
        req.start_time.secs=1
    req.joint_name="Move"
    req.effort=speed
    req.duration.secs=abs(distance)
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)

def Asta3(speed,distance):
    req=ApplyJointEffortRequest()
    if(distance<0):
        req.start_time.secs=1
    req.joint_name="Asta3"
    req.effort=speed
    req.duration.secs=abs(distance)
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)


    def UpdatePos(x,y,angle):
        req=ApplyJointEffortRequest()
        req.joint_name="Update"
        req.effort=angle
        req.duration.secs=x
        req.duration.nsecs=y
        try:
            resp=stmService(req)
            # rospy.spin()
            print(resp)
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)


def Asta3L(speed,distance):
    req=ApplyJointEffortRequest()
    if(distance<0):
        req.start_time.secs=1
    req.joint_name="Asta3L"
    req.effort=speed
    req.duration.secs=abs(distance)
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)



def go(x,y,speedF):
    # global waitInGO
    evitementCmd.publishGoal(int(x),int(y),int(speedF))
    
    
    
    
    req=ApplyJointEffortRequest()
    if(x<0):
        req.start_time.secs=1
    if(y<0):
        req.start_time.nsecs=1
    req.joint_name="Go"
    req.effort=speedF
    req.duration.secs=abs(x)
    req.duration.nsecs=abs(y)
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)
    # print("waitt"+str(waitInGO))
    # while waitInGO==True:
        # print("waiting seq")
    # raja3haaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa tab3a evitement    
        
    test=False
    '''while test==False:
        try:
            xx=GetLightPropertiesRequest()
            xx.light_name=""
            if(evitementCmd.nemchi==False):
                resp=stmPose(xx)
                print(resp)          
                if(abs(int(resp.attenuation_constant)-int(x))<30 and abs(int(resp.attenuation_linear)-int(y))<30):
                    if(abs(int(resp.attenuation_constant)-int(x))<10 and abs(int(resp.attenuation_linear)-int(y))<10):
                        test==True
                        print(abs(int(resp.attenuation_linear)-int(y)))
                        break
                    else:
                        go(x,y,speedF)
                        
     
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)  
    print("Doneeeeeeeeeeeeeeeeeeeeee")'''  


def sendArduino(seq):
        try:
        #    x=input("aaaaaaaaaaa")
           arduino.flush()
           arduino.write(seq.encode("UTF-8"))
        #    arduino.write(x.encode("UTF-8"))
        except KeyboardInterrupt:
            print("KeyboardInterrupt has been caught.")


def emchiLelktaya3Louta():
    
    go(680,-675,700) #710,660,500
    Orientate(500,0)
    #Move(500,70)
    sendArduino("A")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    print("out")
    sendArduino("B")
    
    go(1255,-320,700) #1220,360,500
    Orientate(700,90)
    #time.sleep(1)
    Asta3L(100,200)
    #time.sleep(2)

    sendArduino("B") 
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    sendArduino("B") 


def lemmKa3betInc():
    #arduino.readline()
    sendArduino("C")
    Move(100,-95)
    Orientate(500,90)
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()    
    sendArduino("B")    
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Orientate(500,90)
    Asta3L(200,120)
    print("kaddamt lmarra loula")   
    sendArduino("B")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Orientate(500,90)   
    Asta3L(200,100)
    sendArduino("B")
    print("kaddamt marra thenya")    
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    sendArduino("B")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Orientate(500,90)
    Asta3L(200,100)
    print("kaddamt marra theltha")   
    sendArduino("B")
    # arduino.flush()
    #Move(200,-40)    
    while(not(arduino.read().decode())=="A"):
        print("waiting")  
    arduino.flush()
    Orientate(500,90)
    Asta3L(200,200)
    sendArduino("B")
    time.sleep(0.5)


def HottFelGalerie():
    # arduino.readline()
    Move(100,-150)
    Orientate(300,180)
    sendArduino("D")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()    
    sendArduino("B")
    go(645,-320,700) #620,320,500#650
    Orientate(500,90)
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()    
    Asta3L(200,500)
    sendArduino("B")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()    
    time.sleep(0.5)
    sendArduino("B")
    Move(200,-70)
    Orientate(500,90)
    # sendArduino("B")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Orientate(500,90)
    Asta3L(200,500)
    sendArduino("B")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    # sendArduino("B")
    Move(200,-150)
    sendArduino("B")
    Orientate(500,90)
    # sendArduino("B")
    while(not(arduino.read().decode())=="A"):
        print("waiting") 
    Asta3L(200,60)
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Asta3L(200,500)
    sendArduino("B")
    time.sleep(0.8)
    Move(200,-150)
    #sendArduino("B")
    Orientate(500,90)
    # sendArduino("B")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Orientate(500,90)
    Asta3L(200,500)
    sendArduino("B")
    time.sleep(0.5)
    Move(200,-120)

def EmchiIncBahdhaCamp():
    # arduino.readline()
    sendArduino("T")
    Move(700,320-1320) #go 600,1400,500
    Orientate(500,180)
    go(320,-1320,700)
    Orientate(500,180)
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Asta3L(200,500)    
    sendArduino("B")    
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()    
    Orientate(500,180)
    Asta3L(200,500)  
    sendArduino("B")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Orientate(500,180)   
    Asta3L(200,500)
    sendArduino("B")   
    time.sleep(0.5)
    arduino.flush()
    Move(700,-320)
    Orientate(700,90)

def EkherPlaceFelGalerie():
    # na7iii readline khater fama delay 
    #arduino.readline()
    sendArduino("Y")
    go(1020,-320,700) #950,320,500
    Orientate(500,90)
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()    
    Asta3L(200,200)
    sendArduino("B")
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Move(200,-150)
    sendArduino("B") 
    Orientate(500,90)
    while(not(arduino.read().decode())=="A"):
        print("waiting")
    arduino.flush()
    Asta3L(200,200)
    sendArduino("B")
    time.sleep(0.5)


def FinalRetourCamp():
    sendArduino('U')
    go(700,-260,638)
    while(not(arduino.read().decode())=="A"):
        print("waiting")    
    arduino.flush()
    sendArduino("B")

       




def strategy1V():
    emchiLelktaya3Louta()
    lemmKa3betInc()
    emchiLelktaya3Louta()
    lemmKa3betInc()
    HottFelGalerie()
    EmchiIncBahdhaCamp()
def strategy2V():
    go(260,800,300)
def strategy3V():
    go(260,900,350)
def strategy4V():
    go(260,1000,400)




print("Begining..............")


with serial.Serial(portName, 9600, timeout=3,bytesize=serial.EIGHTBITS) as arduino:
    while not arduino.isOpen():
        print("Waiting for arduino Serial")
    print("{} connected!".format(arduino.port))
    try:
        print("in spin")
       # Move(300,155)
        #go(260,660,1000)
        #Orientate(400,0)
        #time.sleep(4)
        Move(700,150)
        Orientate(700,0)
        
        rospy.spin()
        print("after spin")
        # emchiLelktaya3Louta()
        # lemmKa3betInc()
        # HottFelGalerie()
        # go(1050,1200,700)
        # Orientate(600,-90)
        #EmchiIncBahdhaCamp()
        #time.sleep(4)
        #EkherPlaceFelGalerie()
    except KeyboardInterrupt:
        print("KeyboardInterrupt has been caught.")
