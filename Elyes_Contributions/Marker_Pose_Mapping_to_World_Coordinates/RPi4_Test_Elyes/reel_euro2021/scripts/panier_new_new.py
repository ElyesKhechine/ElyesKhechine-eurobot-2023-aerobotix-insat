#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int32
from std_msgs.msg import Int16MultiArray
import serial,time,os
import alfons_msgs 
from alfons_msgs.msg import ArucoInfo
import numpy as np
import cv2

score = 0 

# Camera's position and rotation in the map frame
map_camera_pos = np.array([0, -100, 1050])
map_camera_rot = np.array([10, 0, 0])

# Camera's intrinsic matrix and distortion coefficients
camera_matrix = np.array([[fx, 0, cx], [0, fy, cy], [0, 0, 1]])
dist_coeffs = np.array([k1, k2, p1, p2, k3])

def panierCallback(msg): 
    if msg.data==1:
        sendArduino('e') 
		#Command the basket's arduino to elevate
        print("Start Elevation")


def camera_to_map(camera_coords):
    # Convert camera's rotation to rotation matrix
    camera_rotation_matrix, _ = cv2.Rodrigues(np.deg2rad(map_camera_rot))
    # Create camera's transformation matrix in the map frame
    camera_transform_matrix = np.hstack((camera_rotation_matrix, map_camera_pos.reshape(3, 1)))
    # Inverse of camera's transformation matrix
    camera_transform_matrix_inv = np.linalg.inv(camera_transform_matrix)

    # Convert camera's pixel coordinates to homogeneous coordinates
    camera_coords_homogeneous = np.ones((3, 1))
    camera_coords_homogeneous[0] = camera_coords[0]
    camera_coords_homogeneous[1] = camera_coords[1]

    # Convert camera's coordinates to map coordinates
    map_coords_homogeneous = np.dot(camera_transform_matrix_inv, camera_coords_homogeneous)
    map_coords = tuple(map_coords_homogeneous[:3].reshape(1, -1)[0])

    return map_coords


def arucoCallback(msg):
    global x
    global y
    global ids
    x = msg.center_x_px
    y = msg.center_y_px
    ids = msg.marker_ids
    
    for i in ids:
        index = ids.index(i)
        if int(i/10) == 47:
            aruco_marker_camera = (x[index], y[index])
            aruco_marker_map = camera_to_map(aruco_marker_camera)
            print("Aruco marker coordinates in the camera frame: ", aruco_marker_camera)
            print("Aruco marker coordinates in the map frame: ", aruco_marker_map)

            if 0 < x[index] < 800 and 1100 < y[index] < 1650:
                Gobst(-750, 700, 500)
            if 0 < x[index] < 900 and 250 < y[index] < 750:
                Gobst(-750, 1500, 500)
            if 2600 < x[index] < 3250 and 250 < y[index] < 750:
				Gobst(750, 1500, 500)
			if 2600 < x[index] < 3350 and 1100 < y[index] < 1650:
				Gobst(750, 700, 500)
		else:
			pass

def sendArduino(data):
ser.write(data.encode())
time.sleep(0.1)
ser.flushInput()

if name == 'main':
	# Initialize the ROS node
	rospy.init_node('basketball_shooter_node')
	# Subscribe to the aruco info topic
	rospy.Subscriber('aruco_info', ArucoInfo, arucoCallback)
	
	# Subscribe to the panier topic
	rospy.Subscriber('panier', Int32, panierCallback)
	
	# Start the serial communication with the arduino
	ser = serial.Serial('/dev/ttyACM0',9600)
	
	# Wait for the serial port to open
	time.sleep(2)
	
	# Raise the basket to the shooting position
	sendArduino('e')
	time.sleep(5)
	
	# Shoot the ball
	sendArduino('f')
	time.sleep(3)
	
	# Lower the basket to the original position
	sendArduino('d')
	time.sleep(5)
	
	# Close the serial port
	ser.close()

