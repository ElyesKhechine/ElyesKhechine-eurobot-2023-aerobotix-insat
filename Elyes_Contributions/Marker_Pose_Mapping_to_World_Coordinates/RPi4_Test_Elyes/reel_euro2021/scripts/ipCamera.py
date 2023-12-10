
import rospy 
import cv2
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import time
from imutils.video import FileVideoStream
from imutils.video import FPS
import numpy as np
import argparse
import imutils




class AndroidCamera(object):
    def __init__(self, node_name):
        # construct the argument parse and parse the arguments
        ap = argparse.ArgumentParser()
        ap.add_argument("-v", "--video", required=True,
            help="path to input video file")
        args = vars(ap.parse_args())
        # start the file video stream thread and allow the buffer to
        # start to fill
        print("[INFO] starting video file thread...")
        fvs = FileVideoStream(args["video"]).start()
        
        # start the FPS timer
        fps = FPS().start()
        self.image_pub = rospy.Publisher("/usb_cam/image_raw", Image,queue_size=100)
        self.bridge = CvBridge()
        rospy.init_node(node_name, anonymous = False)
        time.sleep(1.0)
        while not rospy.is_shutdown() and  fvs.more():
            # grab the frame from the threaded video file stream, resize
            # it, and convert it to grayscale (while still retaining 3
            # channels)
            frame = fvs.read()
            frame = imutils.resize(frame, width=1920)
            frame1=frame
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            frame = np.dstack([frame, frame, frame])
            # display the size of the queue on the frame
            # cv2.putText(frame, "Queue Size: {}".format(fvs.Q.qsize()),
            #     (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)	
            # show the frame and update the FPS counter
            try:
                    self.image_message = self.bridge.cv2_to_imgmsg(frame, "passthrough")
            except CvBridgeError as e:
                    print(e)
            self.image_message.header.frame_id="camera";
            self.image_pub.publish(self.image_message)
            # cv2.imshow("Frame", frame)
            # cv2.waitKey(1)
            fps.update()


    # def __del__(self):
    #     self._device.release()
    
    # def get_frame(self):
    #     _, frame = self._device.read()
        
    #     return frame
                

        
print("Starting...")         
node = AndroidCamera(
    "ipcamera_node")


