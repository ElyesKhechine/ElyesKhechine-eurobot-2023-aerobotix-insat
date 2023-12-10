
import time
import rospy
from gazebo_msgs.srv import ApplyJointEffort
from gazebo_msgs.srv import ApplyJointEffortRequest
from geometry_msgs.msg import Point
from std_msgs.msg import Bool


# trueFlag=True

nemchi= False

def do(msg):
      global nemchi
      nemchi=True
      try:
        x=ApplyJointEffortRequest()
        x.joint_name="Go"
        x.effort=abs(int(msg.z))
        x.duration.secs=abs(int(msg.x))
        x.duration.nsecs=abs(int(msg.y))
        if(msg.x<0):
              x.start_time.secs=1
        if(msg.y<0):
              x.start_time.nsecs=1
                  
        resp=stmService(x)
        print(resp)
        nemchi=False
        
      #   print("eeeeeeeeeeee"+str(trueFlag))
      #   if trueFlag==True:#tetsalaaaaaaaaaa7
      #       x=Bool()
      #       x.data=False
      #       inGoCmd.publish(x)
      except rospy.ServiceException as e:
        print("Service call failed: %s"%e)

def callback_trueFlag(msg):
      global trueFlag
      trueFlag=msg.data



try:
  # rospy.init_node('EVitementCmd2', anonymous=True)
  cmd_sub=rospy.Subscriber("/evitementCmd",Point,do,queue_size=100)
  trueFlag_sub=rospy.Subscriber("/evitement",Bool,callback_trueFlag,queue_size=100)
  goal=rospy.Publisher("/currentGoal",Point,queue_size=100)
#   inGoCmd=rospy.Publisher("/ingo",Bool,queue_size=100)

  
  rospy.wait_for_service('/StmCommand')
  stmService = rospy.ServiceProxy('/StmCommand',ApplyJointEffort)
#   rospy.spin()
except rospy.ROSInterruptException:
    print('ROS Interrupt Exception occured!')

def publishGoal(x,y,speed):
    global goal
    p=Point()
    p.x=x
    p.y=y
    p.z=speed
    goal.publish(p)

