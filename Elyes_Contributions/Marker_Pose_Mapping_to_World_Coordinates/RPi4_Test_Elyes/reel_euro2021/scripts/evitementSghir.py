
import math
from math import pi
import rospy
import time 
from sensor_msgs.msg import LaserScan 
from std_msgs.msg import Bool
from geometry_msgs.msg import Point
from gazebo_msgs.srv import GetLightProperties
from gazebo_msgs.srv import GetLightPropertiesRequest
from gazebo_msgs.srv import GetLightPropertiesResponse



from reel_euro2021.msg import ta7wil
import evitementCmdSghir

sending=False


timer=0
blocked=False

GoalX=0
GoalY=0
GoalZ=0

XPos=0
YPos=0
TetaPos=0
VxPos=0
VyPos=0
flag=True

# print("aa")  
def sendPoint(xx,yy,speed):
      x=Bool()
      x.data=True
      sending_pub.publish(x)
      start_pub()
      # while flag==False:
      #       start_pub()
      #       print("flag")
      #time.sleep(2)
      # print(str(xx)+"   "+str(yy))
      
      p=Point()
      p.x=int(xx)
      p.y=int(yy)
      p.z=int(speed)
      
      evitement_cmdSghir.publish(p)

def cancel_pub():
      
      x=Bool()
      x.data=False
      # if evitementCmd.nemchi==False:
      evitement_publisherSghir.publish(x)
      # x.data=True
      # inGo.publish(x)
def start_pub():
      x=Bool()
      x.data=True
      evitement_publisherSghir.publish(x)
def LinearPointX(teta,dist,xx):
      x1=math.sin(teta)*dist
      return xx+x1
def LinearPointY(teta,dist,yy):
      y1=math.cos(teta)*dist 
      return yy+y1
             
def tsaref():
      global XPos
      global YPos
      global TetaPos
      teta=TetaPos
      
      
      # Pose
      req=GetLightPropertiesRequest()
      req.light_name="getPoint"
      try:
            resp=StmPoseSrvSghir(req)
            XPos=resp.attenuation_constant;
            YPos=resp.attenuation_linear;
            TetaPos=resp.attenuation_quadratic;
            print(str(XPos)+" "+str(YPos)+"  " +str(TetaPos))
      except rospy.ServiceException as e:
            print("Service call failed: %s"%e)
      
      # Pose
      # evitementCmd.nemchi=True
      blocked=False
      print("netsarreeeeeeeeef")


      # ne5taaar l point

      print(str(XPos)+"    "+str(YPos))
      
      x2=400*math.sin(TetaPos)
      y2=400*math.cos(TetaPos)

      a=True
      b=True
      # point A
      p1x=float(XPos+x2)
      p1y=float(YPos-y2)
      # point B
      p2x=float(XPos-x2)
      p2y=float(YPos+y2)
      print(str(p1x)+"   "+str(p1y))
      print(str(p2x)+"   "+str(p2y))

      if p1x < 250 or p1x > 2200 or p1y <250 or p1y > 1750:
            a=False
      if p2x < 250 or p2x > 2200 or p2y <250 or p2y > 1750:
            b=False
      if a==True and b==False:
            print("1")
            print(str(p1x)+"   "+str(p1y))
            sendPoint(int(p1x),int(p1y),350)
            # print("sentt")
            # while True:
            #       print(evitementCmd.nemchi)
            # while evitementCmd.nemchi==True:
            #       print("waiting")
            # sendPoint(int(LinearPointX(teta,200,p1x)),int(LinearPointY(teta,200,p1y)),350)
            
      elif a==False and b==True:
            print("2")
            
            print(str(p2x)+"   "+str(p2y))
            
            sendPoint(p2x,p2y,350)
            print("sentt")
            
            # while True:
            #       print(evitementCmd.nemchi)
            # while evitementCmd.nemchi==True:
            #       print("waiting")
            # sendPoint(int(LinearPointX(teta,200,p2x)),int(LinearPointY(teta,200,p2y)),350)
            
      elif a==True and b==True:
            d1=math.sqrt((XPos-p1x)*(XPos-p1x)+(YPos-p1y)*(YPos-p1y))
            d2=math.sqrt((XPos-p2x)*(XPos-p2x)+(YPos-p2y)*(YPos-p2y))
            print("twooooo possibilities")
            print("twooooo possibilities")
            print("twooooo possibilities")
            print("twooooo possibilities")
            print("twooooo possibilities")
            if d1<d2:
                  sendPoint(p1x,p1y,350)
                  print("A")
                  # while evitementCmd.nemchi==True:
                  #     print("waiting")
                  # sendPoint(int(LinearPointX(teta,200,p1x)),int(LinearPointY(teta,200,p1y)),350)
            
            else:
                  sendPoint(p2x,p2y,350)
                  print("B")
                  # while evitementCmd.nemchi==True:
                  #     print("waiting")
                  # sendPoint(int(LinearPointX(teta,200,p2x)),int(LinearPointY(teta,200,p2y)),350)
            


      else:
            print("Stop")
            print("Stop")
            print("Stop")
            print("Stop")
            print("Stop")
      # ne5taaar l point

      # sendPoint(GoalX,GoalY,GoalZ)


def true_Callback(msg):
      global flag
      flag=msg.data
      # print(str(flag)+"               callback")

def republish(xx,yy,zz):
      x=Point()
      x.x=xx
      x.y=yy
      x.z=zz
      evitement_cmdSghir.publish(x)
def currentGoal_callback(msg):
      global GoalX
      global GoalY
      global GoalZ

      GoalX=msg.x
      GoalY=msg.y
      GoalZ=msg.z

def sending_callback(msg):
      global sending
      sending=msg.data
# def pose_callback(msg):
#       # print(msg)
#       global XPos
#       global YPos
#       global TetaPos
#       global VxPos
#       global VyPos
#       XPos=msg.x
#       YPos=msg.y
#       TetaPos=msg.teta
#       VxPos=msg.vx
#       VyPos=msg.vy
      # print(str(XPos)+"      "+str(YPos))
    



def laser_Callback(msg):    
    y=LaserScan()
    y.header=msg.header
    y.angle_increment=msg.angle_increment
    y.angle_max=msg.angle_max
    y.angle_min=msg.angle_min
    y.range_max=msg.range_max
    y.range_min=msg.range_min
    y.scan_time=msg.scan_time
    y.time_increment=msg.time_increment
    y.intensities=msg.intensities
    




    #y=msg
    #y.ranges=[]
    
    global timer
    global blocked
    i=0
    #timer=timer+1
    obstacle=False
    for range in msg.ranges:
      global blocked
      ang=msg.angle_min+i*msg.angle_increment*180/pi
      if  ang>=180-float(angleSghir)/2 and ang <= 180+float(angleSghir)/2:
            global blocked
            # print(ang)
            # r=Float32()
            # r.data=5.0
            # y.ranges.append(5)
            # y.ranges[i].data=5.0
            
            if(range<float(distanceSghir)/1000):
                  global blocked
                  global sending

                  # if blocked==False:
                  
                  
                  obstacle=True
                  if blocked==False:
                        timer=0
                        blocked=True
                  if sending==False:
                      cancel_pub()
                  break

                  
                 

                  # rospy.loginfo("Detected crucial OBSTACLE < "+str(float(distance))+"mm ====>  "+str(range))            
      # else:
            # r=Float32()
            # r.data=10
            # y.ranges.append(10)
            # y.ranges[i].data=5.0
    
      i=i+1

#     rospy.loginfo("Timer= "+str(timer)) 
    if obstacle==True and blocked==True:
          timer=timer+1   
      #     if sending==False:
      #             cancel_pub()
    if obstacle==False and blocked==True:
          timer=0
          global currentGoal
          blocked=False
          sendPoint(int(GoalX),int(GoalY),int(GoalZ))  
      #     print(str(GoalX)+"        "+str(GoalY))
      #     print(str(GoalX)+"        "+str(GoalY))
      #     print(str(GoalX)+"        "+str(GoalY))
      #     print(str(GoalX)+"        "+str(GoalY))

    if(timer>float(delaySghir)):
          timer=0
          tsaref()  
     


                  




distanceSghir=rospy.get_param("~distanceSghir", default="400")
delaySghir=rospy.get_param("~delaySghir", default="15")
angleSghir=rospy.get_param("~angleSghir", default="15")
    
rospy.loginfo("Distance Param == "+str(distanceSghir))
rospy.loginfo("Delay Param == "+str(delaySghir))
rospy.loginfo("Angle Param == "+str(angleSghir))   



try:
  rospy.init_node('evitementStmSghir', anonymous=True)
  laser_sub=rospy.Subscriber("/scanSghir",LaserScan,laser_Callback,queue_size=1000)
  true_sub=rospy.Subscriber("/evitementSghir",Bool,true_Callback,queue_size=100)

  currentGoal_sub=rospy.Subscriber("/currentGoalSghir",Point,currentGoal_callback,queue_size=100)
#   pose_sub=rospy.Subscriber("/ta7wil",ta7wil,pose_callback,queue_size=10000)
  evitement_publisher=rospy.Publisher("/evitementSghir",Bool,queue_size=100)
  evitement_cmd=rospy.Publisher("/evitementCmdSghir",Point,queue_size=100)
#   inGo=rospy.Publisher("/ingo",Bool,queue_size=100)
  sending_pub=rospy.Publisher("/sendingSghir",Bool,queue_size=100)
  sending_sub=rospy.Subscriber("/sendingSghir",Bool,sending_callback,queue_size=100)
  
  
  
  rospy.wait_for_service('/StmPoseSghir')
  StmPoseSrvSghir = rospy.ServiceProxy('/StmPoseSghir',GetLightProperties)
#   print("done")
  # rospy.wait_for_service('/StmCommand')
  # stmService = rospy.ServiceProxy('/StmCommand',ApplyJointEffort)
  # scan_publisher=rospy.Publisher("/scan2",LaserScan,queue_size=100)
  rospy.spin()
except rospy.ROSInterruptException:
    print('ROS Interrupt Exception occured!')

