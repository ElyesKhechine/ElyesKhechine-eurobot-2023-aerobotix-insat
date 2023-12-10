#! /usr/env python3

from sqlite3 import Time
import time
import serial
import rospy
from gazebo_msgs.srv import *
import time
import numpy as np
import json
import os
from std_msgs.msg import Bool
from std_msgs.msg import Int8
from std_msgs.msg import Int32
from playsound import playsound




# import sequence

color="Violet"
strategy=1
score=0



def putParamSghir(param,value):
    fileObject = open("/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/scripts/StmParamSghir.json", "r")
    aList = json.load(fileObject)
    fileObject.close()
    aList[param]=value

    with open("/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/scripts/StmParamSghir.json", "w") as file:
        json.dump(aList, file)
    file.close()
def putParam(param,value):
    fileObject = open("/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/scripts/StmParam.json", "r")
    aList = json.load(fileObject)
    fileObject.close()
    aList[param]=value

    with open("/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/scripts/StmParam.json", "w") as file:
        json.dump(aList, file)
    file.close()
def initLastValues():
    fileObject = open("/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/scripts/StmParam.json", "r")
    aList = json.load(fileObject)
    fileObject.close()
    for element in aList:
        # time.sleep(0.5)        
        # ser.write(value.encode())
        x=ApplyJointEffortRequest()
        x.joint_name=str(element)
        x.effort=float(aList[element]/100000)
        x.duration.secs=1
        x.duration.nsecs=1
        x.start_time.secs=1
        x.start_time.nsecs=1
        try:
            resp=stmService(x)
            if(resp.success== True):
                print(resp)
                print("Done Setting "+str(element))
            param=str(aList[element])
            if(len(param)<8):
                    for i in range(8-len(param)):
                        param="0"+param
            ser.write(param.encode())
            time.sleep(0.2)
            
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)
    print("Done Initialisation Robot Kbir")
    ser.flush()
    
    
    fileObject = open("/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/scripts/StmParamSghir.json", "r")
    aList = json.load(fileObject)
    fileObject.close()
    for element in aList:
        # time.sleep(0.5)        
        # ser.write(value.encode())
        x=ApplyJointEffortRequest()
        x.joint_name=str(element)
        x.effort=float(aList[element]/100000)
        x.duration.secs=1
        x.duration.nsecs=1
        x.start_time.secs=1
        x.start_time.nsecs=1
        try:
            resp=stmServiceSghir(x)
            if(resp.success== True):
                print(resp)
                print("Done Setting "+str(element))
            param=str(aList[element])
            if(len(param)<8):
                    for i in range(8-len(param)):
                        param="0"+param
            ser.write(param.encode())
            time.sleep(0.2)
            
        except rospy.ServiceException as e:
            print("Service call failed: %s"%e)
    print("Done Initialisation Robot Sghir")
    ser.flush()

def sendScreenRequest():
    fileObject = open("/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/scripts/StmParam.json", "r")
    aList = json.load(fileObject)
    fileObject.close()
    for element in aList:
        ser.write(str(aList[element]).encode())
        time.sleep(0.5)        




portScreen=os.popen('cat /home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/config/screen.port').read()
while len(portScreen)<11:
    portScreen=os.popen('cat /home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/config/screen.port').read()
    print('\033[2;41;41m Trying to connect to Screen Please check the port')

print("The port of Screen is " +portScreen)




ser = serial.Serial(
        port=portScreen, #Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
        baudrate = 115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)


def sendScore(scoreplus):
    global score
    sco=str(score+scoreplus)
    if(len(sco)<8):
                    for i in range(8-len(sco)):
                        sco="0"+sco
    ser.write(str(sco).encode())
    
def tirette_Callback(msg):
    if msg.data==True:
        val="1"
        if(len(val)<8):
                    for i in range(8-len(val)):
                        val="0"+val
        sendScore(val)
        print("1 sent")
def score_Callback(msg):
    sendScore(msg.data)






if __name__ == "__main__":
    rospy.init_node('ScreenCom')
    rospy.wait_for_service('/StmCommand')
    stmService = rospy.ServiceProxy('/StmCommand',ApplyJointEffort)
    rospy.wait_for_service('/StmCommandSghir')
    stmServiceSghir = rospy.ServiceProxy('/StmCommandSghir',ApplyJointEffort)
    tiretteSub=rospy.Subscriber("/tirette",Bool,tirette_Callback,queue_size=100)
    scoreSub=rospy.Subscriber("/score",Int32,score_Callback,queue_size=100)
    
    color_pub=rospy.Publisher("/color",Int8,queue_size=100)
    strategy_pub=rospy.Publisher("/strategy",Int8,queue_size=100)
    confirm_pub=rospy.Publisher("/confirm",Bool,queue_size=100)
    scriptSghir_pub=rospy.Publisher("/waitforscriptSghir",Bool,queue_size=100)
    scriptKbir_pub=rospy.Publisher("/waitforscript",Bool,queue_size=100)
    testKbir_pub=rospy.Publisher("/testKbir",Int8,queue_size=100)
    testSghir_pub=rospy.Publisher("/testSghir",Int8,queue_size=100)
    playsound("/home/ubuntu/AeroBotiX_euro2021/src/reel_euro2021/scripts/screenReady.mp3")
    
      
    print("initializing")
    initLastValues()
    print("Done initializing")

    x=ApplyJointEffortRequest()
    y=ApplyJointEffortResponse()
    while 1:
        # inp="rat=5542"
        
        inp=""
        inp=str(ser.readline().decode());
        print(inp)
        if(inp!="" ):
            inp=inp.split('x')[0];
            tab1=inp.split(':')
            if(len(tab1)==1):
                com=tab1[0].split('=')[0]
                if(com=="Strategy"):
                    
                    strategy=int(tab1[0].split('=')[1])
                    ii=Int8()
                    ii.data=strategy
                    strategy_pub.publish(ii)
                    print("Strategy  "+str(strategy))
                    
                    if color=="Yellow":
                        sendScore(1)
                    elif color=="Violet":
                        sendScore(2)
                    time.sleep(0.2)
                    sendScore(strategy)
                    
                    # sequence.play(color,strategy)
                elif(com=="Yellow"):
                    print("Yellow")
                    color="Yellow"
                    ii=Int8()
                    ii.data=1
                    color_pub.publish(ii)
                elif(com=="Violet"):
                    print("Violet")
                    color="Violet"
                    ii=Int8()
                    ii.data=2
                    color_pub.publish(ii)
                elif(com=="Confirmed"):
                    print("Confirmed")
                    ii=Bool()
                    ii.data=True
                    confirm_pub.publish(ii)
                    
                    
                else:        
                    test=tab1[0].split('-')[1].split('=')[0]
                    if(test=="RS"):
                        tab2=inp.split('=')
                        command=tab2[0].split('-')[0]
                        value=tab2[1].split('x')[0]
                        print(command)
                        print(value)

                        if(len(value)<8):
                            for i in range(8-len(value)):
                                value="0"+value

                        time.sleep(0.2)
                        
                        ser.write(value.encode())
                        putParamSghir(command,int(value))
                        x.joint_name=command
                        x.effort=float(value)/100000
                        x.duration.secs=1;
                        x.duration.nsecs=1;
                        x.start_time.secs=1;
                        x.start_time.nsecs=1;


                    
                        try:
                            resp=stmServiceSghir(x)
                            # rospy.spin()
                            print(resp)
                        except rospy.ServiceException as e:
                            print("Service call failed: %s"%e)
                    elif(test=="RK"):
                        tab2=inp.split('=')
                        command=tab2[0].split('-')[0]
                        value=tab2[1].split('x')[0]
                        print(command)
                        print(value)

                        if(len(value)<8):
                            for i in range(8-len(value)):
                                value="0"+value

                        time.sleep(0.2)
                        
                        ser.write(value.encode())
                        putParam(command,int(value))
                        x.joint_name=command
                        x.effort=float(value)/100000
                        x.duration.secs=1;
                        x.duration.nsecs=1;
                        x.start_time.secs=1;
                        x.start_time.nsecs=1;


                    
                        try:
                            resp=stmService(x)
                            # rospy.spin()
                            print(resp)
                        except rospy.ServiceException as e:
                            print("Service call failed: %s"%e)
            elif(len(tab1)>1):
                test=tab1[0].split('-')[1]
                
                if(test=="RS"):
                    com=tab1[0].split('-')[0]
                    if(com=="Go"):
                        print("go")
                        xx=tab1[1].split('Y')[0].split('=')[1]
                        yy=tab1[1].split('=')[2]
                        x.joint_name="Go"
                        if int(xx)<0:
                            x.start_time.secs=1
                        if int(yy)<0:
                            x.start_time.nsecs=1 
                        x.duration.secs=abs(int(xx))
                        x.duration.nsecs=abs(int(yy))
                        x.effort=250
                        try:
                            resp=stmServiceSghir(x)
                            print(resp)
                        except rospy.ServiceException as e:
                            print("Service call failed: %s"%e)
                    elif(com=="Rotate"):
                        print("rotate")
                        num=int(tab1[1])
                        x.joint_name="Rotate"
                        x.duration.secs=num
                        x.effort=250
                        try:
                            resp=stmServiceSghir(x)
                            print(resp)
                        except rospy.ServiceException as e:
                            print("Service call failed: %s"%e)
                    
                    elif(com=="Debug"):
                        print("Debug")
                        print(tab1[1])
                        ii=Bool()
                        ii.data=True
                        scriptSghir_pub.publish(ii)
                    elif(com=="Motors"):
                        print("Motors")
                        print(tab1[1])
                        x.joint_name="Motors"
                        try:
                            resp=stmServiceSghir(x)
                            print(resp)
                        except rospy.ServiceException as e:
                            print("Service call failed: %s"%e)
                elif (test=="RK"):
                    com=tab1[0].split('-')[0]
                    if(com=="Go"):
                        print("go2")
                        xx=tab1[1].split('Y')[0].split('=')[1]
                        yy=tab1[1].split('=')[2]
                        x.joint_name="Go"
                        if int(xx)<0:
                            x.start_time.secs=1
                        if int(yy)<0:
                            x.start_time.nsecs=1 
                        x.duration.secs=abs(int(xx))
                        x.duration.nsecs=abs(int(yy))
                        x.effort=250
                        try:
                            resp=stmService(x)
                            print(resp)
                        except rospy.ServiceException as e:
                            print("Service call failed: %s"%e)
                    elif(com=="Rotate"):
                        print("rotate2")
                        num=int(tab1[1])
                        x.joint_name="Rotate"
                        x.duration.secs=num
                        x.effort=250
                        try:
                            resp=stmService(x)
                            print(resp)
                        except rospy.ServiceException as e:
                            print("Service call failed: %s"%e)
                    elif(com=="Debug"):
                        print("Debugaaaaaa")
                        print(tab1[1])
                        ii=Bool()
                        ii.data=True
                        scriptKbir_pub.publish(ii)
                    elif(com=="Motors"):
                        print("here")
                        
                        y=ApplyJointEffortRequest()
                        y.joint_name="Motors"
                        try:
                            resp=stmService(y)
                            print(resp)
                        except rospy.ServiceException as e:
                            print("Service call failed: %s"%e)

