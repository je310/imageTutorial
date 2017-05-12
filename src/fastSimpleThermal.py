#!/usr/bin/env python


import rospy


import serial
import cv2
import sys
import numpy

from std_msgs.msg import String
from std_msgs.msg import Float32
from std_msgs.msg import Int32
from collections import namedtuple
import struct




from collections import namedtuple
import struct
import time
import roslib
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import os.path
import base64

threadRunning = False

def pubThread(pub,image,bridge):
    min = 8050
    max = 8650
    mCorrected = (image - min)/(max-min + 0.0)
    mCorrected = mCorrected*255
    cvuint8 = cv2.convertScaleAbs(mCorrected)


    cv_image = bridge.cv2_to_imgmsg(cvuint8, "mono8")
    pub.publish(bridge.cv2_to_imgmsg(cvuint8, "mono8"))

def unsigned(n):
    return n & 0xFFFFFFFF



def talker():



    rospy.init_node('thermal', anonymous=True)
    serdev = '/dev/ttyACM0'
    mbed = serial.Serial(serdev, 921600)
    bridge = CvBridge()
    image_pub = rospy.Publisher("thermal",Image,queue_size=5)

    min = 8050
    max = 8650
    size = 1, 80

    m = numpy.zeros(size, dtype=numpy.int32)

    count = 0;
    start = time.time()
    isSyncronised = 's'
    useCorrection = 0
    while (1):
        count = count+1
        if count%30 == 0:
            print(30/(time.time() - start),'fps')
            start = time.time()
        

        while (ord(isSyncronised) != 0xde):
            isSyncronised = mbed.read(1)

        while (ord(isSyncronised) != 0xad):
            isSyncronised = mbed.read(1)

        while (ord(isSyncronised) != 0xbe):
            isSyncronised = mbed.read(1)

        while (ord(isSyncronised) != 0xef):
            isSyncronised = mbed.read(1)

        packingStruct = struct.Struct('BBBB')
        unpackingStruct = struct.Struct('>i')

        for x in range(1):
            for y in range(80):
                a = mbed.read(1)
                b = mbed.read(1)
                m[x][y] = (ord(a)*255 + ord(b))



        mCorrected = (m - min)/(max-min + 0.0)
        mCorrected = mCorrected*255
        cvuint8 = cv2.convertScaleAbs(mCorrected)


        cv_image = bridge.cv2_to_imgmsg(cvuint8, "mono8")
        image_pub.publish(bridge.cv2_to_imgmsg(cvuint8, "mono8"))



if __name__ == '__main__':
    talker()

