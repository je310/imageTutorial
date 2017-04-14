#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <stdio.h>
#include <string.h>
#include <cstring>
#include <sstream>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>


void imageCallback(const sensor_msgs::ImageConstPtr& msg);

int main(int argc, char** argv){
    ros::init(argc, argv, "image_processing_node");


    //set up ros objects
    ros::NodeHandle node;
    image_transport::ImageTransport it_(node);

    image_transport::Subscriber imSub = it_.subscribe("/pathView",1, imageCallback);

    while(ros::ok()){


        ros::spinOnce();
    }


}

void imageCallback(const sensor_msgs::ImageConstPtr& msg){
     cv::Mat image =  cv_bridge::toCvShare(msg, "bgr8")->image;
     cv::imshow("recievedIMAGE!",image);
     cv::waitKey(1);
}
