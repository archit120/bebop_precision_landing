#!/usr/bin/env python

import cv2
import rospy
from sensor_msgs.msg import Image
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist
import h_detect
from cv_bridge import CvBridge, CvBridgeError
from math import pow, atan2, sin, cos


z = 0

def odomCall(data):
    z = data.pose.pose.position.z


p_error = 0
i_error = 0
p_prev_error = 0
d_error = 0

kp = 1
kd = 0
ki = 0

tans = 0

frame = 0

bridge = CvBridge()

new_resp = False
resp = 0

def processimg(img):
    try:
        img = bridge.imgmsg_to_cv2(img, "bgr8")
    except CvBridgeError as e:
        print(e)
    cv2.imshow(img.data)      
    cx, cy = h_detect.center(img)
    if frame!=0:
        p_prev_error = p_error
    ex = (cx - img.width//2)
    ey = (cy - img.height//2)
    tans = atan2(ey,ex)
    p_error = pow(pow(ex,2)+pow(ey,2), 0.5)
    i_error += p_error
    if frame!=0:
        d_error = (p_error-p_prev_error)
    resp = kp*p_error + kd*d_error + ki*i_error
    new_resp = True
    frame=1


def listener():    
    rospy.init_node('precision_landing')

    rospy.Subscriber('/bebop/odom', Odometry, odomCall)
    rospy.Subscriber('/bebop/image_raw', Image, processimg)
    pub = rospy.Publisher('/bebop/cmd_vel', Twist)

    print("press enter start to PID process")
    input()

    rate = rospy.Rate(50)
    while not rospy.is_shutdown():
        if new_resp:
            new_resp=False
            rot = Twist()
            rot.linear.x = cos(tans)*resp
            rot.linear.y = sin(tans)*resp
            pub.publish(rot)
        rate.sleep()


    # spin() simply keeps python from exiting until this node is stopped
    #rospy.spin()

if __name__ == '__main__':
    listener()
