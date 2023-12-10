
#!/usr/bin/env python
import math 
import rospy

# Brings in the SimpleActionClient
import actionlib
# Brings in the .action file and messages used by the move base action
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal

def movebase_client(x,y,teta):

   # Create an action client called "move_base" with action definition file "MoveBaseAction"
    client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
 
   # Waits until the action server has started up and started listening for goals.
    client.wait_for_server()

   # Creates a new goal with the MoveBaseGoal constructor
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "odom"
    goal.target_pose.header.stamp = rospy.Time.now()
    
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    goal.target_pose.pose.orientation.z = math.sin(teta*math.pi/360)
    goal.target_pose.pose.orientation.w = math.cos(teta*math.pi/360)
    print(x)
    print(y)
    print(teta)

                

   # Sends the goal to the action server.
    client.send_goal(goal)
   # Waits for the server to finish performing the action.
    wait = client.wait_for_result()
   # If the result doesn't arrive, assume the Server is not available
    if not wait:
        rospy.logerr("Action server not available!")
        rospy.signal_shutdown("Action server not available!")
    else:
    # Result of executing the action
       return client.get_result()   

# If the python node is executed as main process (sourced directly)
if __name__ == '__main__':
    while True:
        try:
            # Initializes a rospy node to let the SimpleActionClient publish and subscribe
            rospy.init_node('movebase_client_py')
            result = movebase_client(0.6,0,90)
            rospy.loginfo("1 Goal execution done!")
            rospy.sleep(2)
            result = movebase_client(0.6,0.5,180)
            rospy.loginfo("2 Goal execution done!")
            rospy.sleep(2)
            result = movebase_client(0.0,0.5,-90)
            rospy.loginfo("3 Goal execution done!")
            rospy.sleep(5)
            result = movebase_client(0.0,0,0)
            rospy.loginfo("Home Goal execution done!")
            if result:
                rospy.loginfo("Goal execution done!"+str(result))
        except rospy.ROSInterruptException:
            rospy.loginfo("Navigation test finished.")    	       
