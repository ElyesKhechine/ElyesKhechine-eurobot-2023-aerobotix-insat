marker_position = tvec[0]
                
# Update the coordinates of the detected marker in the map frame
moving_aruco_marker_coordinates[id[0]] = marker_position


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

    camera_coords_homogeneous = np.hstack((camera_coords, 1))
    map_coords_homogeneous = camera_rotation_matrix_inverse.dot(camera_coords_homogeneous.T) + camera_translation_matrix_inverse
    map_coords = map_coords_homogeneous[:3]

    # object_points, _ = cv2.projectPoints(image_points, inv_rotation_vector, inv_translation_vector, camera_matrix, None)
    #return object_points[0][0]

    return map_coords



image_with_markers = detect_aruco_markers(frame)
cake_marker_camera_frame_coords = map_to_camera(47)
cake_marker_map_frame_coords = camera_to_map(cake_marker_camera_frame_coords)
print("Cake marker coordinates in the camera frame:", cake_marker_camera_frame_coords)
print("Cake marker coordinates in the map frame:", cake_marker_map_frame_coords)
# Display the resulting image with detected ArUco markers
    cv2.imshow("ArUco Marker Detection", image_with_markers)