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
 #include <fstream>

#include <nav_msgs/Odometry.h>



using namespace std;
ofstream OutFile;//在头文件fstream中
 float px=0,ox=0;
  float py=0,oy=0;
  float pz=0,oz=0,ow=0;


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
                 ROS_INFO("pose to....................");
   		//  ROS_INFO("send data to UDP server %s:%d!\n", server_ip, port); 
              
             //OutFile<< px << " "<< py << " " << pz <<" " <<endl;
            // OutFile<< ox << " "<< oy << " " << oz <<  " " << ow <<" " <<endl;         
		 pcl::fromROSMsg(*input,*cloud_in);//把ROS格式转为Point
		 int M =  cloud_in->size();
       		  char send_buf[512] =""; 
		 ROS_INFO("size=%d.......................",M);
 		  for (int i = 0;i <M;i=i+2)
       		 {
    			 //UDP发送x,y,z数据
            		   sprintf(send_buf,"%d:x=%f,y=%f,z=%f,\r\n",i, cloud_in->points[i].x,cloud_in->points[i].y,cloud_in->points[i].z);
           		   int len = sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)); 
			   ROS_INFO("%d:x=%f,y=%f,z=%f,\r\n",i, cloud_in->points[i].x,cloud_in->points[i].y,cloud_in->points[i].z);
   			    OutFile<< (int)(cloud_in->points[i].x *100)<< " "<<(int)(cloud_in->points[i].y *100)<< " "<<(int)(cloud_in->points[i].z *100) << " " <<(int)(cloud_in->points[i+1].x *100)<< " "<<(int)(cloud_in->points[i+1].y *100)<< " "<<(int)(cloud_in->points[i+1].z *100) << " "<<endl;
    	         }
             	 OutFile<<  "\r\n" <<endl;
			//ROS_INFO("over to....................");
                // close(sockfd); 
       		 //ROS_INFO("len = %d\n", len);
	
     }
private:

     ros::NodeHandle nh;
     pcl::PointXYZ p;
    ros::Subscriber sub;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in {new pcl::PointCloud<pcl::PointXYZ>};
    
    

};


  void chatterCallback(const nav_msgs::Odometry cloud_msg){
 
			 px = cloud_msg.pose.pose.position.x;
             py = cloud_msg.pose.pose.position.y;
  			 pz = cloud_msg.pose.pose.position.z;
 			 ox = cloud_msg.pose.pose.orientation.x;
  			 oy = cloud_msg.pose.pose.orientation.y;
 			 oz = cloud_msg.pose.pose.orientation.z;
  			 ow = cloud_msg.pose.pose.orientation.w;
           ROS_INFO("odom to....................");
}


int main(int argc,char **argv)
{  
   
    ros::init(argc,argv,"cut_pointcloud5");
     ROS_INFO("ready...................!"); 
    OutFile.open("pose.txt",ios::out);//ios::out表示文件不存在则创建，若文件已存在则清空原内容;如果filename是string类型，需要用c_str()函数转换，因为filename的类型要求是const char *;
    ros::NodeHandle n2;
      ros::Subscriber sub = n2.subscribe("/wheel_odom",1000, chatterCallback);


    cut_pointcloud cut_pointcloud1;
    ros::spin();
}




