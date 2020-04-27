#include "ros/ros.h"                         // ROS Default Header File
#include "move_square/square_service.h"
#include "move_square/square_serviceRequest.h"
#include "move_square/square_serviceResponse.h"
#include <iostream>
#include <cstdlib>


int main(int argc, char **argv)
{
    ros::init(argc, argv, "move_square_client");
    ros::NodeHandle n;

    ROS_INFO("passing the service command");

    ros::ServiceClient square_service = n.serviceClient<move_square::square_service>("square_service");

    move_square::square_serviceRequest req;
    move_square::square_serviceResponse res;
    
    req.side = atoll(argv[1]);
    req.repetitions = atoll(argv[2]);
    square_service.call(req, res);

    ros::spin();

    return 0;
}