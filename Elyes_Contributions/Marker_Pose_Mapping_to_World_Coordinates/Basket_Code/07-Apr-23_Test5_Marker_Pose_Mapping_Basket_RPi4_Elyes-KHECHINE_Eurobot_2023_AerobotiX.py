#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int32
from std_msgs.msg import Int16MultiArray
import serial
import time
import os
import alfons_msgs 
from alfons_msgs.msg import ArucoInfo

def map_to_camera(x_, y_, z_, camera_matrix, dist_coeffs):
    # Create the marker_coords cv::Mat from x, y, z values
    marker_coords = np.array([[x_], [y_], [z_]], dtype=np.float32)

    # Compute the rotation and translation vectors of the camera
    # from the ArUco marker coordinates in the camera frame
    _, rvec, tvec = cv2.solvePnP(marker_coords, np.zeros((1, 3), dtype=np.float32), camera_matrix, dist_coeffs)

    # Transform the ArUco marker coordinates from the map frame
    # to the camera frame using the rotation and translation vectors
    object_points = np.array([[x_, y_, z_]], dtype=np.float32)
    image_points, _ = cv2.projectPoints(object_points, rvec, tvec, camera_matrix, dist_coeffs)

    x_ = int(image_points[0, 0])
    y_ = int(image_points[0, 1])
    return (x_, y_)

def camera_to_map(camera_coords, camera_rotation_vector, camera_translation_vector):
    # Compute the inverse of the rotation and translation vectors
    camera_rotation_matrix, _ = cv2.Rodrigues(camera_rotation_vector)
    camera_translation_matrix = camera_translation_vector.reshape((3, 1))
    camera_rotation_matrix_inverse = np.linalg.inv(camera_rotation_matrix)
    camera_translation_matrix_inverse = -np.dot(camera_rotation_matrix_inverse, camera_translation_matrix)

    # Transform the ArUco marker coordinates from the camera frame
    # to the map frame using the inverse of the rotation and translation vectors
    camera_coords_homogeneous = np.ones((4, 1), dtype=np.float32)
    camera_coords_homogeneous[0, 0] = camera_coords[0]
    camera_coords_homogeneous[1, 0] = camera_coords[1]
    map_coords_homogeneous = np.dot(camera_rotation_matrix_inverse, camera_coords_homogeneous) + camera_translation_matrix_inverse
    map_coords = (map_coords_homogeneous[0, 0], map_coords_homogeneous[1, 0], map_coords_homogeneous[2, 0])
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
        if(int(i/10) == 47):
            if(0 < x[index] < 800 and 1100 < y[index] < 1650):
                Gobst(-750, 700, 500)
            if(0 < x[index] < 900 and 250 < y[index] < 750):
                Gobst(-750, 1500, 500)
            if(2600 < x[index] < 3250 and 250 < y[index] < 750):
                Gobst(750, 1500, 500)
            if(2800 < x[index] < 3800 and 1100 < y[index] < 1650):
                Gobst(750, 700, 500)

def main():
    global x
    global y
    global ids

    x = 0
    y = 0
    ids = []

    
rospy.init_node('alfons_controller', anonymous=True)

# Subscribe to the ArUco marker info topic
rospy.Subscriber('aruco_info', ArucoInfo, arucoCallback)

# Create a publisher for the motor commands
pub = rospy.Publisher('motor_cmds', Int16MultiArray, queue_size=10)

# Initialize the serial connection to the motor controller
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

# Wait for the serial connection to be established
time.sleep(2)

# Initialize the camera matrix and distortion coefficients
camera_matrix = np.array([[1428.93, 0, 959.50], [0, 1433.93, 539.75], [0, 0, 1]], dtype=np.float32)
dist_coeffs = np.array([[-0.109457, 0.126355, -0.000217, -0.000347, 0]], dtype=np.float32)

# Loop until the node is shutdown
while not rospy.is_shutdown():
    # If there are no markers detected, stop the robot
    if len(ids) == 0:
        motor_cmds = Int16MultiArray(data=[0, 0, 0, 0])
        pub.publish(motor_cmds)
        ser.write(b'\x00\x00\x00\x00')
        continue

    # If there are markers detected, get the position of the first one
    marker_x = 0
    marker_y = 0
    for i in range(len(ids)):
        if ids[i] == 47:
            marker_x, marker_y = map_to_camera(0, 0, 0, camera_matrix, dist_coeffs)
            break

    # If the marker is not in the center of the image, turn the robot
    if abs(marker_x - 640) > 50:
        if marker_x < 640:
            motor_cmds = Int16MultiArray(data=[0, -300, 0, 300])
            pub.publish(motor_cmds)
            ser.write(b'\x00\x00\x01\x01')
        else:
            motor_cmds = Int16MultiArray(data=[0, 300, 0, -300])
            pub.publish(motor_cmds)
            ser.write(b'\x00\x00\x02\x02')
    else:
        # If the marker is in the center of the image, move the robot forward
        motor_cmds = Int16MultiArray(data=[300, 300, 300, 300])
        pub.publish(motor_cmds)
        ser.write(b'\x01\x01\x01\x01')

# Shutdown the serial connection
ser.close()
