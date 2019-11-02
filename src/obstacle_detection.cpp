#include "ros/ros.h"
#include <sensor_msgs/PointCloud2.h>

void obstacleCallback(const sensor_msgs::PointCloud2ConstPtr& pcl_data)
{
  ROS_INFO("I heard");
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "obstacle_detection");
	ros::NodeHandle n;

	//Buffer size 1 so that latest info is passed on
	ros::Subscriber sub = n.subscribe("/camera/depth/color/points", 1, obstacleCallback);

	ros::spin();
	return 0;
}


