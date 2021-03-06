#include "ros/ros.h"                          // ROS Default Header File
#include <tf/tf.h>
#include "move_square/square_service.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "geometry_msgs/Twist.h"
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <math.h>
#define PI 3.14159265

struct Quaternion {
    double w, x, y, z;
};

struct position
{
    double x;
    double y;
};

struct EulerAngles {
    double roll, pitch, yaw;
};


class Mover
{
    private:
        EulerAngles angles;//the euler angles of the robot 
        Quaternion q;//the quaternian angles of the robot
        position location;//the current location of the robot

        float x_to_goal;// the x distance between the current position and the goal
        float y_to_goal;// the y distance between the current position and the goal
        float angle_to_goal;// the angle required to be turned until the robot is facing the goal point
        float angle_error;
        float angle_error2;
        position goals[4];
        int repitions;
        int distance;//the absolute distance between the current point and the goal
        int repetition_counter; // a counter to count how many times the robot has made a full square
        int goal_index; // the index used to iterate over the goal 

        float angle_threshold;
        float distance_threshold;

    public:
        ros::Subscriber sub; 
        ros::NodeHandle n;
        ros::ServiceServer square_service;
        ros::Publisher pub;
        geometry_msgs::Twist msgg;

    Mover(){
        
        ROS_INFO("Intiating the move square command");
        sub= n.subscribe("/odom",10,&Mover::OdomCallback,this);
        // ROS_INFO("subscribtion succeeded");
        square_service = n.advertiseService("square_service",&Mover::move,this);
        pub = n.advertise<geometry_msgs::Twist>("cmd_vel",1);
        
        angle_to_goal=0;
        goal_index=0;

        repetition_counter=0;
        
        angle_threshold=0.5;
        distance_threshold=0.07;
    // these list of points make a square . we can make the square bigger by simply muliplying each one of those points with a constant (side length)
        goals[0].x=1;
        goals[0].y=0;
        
        goals[1].x=1;
        goals[1].y=1;
        
        goals[2].x=0;
        goals[2].y=1;

        goals[3].x=0;
        goals[3].y=0;
        
    }

    float get_distance()//simple function to calculate distance between two functions 
    {
        return sqrt(pow(distance * goals[goal_index].x-location.x,2) + pow( distance * goals[goal_index].y-location.y,2));
    }

void OdomCallback(const nav_msgs::OdometryConstPtr &msg)
    {
        
        location.x = msg->pose.pose.position.x;  // get the robot x pose from the odom topic
        location.y= msg->pose.pose.position.y;  // get the robot y pose from the odom topic
    
        tf::Quaternion q(
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w);
            
        tf::Matrix3x3 m(q);
        double roll, pitch, yaw;
        m.getRPY(roll, pitch, yaw);
//get the roll pitch and yaw and convert them to degrees
        angles.pitch=pitch * 180.0/PI;
        angles.roll=roll * 180.0/PI;
        angles.yaw=yaw * 180.0/PI;
    }



    bool move(move_square::square_service::Request &request,move_square::square_service::Response &response)
    {
        ROS_INFO("Move Square called");
        
        ros::Rate loop_rate(50);
        repitions=request.repetitions;
        distance=request.side;
            
            while(ros::ok())
            {
                if (repetition_counter==repitions)//check if we satisfied the full number of repitions
                {
                    response.success=true;
                    return true;
                } 
                x_to_goal = distance * goals[goal_index].x-location.x;
                y_to_goal = distance * goals[goal_index].y-location.y;   

                // we are tracking the error with two varables becasuet the angle refernce is changing with each point
                if(angles.yaw<0){angles.yaw+=360;}
                angle_error=angle_to_goal-angles.yaw;
                if(angle_error<0){angle_error+=360;}

                // these condtions make sure that the robot is near the goal and facing it
                bool small_distance=(get_distance()<distance_threshold);
                bool facing_correctly=(angle_error <angle_threshold || 360-angle_error<angle_threshold);
                // a way to represnt all the calculated data for debugging
                ROS_INFO_STREAM("----------------------------------------------------------");
                ROS_INFO_STREAM("----------------------------------------------------------");
                ROS_INFO_STREAM("angle_error : "<< angle_error);
                ROS_INFO_STREAM("");
                ROS_INFO_STREAM("angle to goal : "<< angle_to_goal);
                ROS_INFO_STREAM("current angle "<<angles.yaw);
                ROS_INFO_STREAM("");
                ROS_INFO_STREAM("X to goal : "<<x_to_goal);
                ROS_INFO_STREAM("Y to goal : "<<y_to_goal);
                ROS_INFO_STREAM("distance : "<< get_distance());
                ROS_INFO_STREAM("");
                ROS_INFO_STREAM("current location : "<<"("<<location.x<<" , "<<location.y<< ")");
                ROS_INFO_STREAM("goal : "<<"("<<distance*goals[goal_index].x<<" , "<<distance*goals[goal_index].y<< ")");
                ROS_INFO_STREAM("");
                ROS_INFO_STREAM("repitions : "<< repitions);
                ROS_INFO_STREAM("repetition_counter : "<<repetition_counter);
                ROS_INFO_STREAM("goal_index : "<<goal_index);
                ROS_INFO_STREAM("----------------------------------------------------------");
                ROS_INFO_STREAM("----------------------------------------------------------");

                if (small_distance)// if we reached the goal update the goal to make it to the next point
                {
                    ROS_INFO_STREAM("changing goal");
                    goal_index++;
                    angle_to_goal+=90;
                    if (goal_index==4)
                    {
                        angle_to_goal=0;
                        goal_index=0;
                        repetition_counter++;
                    }
                }

                else if(!facing_correctly)//check if the rorbot is facing the goal
                {
                    ROS_INFO_STREAM("correcting orientation");
                    msgg.linear.x=0.0;
                    if (360-angle_error<90)
                    {
                        msgg.angular.z=-0.2;    
                    }
                    else
                    {
                        msgg.angular.z=0.2;    
                    }
                    
                }
                else if (!small_distance)
                {
                    ROS_INFO_STREAM("moving to goal");
                    msgg.linear.x=0.22;
                    msgg.angular.z=0.0;    
                }
                pub.publish(msgg);
                ROS_INFO_STREAM("linear vel : "<<msgg.linear.x<< "angular vel : "<<msgg.linear.x);
                ros::spinOnce();
                loop_rate.sleep();
            }

    }
};


int main(int argc, char **argv)
{
    ros::init(argc, argv, "mover");
    Mover mover;

    ros::spin();
    
    return 0;


}
