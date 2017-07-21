#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>
#include <sensor_msgs/Joy.h>
#include <sstream>
#include <iostream>



#include <iostream>
#include <string>


ros::Publisher turnPub ;
ros::Publisher forwardPub ;
ros::Publisher modePub;
void joyCallback(const sensor_msgs::JoyConstPtr& msg);
int main(int argc, char** argv){





    ros::init(argc, argv, "rc_node");

    //make strings containing the paths to the test images.
    std::string path = ros::package::getPath("imageTutorial"); // this line finds where this package is on your computer without using /philip etc



    //do a check to make sure that ros found the folder on the computer.
    if(path.empty()){
        std::cout << "I do not know where imageTutorial is..." << std::endl;
        return -1;
    }

    //set up ros objects
    ros::NodeHandle node;
    ros::Subscriber joySub = node.subscribe("/joy",1,&joyCallback);
    turnPub = node.advertise<std_msgs::Float32>("/turn",1);
    forwardPub = node.advertise<std_msgs::Float32>("/forward",1);
    modePub = node.advertise<std_msgs::Int32>("/mode",1);



    while(ros::ok()){
        ros::spinOnce();
    }


}

void joyCallback(const sensor_msgs::JoyConstPtr &msg){

    float forward = msg->axes[3];
    float turn = msg->axes[0];
    int shouldMode = 0;
    static int mode = 0;
    if(msg->buttons[2] == 1){
        mode = 1;
        shouldMode = 1;
    }
    if(msg->buttons[0] == 1){
        mode = 2;
        shouldMode = 1;
    }
    std_msgs::Float32 forwardMsg;
    std_msgs::Float32 turnMsg;
    std_msgs::Int32 modeMsg;
    forwardMsg.data = forward;
    turnMsg.data = turn;
    modeMsg.data = mode;
    if(mode == 1){
        turnPub.publish(turnMsg);
        forwardPub.publish(forwardMsg);
    }
    if(shouldMode){
        modePub.publish(modeMsg);
    }


}

