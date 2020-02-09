#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h> 
#include <boost/thread/thread.hpp>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <fstream>
#include <iostream>
#include <string>



using namespace std;



int main(int argc, char *argv[])
{
	int client_sockfd;
	int len,a,flag=0;
	int ispeed = 0, iaix = 0, iback = 0;
	

	struct sockaddr_in remote_addr; //服务器端网络地址结构体
	char buf[2048];  //数据传送的缓冲区
	

	ros::init(argc, argv, "client_run_node");
	geometry_msgs::Twist cmdvel_;
  	ros::NodeHandle n_;
 	 ros::Publisher pub_;
  	ros::Rate loop_rate(10);
         pub_ = n_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
        
  	cmdvel_.linear.x = 0.0;
   	cmdvel_.angular.z = 0.0;
      
        memset(buf, 0, sizeof(buf));
	memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零
	remote_addr.sin_family=AF_INET; //设置为IP通信
	remote_addr.sin_addr.s_addr=inet_addr("192.168.50.116");//服务器IP地址192.168.6.2
	remote_addr.sin_port=htons(5050); //服务器端口号
	
	/*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
	if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
	{
		ROS_INFO("socket error");
		return 1;
	}
	
	/*将套接字绑定到服务器的网络地址上*/
	if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
	{
		ROS_INFO("connect error");
		return 1;
	}
	else{
		/*循环的发送接收信息并打印接收信息（可以按需发送）--recv返回接收到的字节数*/
	
	while(ros::ok())
	{   
 		
		        memset(buf, 0, sizeof(buf));
			len=recv(client_sockfd,buf,BUFSIZ,0);
			if (len == 0) {
				close(client_sockfd);
				//WSACleanup();
				return -1;
			}
			else if (len == (-1)) {
				printf("recv() Failed:\n");
				close(client_sockfd);
				//WSACleanup;
				return -1;
			}
			else {
				
				 memcpy(&ispeed, buf, sizeof(ispeed));
				 memcpy(&iaix, buf + sizeof(ispeed), sizeof(iaix));
                                 memcpy(&iback, buf + sizeof(ispeed)+sizeof(iaix), sizeof(iback));
				 ROS_INFO("speed:%d, aix: %d, back:%d\n", ispeed, iaix, iback);//333 161138

				
					if(ispeed > 0 &&ispeed <= 60)
						{
						cmdvel_.linear.x = 0.15;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(ispeed > 60 && ispeed <= 120)
						{
						cmdvel_.linear.x = 0.2;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(ispeed > 120 && ispeed <= 180)
						{
						cmdvel_.linear.x = 0.25;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(ispeed > 180 && ispeed <= 240)
						{
						cmdvel_.linear.x = 0.3;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(ispeed > 240 && ispeed <= 400)
						{
						cmdvel_.linear.x = 0.35;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
				
				
					if(iback > 0 && iback <= 60)
						{
						cmdvel_.linear.x = -0.15;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(iback > 60 && iback <= 120)
						{
						cmdvel_.linear.x = -0.2;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(iback > 120 && iback <= 180)
						{
						cmdvel_.linear.x = -0.2;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(iback > 180 && iback <= 240)
						{
						cmdvel_.linear.x = -0.25;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(iback > 240 && iback <= 400)
						{
						cmdvel_.linear.x = -0.3;
		 		 		cmdvel_.angular.z = 0.0;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
				
				
					if(iaix >= -161138 && iaix < -61138)
						{
						cmdvel_.linear.x = 0.0;
		 		 		cmdvel_.angular.z = -0.6;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(iaix >= -61138 && iaix < 0)
						{
						cmdvel_.linear.x = 0.0;
		 		 		cmdvel_.angular.z = -0.3;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(iaix > 0 && iaix < 61138)
						{
						cmdvel_.linear.x = 0.0;
		 		 		cmdvel_.angular.z = 0.3;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
					else if(iaix >= 61138 && iaix <= 161138)
						{
						cmdvel_.linear.x = 0.0;
		 		 		cmdvel_.angular.z = 0.6;
   						pub_.publish(cmdvel_);
   				 		ros::spinOnce();
   		 		 		loop_rate.sleep();
						}
				

				memset(buf, 0, sizeof(buf));

			}
	}
}
	

	
	/*关闭套接字*/
	close(client_sockfd);
    
	return 0;
}


