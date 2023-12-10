double[][] camera_param = api.getNavCamIntrinsics();
Mat cameraMatrix = new Mat(3, 3, CvType.CV_64FC1);
Mat distCoeffs = new Mat(1, 5, CvType.CV_64FC1);
Dictionary dictionary = Aruco.getPredefinedDictionary(Aruco.DICT_5X5_250);
List<Mat> markerCorners = new ArrayList<>();
Mat markerIds = new Mat();
DetectorParameters parameters = DetectorParameters.create();
List<Point> markerCenterPoints = new ArrayList<>();
Mat rvec = new Mat();
Mat tvec = new Mat();
cameraMatrix.put(0, 0, camera_param[0]);
distCoeffs.put(0, 0, camera_param[1]);
Aruco.detectMarkers(source, dictionary, markerCorners, markerIds, parameters);
Aruco.drawDetectedMarkers(outputImage, markerCorners, markerIds);

////
//1) test without ROI
//for (Mat corner : markerCorners) {
//    Log.i("ARUCO TEST", "without ROI corner.dump()" + corner.dump());
//}
//Mat outputImage = source.clone();
//Aruco.estimatePoseSingleMarkers(markerCorners, 0.05f, cameraMatrix, distCoeffs, rvec, tvec);
//Log.i("ARUCO test ", "without ROI rvec.dump() = " + rvec.dump());
//Log.i("ARUCO test ", "without ROI tvec.dump() = " + tvec.dump());


/////
final double[] laser = {777, 440};
final double[] euler_turningAngle = find_turningAngle(center_in_pixels.x , center_in_pixels.y , laser);
////////////////////////////////////////////////////////Log.i("angles ", "euler_turningAngle = ("+ euler_turningAngle[0] + ", " + euler_turningAngle[1] + ")");
final Quaternion q_turningAngle = eulerToQuaternion(euler_turningAngle[1], 0, euler_turningAngle[0]); // z y x
////////////////////////////////////////////////////////Log.i("angles ", "q_turningAngle = ("+ q_turningAngle.getX() + ", " + q_turningAngle.getY() + ", " +q_turningAngle.getZ() + ", " +q_turningAngle.getW() + ") ");
final Quaternion target_angle  = combineQuaternion(q_turningAngle, new Quaternion(0, 0, -0.707f, 0.707f));

result = moveToWrapper(11.22481667D, -10.1D, 5.493206667D, target_angle.getX(), target_angle.getY(), target_angle.getZ(), target_angle.getW());

/////
//test_board(cameraMatrix, distCoeffs,  markerCorners,  markerIds, b_tvec,  b_rvec,  offset);

//double[] target2_test_pos = calTargetPos();
//Log.i("target2_test_pos", "target2_test_pos: ("+ target2_test_pos[0] + ", " + target2_test_pos[1] + ")");
//Point3 target2 = find_target_cam(rvec, tvec);

Aruco.estimatePoseSingleMarkers(markerCorners, 0.05f, cameraMatrix, distCoeffs, rvec, tvec);
//Log.i("ARUCO test ", "with calculated ROI rvec.dump() = " + rvec.dump());
//Log.i("ARUCO test ", "with calculated ROI tvec.dump() = " + tvec.dump());
//4) test undistorted points
/*List <List <Point>> all_points = new ArrayList<>();
all_points.add(markerCornerPointsID11);
all_points.add(markerCornerPointsID12);
all_points.add(markerCornerPointsID13);
all_points.add(markerCornerPointsID14);
all_points.add(markerCenterPoints);
for (List <Point> list : all_points) {
    for (Point point : list){
        point = undistort(source, cameraMatrix, distCoeffs, point);
        Log.i("undistorting points", "Point (" + point.x + ", " + point.y + ")");
    }
}*/
//TBV
//Point3 center_cam_frame = find_target_center_cam_frame(centerOfAllMarkers, rvec, tvec, id12);
//Log.i("ARUCO test", "Center of markers in camera frame: ("+ center_cam_frame.x +", "+center_cam_frame.y+ ", " + center_cam_frame.z + ")");





public Point undistort(Mat source, Mat cameraMatrix, Mat distCoeffs, Point point){
        float[] floatpoint = {(float) point.x, (float) point.y};
        Mat undistortedMat = new Mat(1, 2, CvType.CV_32FC2);
        undistortedMat.put(0, 0, floatpoint);
        Imgproc.undistortPoints(undistortedMat, undistortedMat, cameraMatrix, distCoeffs, new Mat(), cameraMatrix);
        Log.i("undistort", "undistortedPoint.dump()" + undistortedMat.dump());
        Point undistortedPoint = new Point(undistortedMat.get(0, 0)[0], undistortedMat.get(0, 1)[0]);
        Imgproc.circle(source, undistortedPoint, 3, new Scalar(100, 100, 0), -1);
        return undistortedPoint;
    }


public double[] getCenter(List<Point> points) {
        final MatOfPoint points_ = new MatOfPoint();
        points_.fromList(points);
        return getCenter(points_);
    }
public double[] getCenter(MatOfPoint points) {

        Moments moments = Imgproc.moments(points);
        return (new double[]{moments.get_m10() / moments.get_m00(), moments.get_m01() / moments.get_m00()});
    }


 private double[] find_turningAngle(double circlecenterx,double circlecentery, double[] laser) {
        final double xDistance = circlecenterx - laser[0];
        final double yDistance = laser[1] - circlecentery;
        ////////////////////////////////////////////////////////Log.i("find_turningAngle", "xDistance=" + xDistance + " ; yDistance=" + yDistance);

        final double anglePerPixel = 0.08125;
        //anglePerPixel = + 130 / Math.sqrt(Math.pow(1280, 2) + Math.pow(960, 2));

        double xAngle = xDistance * anglePerPixel;
        double yAngle = yDistance * anglePerPixel;
        ////////////////////////////////////////////////////////Log.i("find_turningAngle", "xAngle=" + xAngle + "yAngle=" + yAngle);
        return new double[]{xAngle, yAngle};
    }


 private Quaternion eulerToQuaternion(double xAngle, double yAngle, double zAngle) {
        xAngle = Math.toRadians(xAngle);
        yAngle = Math.toRadians(yAngle);
        zAngle = Math.toRadians(zAngle);
        double c1 = Math.cos(yAngle/2);
        double s1 = Math.sin(yAngle/2);
        double c2 = Math.cos(zAngle/2);
        double s2 = Math.sin(zAngle/2);
        double c3 = Math.cos(xAngle/2);
        double s3 = Math.sin(xAngle/2);
        double w = c1*c2*c3 - s1*s2*s3;
        double x = s1*s2*c3 + c1*c2*s3;
        double y = s1*c2*c3 + c1*s2*s3;
        double z = c1*s2*c3 - s1*c2*s3;

        ////////////////////////////////////////////////////////Log.i("Convert euler angle to quaternion", " x:" + x + " y:" + y + " z:" + z + " w:" + w);
        return new Quaternion((float)x, (float) y, (float)z, (float)w);
    }

public void alignLaser(Point3 target){
        double[] current_orientation = {-90, 0, 0};
        double[] laser_point = {0.1302, 0.1111};
        double pivot_laser_target_angle = 139.525787;
        double r1 = 0.1711585522;  //pivot_to_laser_length
        double[] target_point = new double[2];

        //TBV
        double[] target_pos = calTargetPos();
        target_point[0] = 0.785;
//      target_point[1] = target_pos[1] + 0.0826;
        target_point[1] = target_pos[1] + 0.0926;
        //TBV

        // law of cosines: c2=a2+b2﹣2abcosγ
        // γ = pivot_laser_target_angle
        // a = R2 (unknown)
        // b = R1
        // c= pivot_to_target_length
        Log.d("AR[target_point(xy plane)]", target_point[0] + ", " + target_point[1]);
        // square root(delta x square + delta y square)
        double pivot_to_target_length = length(target_point[0], target_point[1]);
        Log.d("AR[pivot_to_target_length]", "" + pivot_to_target_length);
        double a = 1;
        double b = 2 * r1 * Math.cos(Math.toRadians(180 - pivot_laser_target_angle));
        double c = Math.pow(r1, 2) - Math.pow(pivot_to_target_length, 2);
        double r2 = (-b + Math.sqrt(Math.pow(b, 2) - 4 * a * c)) / 2 * a;
        Log.d("AR[radius]", r1 + ", " + r2);

        double[] laser_shooting_coord = find_laser_point(target_point, r1, r2);
        Log.d("AR[laser_shooting_coord]", laser_shooting_coord[0] + ", " + laser_shooting_coord[1]);

        double laser_origin_to_shooting_length = length(laser_point[0] - laser_shooting_coord[0], laser_point[1] - laser_shooting_coord[1]);
        Log.d("AR[laser_origin_to_shooting_length]", "" + laser_origin_to_shooting_length);

        double pitch = 2 * Math.toDegrees(Math.asin((0.5 * laser_origin_to_shooting_length) / r1));
        Log.d("AR[pitch]", "" + pitch);
        if (target_point[1] != 0 && pitch < 45) {
            current_orientation[1] = current_orientation[1] - pitch;
        } else {
            //Just to be safe
            current_orientation[1] = current_orientation[1] - 30;
        }

        double[] robot_pos = {api.getRobotKinematics().getPosition().getX(), api.getRobotKinematics().getPosition().getY(), api.getRobotKinematics().getPosition().getZ()};

        //TBV
        //        robot_pos[0] = robot_pos[0] + (target_pos[0] - 0.0994);
//        robot_pos[0] = robot_pos[0] + (target_pos[0] - 0.092);
        robot_pos[0] = robot_pos[0] + (target_pos[0] - 0.1094);
        //TBV

        Log.d("AR[Robot Position]", robot_pos[0] + ", " + current_orientation[1]);
        Quaternion q = eulerToQuaternion(current_orientation[0], current_orientation[1], current_orientation[2]);
        Result result = moveToWrapper(robot_pos[0],robot_pos[1],robot_pos[2], q.getX(), q.getY(), q.getZ(), q.getW());
    }

private double[] calTargetPos() {
        long start_time = SystemClock.elapsedRealtime();
        double[][] cameraParam = api.getNavCamIntrinsics();
        Mat cameraMatrix = new Mat(3, 3, CvType.CV_32FC1);
        Mat dstMatrix = new Mat(1, 5, CvType.CV_32FC1);
        cameraMatrix.put(0, 0, cameraParam[0]);
        dstMatrix.put(0, 0, cameraParam[1]);

        Mat ids = new Mat();
        Dictionary dictionary = Aruco.getPredefinedDictionary(Aruco.DICT_5X5_250);
        ArrayList<Mat> corners = new ArrayList<>();
        double xdiff = 0, ydiff = 0;

        Log.d("AR[status]:", "start");
        byte ar_count = 0;
        Rect ar_roi;
        while (ids.size().height != 4 && ar_count < 3) {
            try {
                if (ar_count == 0) {
                    ar_roi = new Rect(470, 680, 360, 280);
                }
                //fall back if the aruco tag is outside of cropped boundary.
                else {
                    ar_roi = new Rect(100, 660, 1080, 300);
                }
                Mat source = new Mat(api.getMatNavCam(), ar_roi);
                Aruco.detectMarkers(source, dictionary, corners, ids);
                for (int i = 0; i < corners.size(); i++) {
                    //Get shifted aruco tag corners
                    Mat corrected_corner = corners.get(i);
                    //Shift it by the position that it get cropped.
                    corrected_corner.put(0, 0, corrected_corner.get(0, 0)[0] + ar_roi.x, corrected_corner.get(0, 0)[1] + ar_roi.y);
                    corrected_corner.put(0, 1, corrected_corner.get(0, 1)[0] + ar_roi.x, corrected_corner.get(0, 1)[1] + ar_roi.y);
                    corrected_corner.put(0, 2, corrected_corner.get(0, 2)[0] + ar_roi.x, corrected_corner.get(0, 2)[1] + ar_roi.y);
                    corrected_corner.put(0, 3, corrected_corner.get(0, 3)[0] + ar_roi.x, corrected_corner.get(0, 3)[1] + ar_roi.y);
                    //tmp mat to store undistorted corners.
                    Mat tmp = new Mat(1, 4, CvType.CV_32FC2);
                    //undistort the corners.
                    Imgproc.undistortPoints(corners.get(i), tmp, cameraMatrix, dstMatrix, new Mat(), cameraMatrix);
                    //put it back in to the same array list.
                    corners.set(i, tmp);
                }
            } catch (Exception e) {
                Log.e("AR[status]:", "error", e);
            }
            ar_count++;
        }

        //TBV
        if (ids.size().height == 4) {
            float markerSize = 0.05f;
            double avg_ar_size = 0;
            double tx_undistort = 0, ty_undistort = 0;
            for (Mat corner : corners) {
                double _x = 0;
                double _y = 0;
                for (int j = 0; j < corner.size().width; j++) {
                    _x = _x + corner.get(0, j)[0];
                    _y = _y + corner.get(0, j)[1];
                }
                avg_ar_size += Math.abs(corner.get(0, 0)[0] - corner.get(0, 1)[0]);
                avg_ar_size += Math.abs(corner.get(0, 2)[0] - corner.get(0, 3)[0]);
                avg_ar_size += Math.abs(corner.get(0, 0)[1] - corner.get(0, 3)[1]);
                avg_ar_size += Math.abs(corner.get(0, 1)[1] - corner.get(0, 2)[1]);
                tx_undistort += _x / 4.0;
                ty_undistort += _y / 4.0;
            }
            tx_undistort /= 4;
            ty_undistort /= 4;
            avg_ar_size /= 16;
            double pixelPerM = avg_ar_size / markerSize;
            Log.d("AR[pixelperM]", "" + pixelPerM);
            //find diff from the center of the image
            xdiff = (tx_undistort - 640) / pixelPerM;
            ydiff = (480 - ty_undistort) / pixelPerM;
        }
        long stop_time = SystemClock.elapsedRealtime();
        Log.d("AR[count]", "" + ar_count);
        Log.d("AR[total_time]:", " " + (stop_time - start_time));
        Log.d("AR[target_pos]", xdiff + ", " + ydiff);
        return new double[]{xdiff, ydiff};

        //TBV
    }
    private double[] find_laser_point(double[] target_point, double r1, double r2) {
        double x1 = 0, y1 = 0, x2 = target_point[0], y2 = target_point[1];
        double centerdx = x1 - x2;
        double centerdy = y1 - y2;
        double R = length(centerdx, centerdy);
        double R2 = R * R;
        double R4 = R2 * R2;
        double a = (r1 * r1 - r2 * r2) / (2 * R2);
        double r2r2 = (r1 * r1 - r2 * r2);
        double c = Math.sqrt(2 * (r1 * r1 + r2 * r2) / R2 - (r2r2 * r2r2) / R4 - 1);
        double fx = (x1 + x2) / 2 + a * (x2 - x1);
        double gx = c * (y2 - y1) / 2;
        double ix1 = fx + gx;
        double ix2 = fx - gx;
        double fy = (y1 + y2) / 2 + a * (y2 - y1);
        double gy = c * (x1 - x2) / 2;
        double iy1 = fy + gy;
        double iy2 = fy - gy;
        if (iy1 > iy2) {
            return new double[]{ix1, iy1};
        }
        return new double[]{ix2, iy2};
    }

    
   
public Point3 find_target_center_cam_frame(Point center, Mat rvec, Mat tvec, int id) {
        //TBV
        Mat rot_mat = new Mat();
        Calib3d.Rodrigues(rvec, rot_mat);
        Mat rmat = rot_mat.t();
        double[][] rarray = {
                new double[]{rmat.get(0, 0)[0], rmat.get(0, 1)[0], rmat.get(0, 2)[0]},
                new double[]{rmat.get(1, 0)[0], rmat.get(1, 1)[0], rmat.get(1, 2)[0]},
                new double[]{rmat.get(2, 0)[0], rmat.get(2, 1)[0], rmat.get(2, 2)[0]}
        };

        double[][] offset = {
                new double[]{center.x},
                new double[]{center.y},
                new double[]{0.0D}};

        double[][] global_offset = multiplyMat(rarray, offset);

        return( new Point3(
                global_offset[0][0] +  tvec.get(0, 0)[0],
                global_offset[1][0] +  tvec.get(1, 0)[0],
                global_offset[2][0] +  tvec.get(2, 0)[0]));
    }