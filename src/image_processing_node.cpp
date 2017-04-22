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
bool lor(cv::Mat image);

int main(int argc, char** argv){
    ros::init(argc, argv, "image_processing_node");


    //set up ros objects
    ros::NodeHandle node;
    image_transport::ImageTransport it_(node);

    image_transport::Subscriber imSub = it_.subscribe("/thermal",1, imageCallback);

    //test lor function
    cv::Mat left, right;
    left = cv::imread("/home/philip/catkin_ws/src/imageTutorial/src/left.jpg");
    right = cv::imread("/home/philip/catkin_ws/src/imageTutorial/src/right.jpg");
    cv::imshow("left", left);
    cv::imshow("right", right);
    cv::waitKey(1);
    bool leftTest = lor(left);
    bool rightTest = lor(right);

    while(ros::ok()){

        ros::spinOnce();
    }


}

void imageCallback(const sensor_msgs::ImageConstPtr& msg){
    cv::Mat image =  cv_bridge::toCvShare(msg, "bgr8")->image;
    bool turn = lor(image);
    if(turn){
        std::cout << "right" << std::endl;

    }
    else{
        std::cout << "left" << std::endl;
    }
    cv::waitKey(1);
}
bool lor(cv::Mat image){
    //True = right, False = left
    cv::Mat gray;
    cv::cvtColor(image, gray, CV_BGR2GRAY);
    int left = 0;
    int right = 0;
    // add pixels into 2 bins: left and right.
    for(int i = 0; i<image.cols; i++){
        for(int j = 0; j<image.rows; j++){
            int value = gray.at<uchar>(j, i);
            if(i<gray.cols/2){
                left += value;


            }
            else{
                right+=value;
            }
        }
    }
    if(left>right){
        return false;

    }
    else{
        return true;
    }
}
