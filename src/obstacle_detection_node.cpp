#include "ros/ros.h"
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>


ros::Publisher pub_cloud;


class process_point_cloud 
{
    const sensor_msgs::PointCloud2ConstPtr& input_cloud;
    float voxel_filter_size = 0.01;

public:

    sensor_msgs::PointCloud2 voxel_filtered_cloud;
    // sensor_msgs::PointCloud2 statistical_outlier_removed_cloud;

    process_point_cloud(const sensor_msgs::PointCloud2ConstPtr& cloud) : input_cloud(cloud) {}
    ~process_point_cloud() {}

    void voxel_filter()
    {
        // Container for original & filtered data
        pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2; 
        pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);
        pcl::PCLPointCloud2 cloud_filtered;

        // Convert to PCL data type
        pcl_conversions::toPCL(*input_cloud, *cloud);

        // Perform the actual filtering
        pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
        sor.setInputCloud(cloudPtr);
        sor.setLeafSize(voxel_filter_size, voxel_filter_size, voxel_filter_size);
        sor.filter(cloud_filtered);

        // Convert to ROS data type
        pcl_conversions::fromPCL(cloud_filtered, voxel_filtered_cloud);

    }

    // void statistical_outlier_removal()
    // {
    //     pcl::StatisticalOutlierRemoval<pcl::PCLPointCloud2> sor;
    //     sor.setInputCloud(voxel_filtered_cloud);
    //     sor.setMeanK(20); 
    //     sor.setStddevMulThresh(2); //2
    //     // sor.setNegative (true);
    //     sor.filter (statistical_outlier_removed_cloud);

    // }
    
};


void cloudCallback(const sensor_msgs::PointCloud2ConstPtr& input_cloud)
{
    process_point_cloud p1(input_cloud);
    p1.voxel_filter();
    pub_cloud.publish(p1.voxel_filtered_cloud);

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "obstacle_detection");
    ros::NodeHandle n;

    int queue_size = 1; // Queue size 1 so that latest information is passed on

    // Subscribes to the point cloud published from the camera
    ros::Subscriber sub_cloud = n.subscribe("/camera/depth/color/points", queue_size, cloudCallback);  

    // Publisher to publish the output point cloud 
    pub_cloud = n.advertise<sensor_msgs::PointCloud2> ("/obstacle_detection/point_cloud", 1);

    ros::spin();
    return 0;
}


