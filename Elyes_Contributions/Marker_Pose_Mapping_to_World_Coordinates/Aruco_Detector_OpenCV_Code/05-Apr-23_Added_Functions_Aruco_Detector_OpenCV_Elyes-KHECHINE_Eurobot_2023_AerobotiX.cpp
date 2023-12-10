#include <opencv2/opencv.hpp>

cv::Point2d map_to_camera(int frame_coords, cv::Mat camera_matrix, cv::Mat dist_coeffs, std::vectorcv::Point3d moving_aruco_marker_coordinates)
{
// Compute the rotation and translation vectors of the camera
// from the ArUco marker coordinates in the camera frame
cv::Mat rvec, tvec;
cv::Mat marker_coords = cv::Mat(moving_aruco_marker_coordinates[frame_coords]);
cv::solvePnP(marker_coords, cv::Mat::zeros(1, 3, CV_64F), camera_matrix, dist_coeffs, rvec, tvec);

// Transform the ArUco marker coordinates from the map frame
// to the camera frame using the rotation and translation vectors
std::vector<cv::Point3d> object_points;
object_points.push_back(moving_aruco_marker_coordinates[frame_coords]);

std::vector<cv::Point2d> image_points;
cv::projectPoints(object_points, rvec, tvec, camera_matrix, dist_coeffs, image_points);

return image_points[0];

}

cv::Point3d camera_to_map(cv::Point2d camera_coords, cv::Mat camera_rotation_vector, cv::Mat camera_translation_vector)
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

// Define the function to run the ArUco marker detection algorithm
void run_aruco_marker_detection(cv::Mat camera_matrix, cv::Mat dist_coeffs, std::vectorcv::Point3d moving_aruco_marker_coordinates)
{
// Initialize the camera and start capturing frames
cv::VideoCapture camera(0);
while (true)
{
    // Capture a frame from the camera
    cv::Mat frame;
    camera.read(frame);

    // Detect ArUco markers in the frame and convert their coordinates to the camera and map frames
    cv::Mat image_with_markers = detect_aruco_markers(frame);
    cv::Point2d cake_marker_camera = map_to_camera(47, camera_matrix, dist_coeffs, moving_aruco_marker_coordinates);
    cv::Point3d cake_marker_map = camera_to_map(cake_marker_camera, camera_rotation_vector, camera_translation_vector);
    std::cout << "Cake marker coordinates in the camera frame: " << cake_marker_camera << std::endl;
    std::cout << "Cake marker coordinates in the map frame: " << cake_marker_map << std::endl;

    // Display the resulting image with detected ArUco markers
    cv::imshow("ArUco Marker Detection", image_with_markers)