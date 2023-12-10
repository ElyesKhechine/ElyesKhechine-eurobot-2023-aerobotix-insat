#!/usr/bin/env python3

from scripts.mission import Mission
import rospy
from std_msgs.msg import String
from reel_euro2021.msg import goal
missions=[]
pub_goal = rospy.Publisher('/better_goal', goal, queue_size=10)


def executeMission(name):
    m=Mission()
    for element in missions:
        if element.name==name:
            g=goal()
            g.x=element.x
            g.y=element.y
            g.teta=element.teta
            pub_goal.publish()
            
            
def callback(data):
    rospy.loginfo("My mission is now ==> %s", data.data)
    executeMission(data.data)
def mission_order():
    rospy.init_node('executing_kbir', anonymous=True)

    rospy.Subscriber("/mission_kbir", String, callback)

    rospy.spin()

if __name__ == '__main__':
    missions=Mission.initialize()
    mission_order()
    
