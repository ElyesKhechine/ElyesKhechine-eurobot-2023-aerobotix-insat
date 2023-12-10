#!/usr/bin/env python

import rospy, math, tf
from nav_msgs import OccupancyGrid


def publish_obstacle_msg():
  pub = rospy.Publisher('/map', OccupancyGrid, queue_size=100)
  rospy.init_node("obstacle.py")
  
  grid=OccupancyGrid(0)
  r = rospy.Rate(10) # 10hz
  t = 0.0
  while not rospy.is_shutdown():
    

    pub.publish(obstacle_msg)
    
    r.sleep()


if __name__ == '__main__': 
  try:
    publish_obstacle_msg()
  except rospy.ROSInterruptException:
    pass
