#!/usr/bin/env python3
import numpy as np
import rospy
import tf
from tf.msg import tfMessage
import math as m
from std_msgs.msg import Int16

listener = 0
presence = 0

distributeurUn = {
    "xMin": 1,
    "xMax": 2,
    "yMin": 2,
    "yMax": 0
}

distributeurDeux = {
    "xMin": 10,
    "xMax": 11,
    "yMin": 12,
    "yMax": 00
}

def checkDistributeurUn(trans):
    if trans[0] > distributeurUn["xMin"] and trans[0] < distributeurUn["xMax"] and trans[1] > distributeurUn["yMin"] and trans[1] < distributeurUn["yMax"]:
        return 10
    else: 
        return 0

def checkDistributeurDeux(trans):
    if trans[0] > distributeurDeux["xMin"] and trans[0] < distributeurDeux["xMax"] and trans[1] > distributeurDeux["yMin"] and trans[1] < distributeurDeux["yMax"]:
        return 1
    else: 
        return 0

def detectionDistributeurs():
    rospy.init_node('detectDistributeurs')
    global listener
    listener = tf.TransformListener()
    rospy.Subscriber("/detectionDistributeurCamera/image_raw/tf_list", tfMessage, detect)
    pub = rospy.Publisher('presenceDistributeur', Int16, queue_size=10)
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        pub.publish(presence)
        rate.sleep()

def detect(data):
    global listener
    global presence
    presence = 0
    for t in data.transforms:
        if "marker" in t.child_frame_id:
            try:
                    (trans, rot) = listener.lookupTransform(
                        '/map', t.child_frame_id, rospy.Time(0))
            except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
                    continue
            presence += checkDistributeurUn(trans)
            presence += checkDistributeurDeux(trans)
    return presence
    
if __name__ == '__main__':
    try:
        detectionDistributeurs()
    except rospy.ROSInterruptException:
        pass
