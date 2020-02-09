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
	int ret;
    	

	struct sockaddr_in remote_addr; //服务器端网络地址结构体
	char buf[2048];  //数据传送的缓冲区
	 fd_set fd;
   	 FD_ZERO(&fd);
   	 FD_SET(0,&fd);
	struct timeval timeout = { 0, 200 };

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
  	//判断输入的参数是否正确
	if (argc < 2) {
		//提示在命令行中输入服务器IP地址
		remote_addr.sin_addr.s_addr=inet_addr("192.168.50.116");//服务器IP地址
		
	}else
		remote_addr.sin_addr.s_addr=inet_addr(argv[1]);//服务器IP地址
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
			FD_SET(0,&fd);
    			FD_SET(client_sockfd,&fd);
			ret = select(client_sockfd+1,&fd,NULL,NULL,&timeout);
			
		 if(ret){
			
			len=recv(client_sockfd,buf,BUFSIZ,0);
			if (len == 0) {
				client_sockfd=socket(PF_INET,SOCK_STREAM,0);
				connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr));
				//close(client_sockfd);
				
				return -1;
			}
			else if (len == (-1)) {
				client_sockfd=socket(PF_INET,SOCK_STREAM,0);
				connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr));
				//close(client_sockfd);
				return -1;
			}
			else {
				
					 memcpy(&ispeed, buf, sizeof(ispeed));
					 memcpy(&iaix, buf + sizeof(ispeed), sizeof(iaix));
                               	         memcpy(&iback, buf + sizeof(ispeed)+sizeof(iaix), sizeof(iback));
					 ROS_INFO("liner:%f  angle:%f\n",  ((float)ispeed-(float)iback)/1665,-(float)iaix/268563);//333 161138
					cmdvel_.linear.x = ((float)ispeed-(float)iback)/1665;
		 		 	cmdvel_.angular.z = -(float)iaix/268563;
   					pub_.publish(cmdvel_);
   				 	ros::spinOnce();
   		 		 	loop_rate.sleep();
					memset(buf, 0, sizeof(buf));
					 continue;
			}

			}
 		else{
			        ROS_INFO("liner:0  angle:0\n");
				cmdvel_.linear.x = 0.;
		 		cmdvel_.angular.z = 0.;
   				pub_.publish(cmdvel_);
   				ros::spinOnce();
   		 		loop_rate.sleep();
				memset(buf, 0, sizeof(buf));
				 continue;			
			}
			


	}
}
	

	
	/*关闭套接字*/
	close(client_sockfd);
    
	return 0;
}


