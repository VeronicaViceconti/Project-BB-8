#include <iostream>
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include "std_msgs/String.h"
#include <tf2/transform_datatypes.h>

class Odom_to_tf{

private:
    ros::NodeHandle n;

    tf::TransformBroadcaster br;
    tf:: Transform transform;

public:

    void publish_tf(const nav_msgs::Odometry::ConstPtr &data){

        double x = data->pose.pose.position.x;
        double y = data->pose.pose.position.y;
        geometry_msgs::Quaternion theta = data->pose.pose.orientation;
        tf::Quaternion q;

        quaternionMsgToTF(theta, q);

        /* tf publishing */
        transform.setOrigin( tf::Vector3(x, y, 0.0) );

        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, data->header.stamp, "odom", "base_link"));
    }

    void init(){
        ros::Subscriber bagReader = n.subscribe<nav_msgs::Odometry>("ugv/odom", 1000, &Odom_to_tf::publish_tf, this);
        ros::spin();
    }
};

int main(int argc, char **argv){

    ros::init(argc, argv, "odom_to_tf");

    Odom_to_tf odom_to_tf;
    odom_to_tf.init();
    ROS_INFO("init tf_publisher finished!");


    return 0;
}