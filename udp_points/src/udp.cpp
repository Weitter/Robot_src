#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <fstream>
using namespace std;


   
 ofstream OutFile;//在头文件fstream中

void chatterCallback(const nav_msgs::Odometry cloud_msg)
{
  float px=0,ox=0;
  float py=0,oy=0;
  float pz=0,oz=0,ow=0;
 
   
  px = cloud_msg.pose.pose.position.x;
  py = cloud_msg.pose.pose.position.y;
  pz = cloud_msg.pose.pose.position.z;
  ox = cloud_msg.pose.pose.orientation.x;
  oy = cloud_msg.pose.pose.orientation.y;
  oz = cloud_msg.pose.pose.orientation.z;
  ow = cloud_msg.pose.pose.orientation.w;
   OutFile<< px << " "<< py << " " << pz <<endl;
   OutFile<< ox << " "<< oy << " " << oz <<  " " << ow <<endl;         
  ROS_INFO("position : px=%f,py=%f,pz=%f\r\n", px,py,pz);
  ROS_INFO("orientation :ox=%f,oy=%f,oz=%f,ow=%f\r\n", ox,oy,oz,ow);
  

    
   //ROS_INFO("ok....................");
 
 
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "udp_points1");
  ros::NodeHandle n2;
  
  OutFile.open("odom.txt",ios::out);//ios::out表示文件不存在则创建，若文件已存在则清空原内容;如果filename是string类型，需要用c_str()函数转换，因为filename的类型要求是const char *;

  ros::Subscriber sub = n2.subscribe("/wheel_odom",1000, chatterCallback);
  ROS_INFO("Ready to....................");
 // OutFile.close(); 
  ros::spin();

  return 0;
}
