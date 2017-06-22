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

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            result += buffer.data();
    }
    return result;
}

std::stringstream ssIm;
void imageCallback(const sensor_msgs::ImageConstPtr& msg);
int main(int argc, char** argv){





    ros::init(argc, argv, "image_processing_node");

    //make strings containing the paths to the test images.
    std::string path = ros::package::getPath("imageTutorial"); // this line finds where this package is on your computer without using /philip etc

    ssIm << path << "/src/image.jpg";      //here we just add the ending of the path.


    //do a check to make sure that ros found the folder on the computer.
    if(path.empty()){
        std::cout << "I do not know where imageTutorial is..." << std::endl;
        return -1;
    }

    //set up ros objects
    ros::NodeHandle node;
    image_transport::ImageTransport it_(node);


    //Subscribe to the thermal image topic.
    image_transport::Subscriber imSub = it_.subscribe("/camera",1, imageCallback);



    while(ros::ok()){
        ros::spinOnce();
    }


}

void imageCallback(const sensor_msgs::ImageConstPtr& msg){

    //get the image from the message
    cv::Mat image =  cv_bridge::toCvShare(msg, "bgr8")->image;
    cv::imshow("test", image);
    cv::waitKey(1);
    std::cout << ssIm.str();
    cv::imwrite(ssIm.str(),image);
    std::stringstream cmd;
    cmd << "cd ~/caffe && ./build/examples/cpp_classification/classification.bin \
           models/bvlc_reference_caffenet/deploy.prototxt \
           models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel \
           data/ilsvrc12/imagenet_mean.binaryproto \
           data/ilsvrc12/synset_words.txt ";
    cmd << ssIm.str();



    //std::cout << image.rows << std::endl;
    std::cout << exec(cmd.str().c_str());

}

