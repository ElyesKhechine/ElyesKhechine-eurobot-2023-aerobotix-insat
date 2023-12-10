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
import sys
sys.path.append('/home/ubuntu/AeroBotiX_euro2021/src/aruco_detector_ocv/src')

from aruco_detector_ocv import arucoCallback, returnCorners


"""portName=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
while len(portName)<11:
    portScreen=os.popen('cat ~/AeroBotiX_euro2021/src/reel_euro2021/config/arduino.port').read()
    print('\033[2;41;41m Trying to connect to Arduino Please check the port')

print("The port of Arduino is " +portName)
os.system('sudo chmod 777 '+portName)

score = 0 
"""
def panierCallback(msg): 
    if msg.data==1:
    	sendArduino('e') #Command the basket's arduino to elevate
    	print("Start Elevation")
    return

def arucoCallback(msg):
    global x
    global y
    global ids
    global corners
    
    # Define Aruco dictionary and parameters
    aruco_dict = aruco.Dictionary_get(aruco.DICT_4X4_50)
    aruco_params = aruco.DetectorParameters_create()

    # Load camera parameters
    camera_params_file = "/home/ubuntu/.ros/camera_info/mehdi.yaml"
    with open(camera_params_file, 'r') as f: camera_params_data = yaml.load(f)
    camera_matrix = np.array(camera_params_data['camera_matrix']['data']).reshape((3, 3))
    dist_coeffs = np.array(camera_params_data['distortion_coefficients']['data'])
    marker_size = 0.07
    obj_points = np.zeros((4,3), dtype = np.float32)
    #obj_points[:,:2] = np.array[[0,0],[0,1],[1,1],[1,0],dtype=np.float32)
    obj_points = np.array([[-marker_size/2,marker_size/2,0],
                          [marker_size/2,marker_size/2,0],
                          [marker_size/2,marker_size/2,0],
                          [-marker_size/2,-marker_size/2,0]], dtype=np.float32)
    x=msg.center_x_px
    y=msg.center_y_px
    corners=aruco_detector_ocv.returnCorners()
    ids= msg.marker_ids
    
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
    
        rvecs, tvecs = None, None
        ret, rvecs, tvecs = cv2.solvePnP(obj_points, corners[0], camera_matrix, dist_coeffs, rvecs, tvecs)
        if ret:
            # Reverse the pose to get the position and orientation of the marker relative to the camera
            R, _ = cv2.Rodrigues(rvecs)
            R = R.T
            tvecs = -R.dot(tvecs)

            # Print the marker pose
            print("Marker pose relative to the camera:")
            print("Rotation: {}".format(R))
            print("Translation: {}".format(tvecs))
    print("x : " , x , "|    y : " , y , "|     id :  " , ids ) 


def sendArduino(seq):
        try:
        #    x=input("aaaaaaaaaaa")
           arduino.flush()
           arduino.write(seq.encode("UTF-8"))
        #    arduino.write(x.encode("UTF-8"))
        except KeyboardInterrupt:
            print("KeyboardInterrupt has been caught.")   
"""
we need
1/ Elevate the basket when needed : robot2 ---> panier : '1'
2/ Aruco detection and send coordinates : panier ----> Robot1 & Robot2
3/ Calculate final score : robot1 & robot2 ----> panier
4/ Coordinate the 2 robots in the play area
5/ Command the arduino UART DONE

score counter: 




aruco codes :

brown :  36 
yellow : 13
pink :	 47

area :  20    21



	22    23
	
opponent : blue team : 1-5
	   green team : 6-10

plates coords
  green:
  
"""
    
def listener():
    rospy.init_node('paniereNode', anonymous=True)
    #print("waiting for panier elevation ...")
    #panierSub=rospy.Subscriber("/panier", Int32, panierCallback, queue_size=10)
    arucoSub=rospy.Subscriber("/camera/image_raw/aruco_list",ArucoInfo ,arucoCallback,queue_size=100)
    rospy.spin()
    


if __name__ == '__main__':
    listener()
