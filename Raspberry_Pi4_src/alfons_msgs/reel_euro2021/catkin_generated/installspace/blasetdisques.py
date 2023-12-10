#!/usr/bin.env python3

import rospy
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
from reel_euro2021.msg import IntList
from std_msgs.msg import String
import time
from alfons_msgs.msg import ArucoInfo
import numpy as np
still = True
global t0 
t0 = time.time()
disks=IntList()
disques = [0,0,0,0]
def aruco_callback(msg:ArucoInfo):
    global disques
    disques=[0,0,0,0]
    print("rani dkhalet")
    t=msg.header.stamp.secs
    x=msg.center_x_px
    y=msg.center_y_px
    id=msg.marker_ids
    #X=[a for a in x if (a>2400 or a<1000)]
    #Y=[b for b in y if (b>300)]
    print(id)
    print(x)
    print(y)    

    for i in id :
        index=id.index(i)
        if(int(i/10)==36 ) :
            if(2300<x[index]<2600 and 1200<y[index]<1500):
                disques[0]=1
            if(1400<x[index]<1570 and 1200<y[index]<1450):
                disques[1]=1 
        if(int(i/10)==13):
            if(3500<x[index]<3800 and 1700<y[index]<2002):
                disques[2]=1
            if(130<x[index]<380 and 1670<y[index]<1950):
                disques[3]=1
    print(disques)
    disks.data=disques
    disques_publisher.publish(disks)

rospy.init_node('blayes_disques')
aruco_sub = rospy.Subscriber("camera/image_raw/aruco_list",ArucoInfo,callback= aruco_callback)
disques_publisher=rospy.Publisher("/blayes_disques",IntList,queue_size=100)
if __name__ == '__main__':
    
    rospy.spin()
