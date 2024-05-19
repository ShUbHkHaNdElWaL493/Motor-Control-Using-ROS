/*
    Shubh Khandelwal
*/

#include <iostream>
#include "ros/ros.h"
#include "std_msgs/Int64.h"

void getPulseCount(const std_msgs::Int64& msg)
{
    std::cout<<"Pulse Count: "<<msg.data<<"\n";
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "Encoder_Pulses");
    ros::NodeHandle nh;

    ros::Subscriber Subscriber_Motor = nh.subscribe("Encoder", 1000, getPulseCount);

    ros::spin();

    return 0;

}