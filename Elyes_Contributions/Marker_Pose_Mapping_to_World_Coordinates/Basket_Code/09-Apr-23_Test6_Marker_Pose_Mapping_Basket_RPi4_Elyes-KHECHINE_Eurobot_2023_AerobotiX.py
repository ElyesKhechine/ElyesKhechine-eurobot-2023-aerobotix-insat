#!/usr/bin/env python3
import rospy
from std_msgs.msg import Int32
from std_msgs.msg import Int16MultiArray
import serial,time,os
import alfons_msgs
from alfons_msgs.msg import ArucoInfo
import cv2
import cv2.aruco as aruco
import numpy as np
import yaml

score = 0

def panierCallback(msg):
if msg.data==1:
sendArduino('e') #Command the basket's arduino to elevate
print("Start Elevation")

def arucoCallback(msg):
global x
global y
global ids

scss
Copy code
# Define Aruco dictionary and parameters
aruco_dict = aruco.Dictionary_get(aruco.DICT_4X4_50)
aruco_params = aruco.DetectorParameters_create()

# Load camera parameters
camera_params_file = "tutorial_camera_params.yml"
with open(camera_params_file, 'r') as f:
    camera_params_data = yaml.load(f)
camera_matrix = np.array(camera_params_data['camera_matrix']['data']).reshape((3, 3))
dist_coeffs = np.array(camera_params_data['distortion_coefficients']['data'])

x = msg.center_x_px
y = msg.center_y_px
ids = msg.marker_ids

for i in ids :
    index=ids.index(i)
    if(int(i/10)==47 ):
        if(0<x[index]<800 and 1100<y[index]<1650):
            Gobst(-750,700,500)
        if(0<x[index]<900 and 250<y[index]<750):
            Gobst(-750,1500,500)
        if(2600<x[index]<3250 and 250<y[index]<750):
            Gobst(750,1500,500)
        if(2800<x[index]<3800 and 1100<y[index]<1650):
            Gobst(750,700,500)

        # Calculate the pose of the marker relative to the camera
        corners, ids, _ = aruco.detectMarkers(frame, aruco_dict, parameters=aruco_params)

        if ids is not None and len(ids) > 0:
            rvecs, tvecs = None, None
            ret, rvecs, tvecs = cv2.solvePnP(obj_points, corners[0], camera_matrix, dist_coeffs, rvecs, tvecs)

            if ret:
                # Reverse the pose to get the position and orientation of the marker relative to the camera
                R, _ = cv2.Rodrigues(rvecs)
                R = R.T
                tvecs = -R.dot(tvecs)

                # Print the marker pose
                rospy.loginfo("Marker pose relative to the camera:")
                rospy.loginfo("Rotation: {}".format(R))
                rospy.loginfo("Translation: {}".format(tvecs))

print("x : " , x , "|    y : " , y , "|     id :  " , ids ) 
def sendArduino(seq):
try:
# x=input("aaaaaaaaaaa")
arduino.flush()
arduino.write(seq.encode("UTF-8"))
# arduino.write(x.encode("UTF-8"))
except KeyboardInterrupt:
print("KeyboardInterrupt has been caught.")

def listener():
rospy.init_node('paniereNode', anonymous=True)
print("waiting for panier elevation ...")
panierSub=rospy.Subscriber("/panier", Int32, panierCallback, queue_size=10)
arucoSub=rospy.Subscriber("/camera/image_raw/aruco_list",ArucoInfo ,arucoCallback,