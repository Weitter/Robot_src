#include <iostream>
#include <ros/ros.h>
// PCL specific includes
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
 
using namespace std;
class cut_pointcloud
{
public:
    cut_pointcloud()
    {
	
        sub = nh.subscribe("rfans_driver/rfans_points",1,&cut_pointcloud::call_back,this);
	 }
     void call_back(const sensor_msgs::PointCloud2ConstPtr input)
     {

     		 unsigned short port = 8235; //端口号   
      		 char *server_ip = "192.168.43.49"; //IPV4
      		 int sockfd;  
      
   		 struct sockaddr_in dest_addr; 
   		 sockfd = socket(AF_INET, SOCK_DGRAM, 0);   
  		  if(sockfd < 0) 
   		 { 
       			 perror("socket"); 
      			  exit(-1); 
   		 } 
   		 bzero(&dest_addr, sizeof(dest_addr));   
  		 dest_addr.sin_family = AF_INET;       
    		 dest_addr.sin_port   = htons(port);   
   		 inet_pton(AF_INET, server_ip, &dest_addr.sin_addr);   
 
   		//  ROS_INFO("send data to UDP server %s:%d!\n", server_ip, port); 
        
		 pcl::fromROSMsg(*input,*cloud_in);//把ROS格式转为Point
		 int M =  cloud_in->size();
       		  char send_buf[512] =""; 
		 ROS_INFO("size=%d.......................",M);
 		  for (int i = 0;i <M;i++)
       		 {
    			 //UDP发送x,y,z数据
            		   sprintf(send_buf,"%d:x=%f,y=%f,z=%f,\r\n",i, cloud_in->points[i].x,cloud_in->points[i].y,cloud_in->points[i].z);
           		   int len = sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)); 
			   ROS_INFO("%d:x=%f,y=%f,z=%f,\r\n",i, cloud_in->points[i].x,cloud_in->points[i].y,cloud_in->points[i].z);
    	         }
                // close(sockfd); 
       		 //ROS_INFO("len = %d\n", len);
	
     }
private:

     ros::NodeHandle nh;
     pcl::PointXYZ p;
    ros::Subscriber sub;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in {new pcl::PointCloud<pcl::PointXYZ>};
    
    

};


int main(int argc,char **argv)
{  
   
    ros::init(argc,argv,"cut_pointcloud4");
     ROS_INFO("ready...................!"); 
    cut_pointcloud cut_pointcloud1;
    ros::spin();
}




