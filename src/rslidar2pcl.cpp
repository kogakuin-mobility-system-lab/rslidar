#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl_ros/point_cloud.h>

ros::Publisher pub;

void
cloud_cb (const sensor_msgs::PointCloud2ConstPtr& input)
{
  // Convert the sensor_msgs/PointCloud2 data to pcl/PointCloud
  pcl::PointCloud<pcl::PointXYZ> cloud;
  pcl::fromROSMsg (*input, cloud);

  

#if 0
  pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
  pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
  // Create the segmentation object
  pcl::SACSegmentation<pcl::PointXYZ> seg;


  // Optional
  seg.setOptimizeCoefficients (true);
  // Mandatory
  seg.setModelType (pcl::SACMODEL_PLANE);
  seg.setMethodType (pcl::SAC_RANSAC);

  double dist_th;
  ros::param::get("dist_th", dist_th);
  seg.setDistanceThreshold (dist_th);

  seg.setInputCloud (cloud.makeShared());
  seg.segment (*inliers, *coefficients);


  if(inliers->indices.size() == 0){
      std::cerr << "Could not estimate a planar model for the given data" << std::endl;
  }else{
    for (size_t i = 0; i < inliers->indices.size (); ++i) {
       std::cerr << inliers->indices[i] << "    " << cloud.points[inliers->indices[i]].x << " "
       << cloud.points[inliers->indices[i]].y << " "
       << cloud.points[inliers->indices[i]].z << std::endl;
      //cloud.points[inliers->indices[i]].r = 255;
      //cloud.points[inliers->indices[i]].g = 0;
      //cloud.points[inliers->indices[i]].b = 0;
      //cloud.points[inliers->indices[i]].a = 255;
    }
  }
#endif

  for(int i = 0 ; i < cloud.points.size() ; i++)
  {
      std::cerr << i << "  " << cloud.points[i].x << " " << cloud.points[i].y << " " << cloud.points[i].z << std::endl; 
  }


  pub.publish(cloud);

  // save pcd file
  pcl::io::savePCDFileASCII ("save.pcd", cloud);
}


int
main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "sample");
  ros::NodeHandle nh;

  // Set ROS param
  ros::param::set("dist_th", 0.1);

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("rslidar_points", 1, cloud_cb);

  // Create a ROS publisher for the model coefficients
  pub = nh.advertise<pcl::PointCloud<pcl::PointXYZ> > ("rslidar_pcl", 1);
  // Spin
  ros::spin ();
}
