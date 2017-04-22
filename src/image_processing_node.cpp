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

    image_transport::Subscriber imSub = it_.subscribe("/thermal",1, imageCallback);

    while(ros::ok()){


        ros::spinOnce();
    }


}

void imageCallback(const sensor_msgs::ImageConstPtr& msg){
    cv::Mat image =  cv_bridge::toCvShare(msg, "bgr8")->image;
    cv::imshow("recievedIMAGE!",image);
    cv::Mat gray;
    cv::cvtColor(image, gray, CV_BGR2GRAY);
    int left = 0;
    int right = 0;
    // add pixels into 2 bins: left and right.
    for(int i = 0; i<image.cols; i++){
        for(int j = 0; j<image.rows; j++){
            int value = gray.at<uchar>(i, j);
            if(i<gray.cols/2){
                left += value;


            }
            else{
                right+=value;
            }
        }
    }
    if(left>right){
        std::cout << "left" << std::endl;

    }
    else{
        std::cout << "right" << std::endl;
    }
    cv::waitKey(1);
}
