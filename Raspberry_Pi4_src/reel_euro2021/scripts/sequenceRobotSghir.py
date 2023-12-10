from math import pi
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
#import evitementCmdSghir
# import screenReciever
# import evitementCmd


waitInGO=False
color="Violet"
strategy=2
pres=0


fouille1=False
fouille2=False
find1=False
find2=False   




portName=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
while len(portName)<11:
    portScreen=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
    print('\033[2;41;41m Trying to connect to Arduino Please check the port')

print("The port of Arduino is " +portName)
os.system('sudo chmod 777 '+portName)



def testKbir_Callback(msg):
    sendArduino('i')
    
def pres_Callback(msg):
    global pres 
    pres=msg.data
    print(pres)
    
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

def fide2i():
    x=Int32()
    x.data=1
    vitrine_pub.publish(x)
def RKO():
    x=Int32()
    x.data=2
    vitrine_pub.publish(x)
def erwin():
    x=Int32()
    x.data=3
    vitrine_pub.publish(x) 
def gta():
    x=Int32()
    x.data=4
    vitrine_pub.publish(x)
def horn():
    x=Int32()
    x.data=5
    vitrine_pub.publish(x)
def confirm_Callback(msg):
    global color
    global strategy
    print("waaaaa")
    
    
    if color=="Violet" and strategy==1:               
        #strategy1V()
        #scoreToScreen(100)
        horn()
        print("horn")
        # time.sleep(1)
        Lanci()
        #erwin()
        sequenceStatuette_1() 
        sequence_carre_fouille_1()
        go(185+150+300,-300,500)
        sendArduino('h')
        go(-100,-1250,500)
        Orientate(500,180)
        sendArduino("w")
        # after_fouille()

    elif color=="Violet" and strategy==2:
                    horn()
                    print("horn")
                    Lanci()
                    sequenceKa3bet()
                    sequence_carre_fouille_2()
                    after_fouille()
                       

                

    elif color=="Violet" and strategy==3:
                   
        strategy3V()
        scoreToScreen(300)

              
                

                

    elif color=="Violet" and strategy==4:
                  
        strategy4V()
        scoreToScreen(400)

    elif color=="Violet" and strategy==10:
        sequence_carre_fouille_1()

                

    # play(color,strategy)

def scoreToScreen(score):
    x=Int32()
    x.data=int(score)
    score_pub.publish(x)
    

rospy.init_node('SequenceSghir')
rospy.wait_for_service('/StmCommandSghir')
rospy.wait_for_service('/StmPoseSghir')
stmService = rospy.ServiceProxy('/StmCommandSghir',ApplyJointEffort)
stmPose = rospy.ServiceProxy('/StmPoseSghir',GetLightProperties)


testKbir=rospy.Subscriber("/testKbir",Int8,testKbir_Callback,queue_size=100)
colorSub=rospy.Subscriber("/color",Int8,color_Callback,queue_size=100)
strategySub=rospy.Subscriber("/strategy",Int8,strategy_Callback,queue_size=100)
confirmSub=rospy.Subscriber("/confirm",Bool,confirm_Callback,queue_size=100)
score_pub=rospy.Publisher("/score",Int32,queue_size=100)
vitrine_pub=rospy.Publisher("/vitrine",Int32,queue_size=100)
presSub=rospy.Subscriber("/presenceAruco",Int32,pres_Callback,queue_size=100)



# inGo_sub=rospy.Subscriber("/ingo",Bool,inGo_calllback,queue_size=100)



print("initializing")

def Lanci():
    req=ApplyJointEffortRequest()
    req.joint_name="Lanci"
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
    time.sleep(0.1)
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
    time.sleep(0.1)

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
    time.sleep(0.1)
def Asta3_90(speed,distance):
    req=ApplyJointEffortRequest()
    if(distance<0):
        req.start_time.secs=1
    req.joint_name="Asta3_90"
    req.effort=speed
    req.duration.secs=abs(distance)
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)
    time.sleep(0.1)


def go(x,y,speedF):
    # global waitInGO
    #evitementCmd.publishGoal(int(x),int(y),int(speedF))
    
    
    
    
    
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
    time.sleep(0.1)
    # print("waitt"+str(waitInGO))
    # while waitInGO==True:
        # print("waiting seq")
        
        
    # test=False
    # while test==False:
    #     try:
    #         xx=GetLightPropertiesRequest()
    #         xx.light_name=""
    #         if(evitementCmd.nemchi==False):
    #             resp=stmPose(xx)
    #             print(resp)          
    #             if(abs(int(resp.attenuation_constant)-int(x))<30 and abs(int(resp.attenuation_linear)-int(y))<30):
    #                 if(abs(int(resp.attenuation_constant)-int(x))<10 and abs(int(resp.attenuation_linear)-int(y))<10):
    #                     test==True
    #                     print(abs(int(resp.attenuation_linear)-int(y)))
    #                     break
    #                 else:
    #                     go(x,y,speedF)
                        
    #     #   print("eeeeeeeeeeee"+str(trueFlag))
    #     #   if trueFlag==True:#tetsalaaaaaaaaaa7
    #     #       x=Bool()
    #     #       x.data=False
    #     #       inGoCmd.publish(x)
    #     except rospy.ServiceException as e:
    #         print("Service call failed: %s"%e)  
    # print("Doneeeeeeeeeeeeeeeeeeeeee")  

def reset():
    req=ApplyJointEffortRequest()

    req.joint_name="Reset"
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)
        
def curv(x,y,phi,speed):
    req=ApplyJointEffortRequest()
    req.joint_name="Curv"
    req.effort=speed
    req.duration.secs=abs(x)
    req.duration.nsecs=abs(y)
    req.start_time.secs=int(abs(phi)*pi/180)
    if(phi<0):
        req.start_time.nsecs=1
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
        print("Update position call failed: %s"%e)     

def UpdateX(x):
    req=ApplyJointEffortRequest()
    req.joint_name="UpdateX"
    req.duration.secs=x
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Update position call failed: %s"%e)     

def UpdateY(y):
    req=ApplyJointEffortRequest()
    req.joint_name="UpdateY"
    req.duration.nsecs=y
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Update position call failed: %s"%e)     

def UpdateAngle(angle):
    req=ApplyJointEffortRequest()
    req.joint_name="UpdatePhi"
    req.effort=angle
    try:
        resp=stmService(req)
        # rospy.spin()
        print(resp)
    except rospy.ServiceException as e:
        print("Update position call failed: %s"%e)     




def sendArduino(seq):
        try:
           arduino.flush()
           arduino.write(seq.encode("UTF-8"))
        except KeyboardInterrupt:
            print("KeyboardInterrupt has been caught.")


def sequenceStatuette_1():
    go(335,1595,500)
    Orientate(250,-45)
    Asta3(200,-500)
    sendArduino("y")
    time.sleep(1.5)
    Move(500,150)
    sendArduino("Y")
    sendArduino("q")
    time.sleep(2)
    sendArduino("e")
    Move(400,150)
    Orientate(250,135)
    sendArduino("h")
    Move(200,150)
    Asta3(200,500)
    sendArduino("H")  
    Move(400,-150)
    sendArduino("e")
    Asta3(200,500)
    sendArduino("H") 
    time.sleep(0.5)
    sendArduino("w") 
    sendArduino("e")
    sendArduino("a")
    time.sleep(2.5)
    Move(500,-150)
    sendArduino("e")


    go(280,200,500)
    Orientate(300,-90)
    sendArduino('U')
    Asta3(200,300)
    UpdateY(75)
    UpdateAngle(-90)
    Move(200,-30)
    sendArduino("A")
    time.sleep(1)
    sendArduino("o")
    sendArduino("Y")
    Move(200,-350)
    go(280,420,300)
    Orientate(300,0)
    Asta3(200,-500)
    UpdateX(75)
    UpdateAngle(0)
    sendArduino("j")
    time.sleep(1)
    Move(500,230)
    Orientate(200,90)
    Move(500,600)
    Orientate(200,0)
    sendArduino("J")
    time.sleep(2)


def sequence_carre_fouille_2Strategy2():
    
    
   go(500,1250,500)
   Orientate(250,90)
   go(500,1850,500)
   
   
#    go(480,1250,500)
#    Orientate(250,90)
#    go(480,1850,500)
  
   Orientate(250,90)
   Asta3_90(200,500)


   Move(500,-90)
   Orientate(250,0)
   Orientate(250,0)
   sendArduino('g')
   c=arduino.read().decode()
   time.sleep(0.7)
   sendArduino('G')


   go(185,-65,200)
   Orientate(250,0)
   sendArduino('f')
   time.sleep(0.3)
   sendArduino('F')
   Orientate(250,0)
   while(not((arduino.read().decode()=="V")|(arduino.read().decode()=="J")|(arduino.read().decode()=="R"))):
        print("waiting for serial")
  
   if (c=="V"):
     go(185*3,-65,200)
     Orientate(250,0)
     sendArduino('g')
     time.sleep(0.7)
     sendArduino('G')
     Orientate(250,0)
     c=arduino.read().decode()
     if (c=="V"):
         go(185*6,-60,200)
         Orientate(250,0)
         sendArduino('f')
         time.sleep(0.7)
         sendArduino('F')
         Orientate(250,0)
     elif (c=="J"):
         go(185*4,-60,200)
         Orientate(250,0)
         sendArduino('f')
         time.sleep(0.7)
         sendArduino('F')
         Orientate(250,0)
   elif (c=="R"):
     go(185*2,-65,200)
     Orientate(250,0)
     sendArduino('f')
     time.sleep(0.7)
     sendArduino('F')
     Orientate(250,0)
     
     go(185*3,-65,200)
     Orientate(250,0)
     sendArduino('g')
     time.sleep(0.7)
     sendArduino('G')
     Orientate(250,0)
     c=arduino.read().decode()
     if (c=="V"):
         go(185*6,-50,200)
         Orientate(250,0)
         sendArduino('f')
         time.sleep(0.7)
         sendArduino('F')
         Orientate(250,0)
     elif (c=="J"):
         go(185*4,-55,200)
         Orientate(250,0)
         sendArduino('f')
         time.sleep(0.7)
         sendArduino('F')
         Orientate(250,0) 
         
         go(185*5,-55,200)
         Orientate(250,0)
         sendArduino('f')
         time.sleep(0.7)
         sendArduino('F')
         Orientate(250,0)
         
   #eli baadou tekmla taa le9dim
   go(185*5,-70,200)
   Orientate(250,0)
   sendArduino('f')
   time.sleep(0.7)
   sendArduino('F')
   Orientate(250,0)


   go(185*3,-70,200)
   Orientate(250,0)
   sendArduino('g')
   time.sleep(0.7)
   sendArduino('G')
   Orientate(250,0)


   go(185*4,-65,200)
   Orientate(250,0)
   sendArduino('g')
   time.sleep(0.7)
   sendArduino('G')
   Orientate(250,0)


   go(185*5,-60,200)
   Orientate(250,0)
   sendArduino('g')
   time.sleep(0.7)
   sendArduino('G')
   Orientate(250,0)

   
   go(185*6,-40,200)
   Orientate(250,0)
   sendArduino('g')
   time.sleep(0.7)
   sendArduino('G')

def sequenceStatuette():
    
    Move(500,-150)
    sendArduino("H")

    Asta3(200,500)
    sendArduino("a")
    time.sleep(2)
    Move(500,-150)
    sendArduino("e")
    Orientate(300,-45)
    Asta3(200,-500)
    sendArduino("y")
    time.sleep(1.3)
    Move(500,200)
    sendArduino("Y")
    go(350,650,500)
    Orientate(400,0)

    sendArduino("1")
    time.sleep(2)
    Orientate(400,-90)
    sendArduino("2")
    time.sleep(1)
    # RKO()
    #time.sleep(2.5)
    #Move(500,200)
    go(350,400,500)
    sendArduino('o')
    Orientate(400,0)
    Asta3(200,-500)
    UpdateX(75)
    UpdateAngle(0)
    sendArduino("j")
    time.sleep(1)
    Move(500,175)
    go(250,300,500)
    Orientate(400,-90)
 
    #Orientate(300,-90)
    #go(225,120,300)
    #Asta3(200,500)
    #Move(500,-30)
    #Orientate(300,-90)
    
    
    sendArduino('U')
    Asta3(200,500)
    UpdateY(75)
    UpdateAngle(-90)
    Move(200,-30)
    sendArduino("A")
    time.sleep(1.5)
    #gta()
    Move(200,-550)
    sendArduino("J")
    time.sleep(2)
    # Move(200,-200)
    Move(200,100)
    go(610,700,600)
    '''Orientate(200,0)
    Asta3(200,500)
    UpdateX(0)
    Move(200,200)
    #go(225,500,500)
    Orientate(400,0)
    go(673,500,500)
    Orientate(400,90)'''

def sequenceKa3bet():
    # go(645,1835,250)
    # Orientate(250,0)
    # sendArduino("T")
    # time.sleep(0.5)
    # sendArduino("t")
    # time.sleep(0.5)
    # sendArduino("w")
    # exit(0)

    # sendArduino("H")
    	
    #curv(258+140, 915+140, 0, 500)
    go(258,915+160, 800)
    go(1260,915+160,1200)
    go(1328,1233,500)
    sendArduino("h")
    # sendArduino("L")
    go(300,1550,400)
    
    Orientate(250,135)
    Asta3(200,500)
    #UpdateAngle(135)
    sequenceStatuette()
    
    
def fouilleFrom2():
    global color
    if color=="Violet":
           co='g'
    else:
       co='G'
    go(185,-70,200)
    Orientate(250,0)
    sendArduino('f')
    time.sleep(0.3)
    sendArduino('F')
    Orientate(250,0)


    go(185*2,-70,200)
    Orientate(250,0)
    sendArduino(co)
    time.sleep(0.7)
    sendArduino('G')
    Orientate(250,0)


    go(185*3,-70,200)
    Orientate(250,0)
    sendArduino(co)
    time.sleep(0.7)
    sendArduino('G')
    Orientate(250,0)


    go(185*4,-65,200)
    Orientate(250,0)
    sendArduino(co)
    time.sleep(0.7)
    sendArduino('G')
    Orientate(250,0)


    go(185*5,-60,200)
    Orientate(250,0)
    sendArduino(co)
    time.sleep(0.7)
    sendArduino('G')
    Orientate(250,0)

    
    go(185*6,-40,200)
    Orientate(250,0)
    sendArduino(co)
    time.sleep(0.7)
    sendArduino('G')    

def fouilleFrom5():
    global color
    if color=="Violet":
           co='g'
    else:
       co='G'
    go(185*4,-65,200)
    Orientate(250,0)
    sendArduino(co)
    time.sleep(0.7)
    sendArduino('G')
    Orientate(250,0)


    go(185*5,-60,200)
    Orientate(250,0)
    sendArduino(co)
    time.sleep(0.7)
    sendArduino('G')
    Orientate(250,0)

    
    go(185*6,-40,200)
    Orientate(250,0)
    sendArduino(co)
    time.sleep(0.7)
    sendArduino('G')   
    
    
def sequence_carre_fouille():
   global color
   if color=="Violet":
           co='g'
   else:
       co='G'
   go(610,1250,500)
   Orientate(250,90)
   go(610,1850,500)

   Orientate(250,90)
   Asta3_90(200,500)


   Move(500,-90)
   Orientate(250,0)
   Orientate(250,0)
   sendArduino(co)
   time.sleep(0.7)
   sendArduino('G')


   go(185,-70,200)
   Orientate(250,0)
   sendArduino('f')
   time.sleep(0.3)
   sendArduino('F')
   Orientate(250,0)


   go(185*2,-70,200)
   Orientate(250,0)
   sendArduino(co)
   time.sleep(0.7)
   sendArduino('G')
   Orientate(250,0)


   go(185*3,-70,200)
   Orientate(250,0)
   sendArduino(co)
   time.sleep(0.7)
   sendArduino('G')
   Orientate(250,0)


   go(185*4,-65,200)
   Orientate(250,0)
   sendArduino(co)
   time.sleep(0.7)
   sendArduino('G')
   Orientate(250,0)


   go(185*5,-60,200)
   Orientate(250,0)
   sendArduino(co)
   time.sleep(0.7)
   sendArduino('G')
   Orientate(250,0)

   
   go(185*6,-40,200)
   Orientate(250,0)
   sendArduino(co)
   time.sleep(0.7)
   sendArduino('G')    
      
 
def rebelote():
   global color
   if color=="Violet":
       co='r'
   else:
       co='R'
   global fouille1
   global fouille2
   global find1
   global find2
   sendArduino(co)
   while True:
            global fouille1
            global fouille2
            global find1
            global find2
            c=arduino.read().decode()
            if c=="V":
                print("vvvvvvvvvv")
                if color=="Violet":
                    fouille1=True
                    find1=True
                    break
                else :
                    fouille1=False
                    find1=True
                    break
            if c=="J":
                print("jjjjjjjjjjjjj")
                if color=="Yellow":
                    fouille1=True
                    find1=True
                    break
                else :
                    '''fouille1=False
                    find1=True'''
                    rebelote()
                    break
                    
            if c=="R":
                print("rrrrrrrrrrrrr")

                fouille1=False
                find1=True
                break
            if c=="N":
                # print("nnnnnnnnnn")
                Move(200,20)
                Orientate(200,0)
                find1=False
                rebelote()
                break 
            print("waaaa")
            print(find1)    

def rebelote2():
   global color
   if color=="Violet":
       co='r'
   else:
       co='R'
   global fouille1
   global fouille2
   global find1
   global find2
   sendArduino(co)
   while True:
            c=arduino.read().decode()
            if c=="V":
                print("vvvvvvvvvv")
                if color=="Violet":
                    fouille2=True
                    find2=True
                    break
                else :
                    fouille2=False
                    find2=True
                    break
            if c=="J":
                print("jjjjjjjjjjjjj")
                if color=="Yellow":
                    fouille2=True
                    find2=True
                    break
                else :
                    '''fouille1=False
                    find1=True'''
                    rebelote2()
                    break
                    
            if c=="R":
                print("rrrrrrrrrrrrr")

                fouille2=False
                find2=True
                break
            if c=="N":
                # print("nnnnnnnnnn")
                Move(200,20)
                Orientate(200,0)
                find2=False
                rebelote2()
                break    
            
def sequence_carre_fouille_1():
   global color
   if color=="Violet":
       co='r'
   else:
       co='R'
   global fouille1
   global fouille2
   global find1
   global find2
   
   go(590,1250,500)
   Orientate(250,90)
   go(590,1850,500)
   #Asta3(200,-1000)
   #Move(500,510)
   Orientate(250,90)
   Asta3_90(200,500)


   Move(500,-90)
   Orientate(250,0)
   Orientate(250,0)
   
   
   rebelote()
   
 
   print(find1)     
   if find1==False:
        fouilleFrom2()
   else:
       go(185,-80,200)
       Orientate(250,0)
       sendArduino('f')
       time.sleep(0.3)
       sendArduino('F')
       
       if fouille1==False:
           go(185*2,-80,200)
           Orientate(250,0)
           sendArduino('f')
           time.sleep(0.3)
           sendArduino('F')
       go(185*3,-80,200)
       Orientate(250,0)
    #    sendArduino(co)
       
       rebelote2()
       
       if find2==False:
           fouilleFrom5()
       else:
           if fouille2==True:
                go(185*6,-50,200)
                Orientate(250,0)
                sendArduino('f')
                time.sleep(0.3)
                sendArduino('F')
           else:
                go(185*4,-65,200)
                Orientate(250,0)
                sendArduino('f')
                time.sleep(0.3)
                sendArduino('F')
                go(185*5,-60,200)
                Orientate(250,0)
                sendArduino('f')
                time.sleep(0.3)
                sendArduino('F')
                                 
def after_fouille():
    if ((pres / 10) % 10)>0:
       go(185*8,-450,500)
       sendArduino("h")
       go(185*8-1870,-450-577,500)
       Asta3(200,100)
    else:
       go(185+150,-400,500)
       Orientate(250,-90)        
   
def sequence_carre_fouille_2():
   global color
   if color=="Violet":
       co='r'
   else:
       co='R'
   global fouille1
   global fouille2
   global find1
   global find2
   
   go(610,1250,500)
   Orientate(250,90)
   go(610,1850,500)
   #Asta3(200,-1000)
   #Move(500,510)
   Orientate(250,90)
   Asta3_90(200,500)


   Move(500,-90)
   Orientate(250,0)
   Orientate(250,0)
   
   
   rebelote()
   
 
   print(find1)     
   if find1==False:
        fouilleFrom2()
   else:
       go(185,-80,200)
       Orientate(250,0)
       sendArduino('f')
       time.sleep(0.3)
       sendArduino('F')
       
       if fouille1==False:
           go(185*2,-80,200)
           Orientate(250,0)
           sendArduino('f')
           time.sleep(0.3)
           sendArduino('F')
       go(185*3,-80,200)
       Orientate(250,0)
    #    sendArduino(co)
       
       rebelote2()
       

    
       if find2==False:
           fouilleFrom5()
       else:
           if fouille2==True:
                go(185*6,-50,200)
                Orientate(250,0)
                sendArduino('f')
                time.sleep(0.3)
                sendArduino('F')
           else:
                go(185*4,-65,200)
                Orientate(250,0)
                sendArduino('f')
                time.sleep(0.3)
                sendArduino('F')
                go(185*5,-60,200)
                Orientate(250,0)
                sendArduino('f')
                time.sleep(0.3)
                sendArduino('F')
     
           
def strategy1V():
    go(260,700,250)
def strategy2V():
    go(260,800,300)
def strategy3V():
    go(260,900,350)
def strategy4V():
    go(260,1000,400)



# go(500,1400,300)
# go(1400,1400,300)
# go(1500,1300,300)

        

print("Begining..............")


with serial.Serial(portName, 9600, timeout=3,bytesize=serial.EIGHTBITS) as arduino:
    while not arduino.isOpen():
        print("Waiting for arduino Serial")
    print("{} connected!".format(arduino.port))
    try:
       print("in spin")
       reset()
       Move(200,-10)
       Orientate(200,90)
       sendArduino("i")
       #sequence_carre_fouille_2()
       #exit(0) 
       rospy.spin()
       print("after spin")
              
    except KeyboardInterrupt:
        print("KeyboardInterrupt has been caught.")
 
def strategy1V():
    go(260,700,250)
def strategy2V():
    go(260,800,300)
def strategy3V():
    go(260,900,350)
def strategy4V():
    go(260,1000,400)



        

# print("Begining..............")


# with serial.Serial(portName, 9600, timeout=3,bytesize=serial.EIGHTBITS) as arduino:
#     while not arduino.isOpen():
#         print("Waiting for arduino Serial")
#     print("{} connected!".format(arduino.port))
#     try:
#        print("in spin")
#        reset()
#        sendArduino("i")
#        #sequence_carre_fouille()
#        rospy.spin()
#        print("after spin")
              
#     except KeyboardInterrupt:
#         print("KeyboardInterrupt has been caught.")

