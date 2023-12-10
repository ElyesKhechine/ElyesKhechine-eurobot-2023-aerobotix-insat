#!/usr/bin/env python3
import rospy
from std_msgs.msg import Int32
from playsound import playsound



def vitrineCallback(msg): 
    print("Mission done")
    if msg.data==1:
    	playsound('/home/ubuntu/Downloads/fide2i.mp3')
    elif msg.data==2:
    	playsound('/home/ubuntu/Downloads/rko.mp3')
    elif msg.data==3:
        playsound('/home/ubuntu/Downloads/erwin.mpga')
    elif msg.data==4:
    	playsound('/home/ubuntu/Downloads/gta.mpga')
    elif msg.data==5:
    	playsound('/home/ubuntu/Downloads/horn.mpga')
    elif msg.data==20:
    	playsound('/home/ubuntu/arduino.mp3')
    elif msg.data==21:
    	playsound('/home/ubuntu/stm.mp3')
    elif msg.data==22:
    	playsound('/home/ubuntu/screen.mp3')
    elif msg.data==23:
    	playsound('/home/ubuntu/lidar.mp3')
    elif msg.data==24:
    	playsound('/home/ubuntu/all.mp3')
    

    
def listener():
    rospy.init_node('vitrineNode', anonymous=True)
    print("waiting for vitrine mission ...")
    vitrineSub=rospy.Subscriber("/vitrine", Int32, vitrineCallback)

    rospy.spin()

if __name__ == '__main__':
    listener()
