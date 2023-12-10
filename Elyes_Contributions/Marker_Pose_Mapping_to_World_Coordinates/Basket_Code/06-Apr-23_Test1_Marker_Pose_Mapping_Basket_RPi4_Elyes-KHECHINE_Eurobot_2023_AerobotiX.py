#Import the required libraries
import cv2
import numpy as np

# Define the coordinates of the fixed ArUco markers in the map frame
fixed_aruco_marker_coordinates = {
    20: np.array([-430, 575, 0]), # x, y, z in millimeters
    21: np.array([430, 575, 0]),
    22: np.array([-430, 2425, 0]),
    23: np.array([430, 2425, 0])
}

# Define the dimensions of the fixed ArUco markers in millimeters
fixed_aruco_marker_size = 100 # 10 cm

# Define the dimensions of the moving ArUco markers in millimeters
aruco_marker_size = 70 # 7cm

# Define the coordinates of the moving ArUco markers in the map frame
moving_aruco_marker_coordinates = {
    47: np.array([0, 0, 0]), # x, y, z in millimeters
    13: np.array([0, 0, 0]),
    36: np.array([0, 0, 0])
}

# Define the intrinsic parameters of the camera
camera_matrix = np.array([[focal_length_x, 0, principal_point_x],
                          [0, focal_length_y, principal_point_y],
                          [0, 0, 1]])
# Replace the values with the actual values of your camera
dist_coeffs = np.zeros((4,1), dtype=np.float32)

# Define the extrinsic parameters of the camera
camera_rotation_vector = np.array([[0], [0], [0]]) # given
camera_translation_vector = np.array([[0], [-100], [1050]]) # given

# Convert the camera orientation from degrees to radians
camera_orientation = np.radians(camera_rotation_vector)

#Load the ArUco dictionary and set parameters for detection
aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_50)
parameters = cv2.aruco.DetectorParameters_create()
#Capture video from the camera
cap = cv2.VideoCapture(0)
#Detect ArUco markers in the video feed
#Here, we are reading frames from the video feed in a loop. 








def detect_aruco_markers(image):
    # Convert the image to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    # then using the detectMarkers function to detect ArUco markers in the frame. 
    #This function returns the corners of the detected markers, their ids, and any rejected points (points that were not detected as part of a marker). 
    corners, ids, _ = cv2.aruco.detectMarkers(gray, aruco_dict, parameters)
    

    image_with_markers = cv2.aruco.drawDetectedMarkers(image, corners, ids)
    
    # Extract the coordinates of the detected ArUco markers in the map frame
    if ids is not None:
        for i, id in enumerate(ids):
            if id[0] in moving_aruco_marker_coordinates.keys():
                # Calculate the position of the detected marker in the map frame
                rvec, tvec, _ = cv2.aruco.estimatePoseSingleMarkers(corners[i], aruco_marker_size, camera_matrix, dist_coeffs)






                marker_position = tvec[0]
                
                # Update the coordinates of the detected marker in the map frame
                moving_aruco_marker_coordinates[id[0]] = marker_position

                #For each detected marker, we draw its 3D coordinate axes on the frame using the drawAxis function.
                cv2.aruco.drawAxis(image, camera_matrix, dist_coeffs, rvec[i], tvec[i], 0.1)
                

                # We print the marker ID and the camera's coordinates.
                print("Marker ID:", ids[i])

    return image_with_markers

# Define the function to convert ArUco marker coordinates from the map frame to the camera frame
def map_to_camera(frame_coords):
    # Compute the rotation and translation vectors of the camera
    # from the ArUco marker coordinates in the camera frame
    (success, rotation_vector, translation_vector) = cv2.solvePnP(
        np.array([moving_aruco_marker_coordinates[frame_coords]]),
        np.array([[0, 0, 0]]),
        camera_matrix,
        dist_coeffs
    )
    # Transform the ArUco marker coordinates from the map frame
    # to the camera frame using the rotation and translation vectors
    object_points = np.array([moving_aruco_marker_coordinates[frame_coords]])
    image_points, _ = cv2.projectPoints(object_points, rotation_vector, translation_vector, camera_matrix, dist_coeffs)
    return image_points[0][0]

# Define the function to convert ArUco marker coordinates from the camera frame to the map frame
def camera_to_map(camera_coords):
    # Compute the inverse of the rotation and translation vectors

    camera_rotation_matrix, _ = cv2.Rodrigues(camera_rotation_vector)
    camera_translation_matrix = np.array([camera_translation_vector]).T
    camera_rotation_matrix_inverse = np.linalg.inv(camera_rotation_matrix)
    camera_translation_matrix_inverse = -camera_rotation_matrix_inverse.dot(camera_translation_matrix)
    # Transform the ArUco marker coordinates from the camera frame
    # to the map frame using the inverse of the rotation and translation vectors
    image_points = np.array([camera_coords])

    _homogeneous = np.hstack((camera_coords, 1))
    map_coords_homogeneous = camera_rotation_matrix_inverse.dot(camera_coords_homogeneous.T) + camera_translation_matrix_inverse
    map_coords = map_coords_homogeneous[:3]

    # object_points, _ = cv2.projectPoints(image_points, inv_rotation_vector, inv_translation_vector, camera_matrix, None)
    #return object_points[0][0]

    return map_coords
 


#Define the function to run the ArUco marker detection algorithm
def run_aruco_marker_detection():
    # Initialize the camera and start capturing frames
    camera = cv2.VideoCapture(0)
    ret, frame = camera.read()
    # Detect ArUco markers in the frame and convert their coordinates to the camera and map frames
    image_with_markers = detect_aruco_markers(frame)
    cake_marker_camera_frame_coords = map_to_camera(47)
    cake_marker_map_frame_coords = camera_to_map(cake_marker_camera_frame_coords)
    print("Cake marker coordinates in the camera frame:", cake_marker_camera_frame_coords)
    print("Cake marker coordinates in the map frame:", cake_marker_map_frame_coords)
    # Display the resulting image with detected ArUco markers
    cv2.imshow("ArUco Marker Detection", image_with_markers)

     #  Check if the user pressed the 'q' key to quit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

while True:
   run_aruco_marker_detection()


#To convert points in the camera's coordinate system to points in a world frame away from the camera, we need to know the position and orientation of the camera in the world frame. 
#This can be obtained by placing a known marker (e.g., a large ArUco marker) at a known location in the world frame and using the estimatePoseSingleMarkers function to obtain the camera's pose relative to the marker



 # We then convert the marker coordinates to the camera coordinate system using the transformation matrix obtained from the marker's pose.
# The transformation matrix is obtained by stacking the rotation and translation vectors obtained from estimatePoseSingleMarkers and adding a row of [0, 0, 0, 1] to the bottom
#This matrix is inverted and multiplied by the marker's coordinates to obtain the camera's coordinates.
#marker_coords = np.array([0, 0, 0, 1], dtype=np.float32)
#marker_coords[:3] = tvecs[i].ravel()
#camera_coords = np.dot(np.linalg.inv(np.vstack([rvecs[i], [0, 0, 0]])), marker_coords)

# We print the marker ID and the camera's coordinates.
#print("Marker ID:", ids[i])
#print("Camera Coordinates:", camera_coords)
