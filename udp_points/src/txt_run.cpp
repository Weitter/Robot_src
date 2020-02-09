#include <boost/thread/thread.hpp>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <fstream>
#include <iostream>
#include <string>



using namespace std;




int main(int argc, char **argv){
  ros::init(argc, argv, "txt_run_node");
  int a[50];
  geometry_msgs::Twist cmdvel_;
  ros::NodeHandle n_;
  ros::Publisher pub_;
  ros::Rate loop_rate(10);

/*
	前进:	 cmdvel_.linear.x = 0.2;
		 cmdvel_.angular.z = 0.0;

	后退:	 cmdvel_.linear.x = -0.2;
		 cmdvel_.angular.z = 0.0;

	左:	 cmdvel_.linear.x =  0.0;
		 cmdvel_.angular.z = 0.5;

	右:	 cmdvel_.linear.x =  0.0;
		 cmdvel_.angular.z = -0.5;

	停:	 cmdvel_.linear.x =  0.0;
		 cmdvel_.angular.z = 0.0;

*/

  ifstream input( "run.txt", ios::in );
    if( ! input )
    {
        cerr << "Open input file error!" << endl;
        exit( -1 );
    }
    for(int i=0;i<50;i++)
		input>>a[i];
     for(int i=0;i<50;i++)
     ROS_INFO("%d/r/n",a[i]);
 

  pub_ = n_.advertise<geometry_msgs::Twist>("cmd_vel", 1);

   cmdvel_.linear.x = 0.0;
   cmdvel_.angular.z = 0.0;

  while (ros::ok()){
   
  for(int i=0;i<50;i++)
       {
	if(a[i] == 1)
     		{
		 cmdvel_.linear.x = 0.2;
		 cmdvel_.angular.z = 0.0;
   		 pub_.publish(cmdvel_);
   		 ros::spinOnce();
   		 loop_rate.sleep();
  		 ROS_INFO("1....................");
	}
        else if (a[i] == 2){
		 cmdvel_.linear.x = 0.0;
		 cmdvel_.angular.z = 0.0;
		 pub_.publish(cmdvel_);
   		 ros::spinOnce();
   		 loop_rate.sleep();
  		 ROS_INFO("2....................");
}
	
     		
}

   
  }
  return 0;
}


