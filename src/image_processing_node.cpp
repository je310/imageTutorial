#include <ros/ros.h>
#include <ros/package.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Float32.h>

#include <stdio.h>
#include <string.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include "csv.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>


void imageCallback(const sensor_msgs::ImageConstPtr& msg);
bool lor(cv::Mat image);
float lorFloat(cv::Mat image);

//global publisher, as we need to use it in the callback function
ros::Publisher turnPub;

int main(int argc, char** argv){





    ros::init(argc, argv, "image_processing_node");

    //make strings containing the paths to the test images.
    std::string path = ros::package::getPath("imageTutorial"); // this line finds where this package is on your computer without using /philip etc
    std::stringstream ssLeft, ssRight, ssDota;
    ssLeft << path << "/src/left.jpg";      //here we just add the ending of the path.
    ssRight << path << "/src/right.jpg";    //using a stringstream is very similar to using std::cout !

    //do a check to make sure that ros found the folder on the computer.
    if(path.empty()){
        std::cout << "I do not know where imageTutorial is..." << std::endl;
        return -1;
    }


    //practicing some Dota Code;
    ssDota << path << "/src/dotaStats.csv";
    io::CSVReader<2> in(ssDota.str());
    in.read_header(io::ignore_extra_column, "A", "STR");
    std::string A; double STR;
    double Maxstr = 0;
    std::string Strongest;
    while(in.read_row(A, STR)){
        std::cout << A<< std::endl;
        if(STR > Maxstr) {
            Maxstr = STR;
            Strongest = A;
        }
        // do stuff with the data
      }
    std::cout << Strongest << " is the strongest hero with " << Maxstr << " starting strength" << std::endl;





    //set up ros objects
    ros::NodeHandle node;
    image_transport::ImageTransport it_(node);
    turnPub = node.advertise<std_msgs::Float32>("/turn",1);

    //Subscribe to the thermal image topic.
    image_transport::Subscriber imSub = it_.subscribe("/thermal",1, imageCallback);

    //test lor function
    cv::Mat left, right;
    left = cv::imread(ssLeft.str());
    right = cv::imread(ssRight.str());
    bool leftTest = lor(left);
    bool rightTest = lor(right);
    if(leftTest == false && rightTest == true) std::cout << "passed our LoR test" << std::endl;

    //test lorFloat function
    float leftFloat = lorFloat(left);
    float rightFloat = lorFloat(right);
    std::cout << leftFloat << " "  << rightFloat << std::endl;
    if(leftFloat < 0 && rightFloat > 0) std::cout << "passed our LoRFloat test" << std::endl;

    while(ros::ok()){
        ros::spinOnce();
    }


}

void imageCallback(const sensor_msgs::ImageConstPtr& msg){

    //get the image from the message
    cv::Mat image =  cv_bridge::toCvShare(msg, "bgr8")->image;

    //find if the warmer side is left or right
    bool turn = lor(image);

    // if right do ..., else if left do...
    std_msgs::Float32 turningMessage;
    if(turn){
        std::cout << "right" << std::endl;
        turningMessage.data = 1.0;
    }
    else{
        std::cout << "left" << std::endl;
        turningMessage.data = -1.0;
    }
    turningMessage.data = lorFloat(image);

    //publish a float number for another node to use to control the robot.
    turnPub.publish(turningMessage);
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

float lorFloat(cv::Mat image){
    //positive right, negative left
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
    float total = left + right;
    return (right - left)/ total;
}
