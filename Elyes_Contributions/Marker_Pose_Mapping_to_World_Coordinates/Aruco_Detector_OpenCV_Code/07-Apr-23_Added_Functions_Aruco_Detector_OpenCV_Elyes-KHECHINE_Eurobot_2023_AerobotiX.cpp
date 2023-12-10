
///function that converts one marker's coordinates x y z from map to camera frame
cv::Point2d map_to_camera(double& x_, double& y_, double& z_, cv::Mat camera_matrix, cv::Mat dist_coeffs)
{
    // Create the marker_coords cv::Mat from x, y, z values
    cv::Mat marker_coords = (cv::Mat_<double>(3,1) << x_, y_, z_);

    // Compute the rotation and translation vectors of the camera
    // from the ArUco marker coordinates in the camera frame
    cv::Mat rvec, tvec;
    cv::solvePnP(marker_coords, cv::Mat::zeros(1, 3, CV_64F), camera_matrix, dist_coeffs, rvec, tvec);

    // Transform the ArUco marker coordinates from the map frame
    // to the camera frame using the rotation and translation vectors
    std::vector<cv::Point3d> object_points;
    object_points.push_back(cv::Point3d(x_, y_, z_));

    std::vector<cv::Point2d> image_points;
    cv::projectPoints(object_points, rvec, tvec, camera_matrix, dist_coeffs, image_points);

	x_ = (int) image_points[0].x;
	y_ = (int) image_points[0].y;
    return image_points[0];
}





cv::Point3d camera_to_map(cv::Point2d &camera_coords, cv::Mat camera_rotation_vector, cv::Mat camera_translation_vector)
{
// Compute the inverse of the rotation and translation vectors
cv::Mat camera_rotation_matrix, camera_translation_matrix;
cv::Rodrigues(camera_rotation_vector, camera_rotation_matrix);
cv::transpose(camera_translation_vector, camera_translation_matrix);
cv::Mat camera_rotation_matrix_inverse = camera_rotation_matrix.inv();
cv::Mat camera_translation_matrix_inverse = -camera_rotation_matrix_inverse * camera_translation_matrix;

// Transform the ArUco marker coordinates from the camera frame
// to the map frame using the inverse of the rotation and translation vectors
cv::Mat camera_coords_homogeneous = cv::Mat::ones(4, 1, CV_64F);
camera_coords_homogeneous.at<double>(0, 0) = camera_coords.x;
camera_coords_homogeneous.at<double>(1, 0) = camera_coords.y;

cv::Mat map_coords_homogeneous = camera_rotation_matrix_inverse * camera_coords_homogeneous + camera_translation_matrix_inverse;
cv::Point3d map_coords(map_coords_homogeneous.at<double>(0, 0), map_coords_homogeneous.at<double>(1, 0), map_coords_homogeneous.at<double>(2, 0));

return map_coords;
}




///////////////////////ELYES/////////////////////////
			cv::Point2d cake_marker_camera = map_to_camera(x,y,z, camera_model.intrinsicMatrix(), distortion_coefficients);
			std::cout << "Cake marker coordinates in the camera frame: " << cake_marker_camera << std::endl;
			
			cv::Mat camera_rotation_vector = (cv::Mat_<double>(3, 1) << 0, 0, 0); // given
			cv::Mat camera_translation_vector = (cv::Mat_<double>(3, 1) << 0, -100, 1050); // give
			cv::Point3d cake_marker_map = camera_to_map(cake_marker_camera, camera_rotation_vector, camera_translation_vector);
    		std::cout << "Cake marker coordinates in the map frame: " << cake_marker_map << std::endl;
    		
    		x = (int) cake_marker_map.x;
		y= (int) cake_marker_map.y;
		z = (int) cake_marker_map.z;
		ar_msg.center_x_px.push_back(x);
		ar_msg.center_y_px.push_back(y);
    }
	///////////////////////ELYES/////////////////////////


