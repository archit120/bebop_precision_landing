#!/usr/bin/env python

import cv2
import rospy
from sensor_msgs.msg import Image
from nav_msgs.msg import Odometry
import h_detect


def callback(data):
    #rospy.loginfo(rospy.get_caller_id() + 'Ishowimg heard %s', data.data)
    a=0

perror = 0
ierror = 0
derror = 0


def processimg(img):
    cv2.imshow(img.data)   
    img = img.data 
    features = h_detect.get_features(img)

    
def listener():    
    rospy.init_node('precision_landing')

    rospy.Subscriber('odom', Odometry, callback)
    rospy.Subscriber('image_raw', Image, processimg)

    print("press enter to PID process")
    input()


    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()