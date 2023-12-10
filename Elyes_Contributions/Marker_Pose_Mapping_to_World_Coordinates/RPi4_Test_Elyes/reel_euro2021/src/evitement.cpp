#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/PoseStamped.h"
#include <nav_msgs/Path.h>
#include <geometry_msgs/Twist.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;




class Subscriber1publisher
{
   public: 
  Subscriber1publisher(){
      if (ros::param::has("timer")){
      ros::param::get("timer", seuil);}
      else {
          seuil=2000;
      }
      if (ros::param::has("distance")){
      ros::param::get("distance", distance);}
      else {
          distance=0.3;
      }
      if (ros::param::has("delay")){
      ros::param::get("delay", delay);}
      else {
          delay=0.5;
      }
      if (ros::param::has("angle")){
      ros::param::get("angle", angle);}
      else {
         angle=90;
      }
      
      
      ROS_INFO("%s%d","Timer Param ==  ",seuil);
      ROS_INFO("%s%f","Distance Param ==  ",distance);
      ROS_INFO("%s%f","Delay Param ==  ",delay);
      ROS_INFO("%s%f","Angle ==  ",angle);
        
        timer=seuil;
        goal_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1000);
        cancel_publisher= n.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 1000);
        global= n.advertise<nav_msgs::Path>("/move_base/TebLocalPlannerROS/global_plan", 1000);
        local= n.advertise<nav_msgs::Path>("/move_base/TebLocalPlannerROS/local_plan", 1000);
        // las= n.advertise<sensor_msgs::LaserScan>("/scan2", 1000);
        
        sub_goal = n.subscribe("/move_base/current_goal", 1000, &Subscriber1publisher::goal_Callback,this);
        sub_scan = n.subscribe("/scan", 1000, &Subscriber1publisher::scan_Callback,this);
        sub_cmd = n.subscribe("/cmd_vel", 1000, &Subscriber1publisher::cmd_Callback,this);


  }

    void cancel_pub(){
      MoveBaseClient ac("move_base", true);
      ac.cancelAllGoals();
    }
    void cmd_Callback( const geometry_msgs::Twist msg){
      current_cmd=msg.linear.x;
    }

  void goal_Callback(const geometry_msgs::PoseStamped msg)
{   
    current_goal=msg;
}

void scan_Callback(sensor_msgs::LaserScan msg)
{    
    timer=timer+1;
      //ROS_INFO("%s%d","Timer = ",timer);
    int i=0;
   if(timer>seuil){

    for(auto range : msg.ranges)
  {
    float ang=msg.angle_min+i*msg.angle_increment*180/M_PI;
    if(ang>= 180-angle/2 && ang<= 180+angle/2){
      // msg.ranges[i]=4;
      if(range<distance  && current_cmd>=0){
      timer=0;
      ROS_INFO("%s%f%s%f","Detected crucial OBSTACLE < ",distance*100,"cm ====>  ",range);
      cancel_pub();
      nav_msgs::Path p;
      global.publish(p);
      local.publish(p);
      ros::Duration(delay).sleep();
      goal_pub.publish(current_goal);
      break;
    }}

    if((ang<=angle/2 )  || (ang>=360-angle/2)){
      // msg.ranges[i]=4;
      if(range<distance  && current_cmd<=0){
      timer=0;
      ROS_INFO("%s%f%s%f","Detected crucial OBSTACLE < ",distance*100,"cm ====>  ",range);
      cancel_pub();
      nav_msgs::Path p;
      global.publish(p);
      local.publish(p);
      ros::Duration(delay).sleep();
      goal_pub.publish(current_goal);
      break;
    }
    }
    
         i++; 
     }
       


    
  }
  
   
     
    //las.publish(msg);
 // ROS_INFO("%s","\n\n\n ");
};
    
  private:
  ros::NodeHandle n;
   ros::Publisher goal_pub;
    
  //  ros::Publisher las;
   ros::Publisher cancel_publisher;
    ros::Publisher global;
   ros::Publisher local;
   ros::Subscriber sub_goal ;
   ros::Subscriber sub_scan;
   ros::Subscriber sub_cmd;;
   geometry_msgs::PoseStamped current_goal;
   int timer;
   int seuil;
   float delay;
   float distance;
   float angle;
   float current_cmd;
   




};
       


int main(int argc, char **argv)
{  
  ros::init(argc, argv, "goal_node");
  Subscriber1publisher sub_pub;
  ros::spin();
  return 0;
}
