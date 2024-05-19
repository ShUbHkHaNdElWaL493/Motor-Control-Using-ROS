/*
    Shubh Khandelwal
*/

#include "ros/ros.h"
#include "std_msgs/Int64.h"

int main(int argc, char **argv)
{

    ros::init(argc, argv, "Motor_PWM");
    ros::NodeHandle nh;

    ros::Publisher Publisher_Motor = nh.advertise<std_msgs::Int64>("Motor", 1000);
    ros::Rate frequency(1);

    std_msgs::Int64 value;
    value.data = 0;

    while (ros::ok())
    {
        std::cout<<"Enter the speed of the motor.\n";
        std::cin>>value.data;
        Publisher_Motor.publish(value);
        ros::spinOnce();
        frequency.sleep();
    }

    return 0;

}