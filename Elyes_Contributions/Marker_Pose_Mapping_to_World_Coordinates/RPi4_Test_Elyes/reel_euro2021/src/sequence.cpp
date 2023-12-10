#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base/move_base.h>
#include <reel_euro2021/goal.h>
#include <geometry_msgs/Twist.h>
#include <actionlib_msgs/GoalID.h>
#include <reel_euro2021/map.h>


double current_orientation = 0;
double current_x = 0;
double current_y = 0;
float xy = 0;
float yaw = 0;
float current_cmd = 0;
float somme_xy_theta = 0;
bool in_mission = false;
bool cancelled = false;

int t = 0;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

ros::Publisher cancel_publisher;
reel_euro2021::mapRequest map_req;
reel_euro2021::mapResponse map_resp;

ros::ServiceClient mapClient;


void goalSend(float x, float y, float theta)
{
    move_base_msgs::MoveBaseGoal goal;

    // we'll send a goal to the robot to move 1 meter forward
    if (theta < -180 || theta > 180)
    {
        ROS_INFO("Failed to send goal ===>  respect the angles limits 180 & -180");
    }
    else
    {
        MoveBaseClient ac("move_base", true);
        while (!ac.waitForServer(ros::Duration(5.0)))
        {
            ROS_INFO("Waiting for the move_base action server to come up");
        }
        theta = theta * M_PI / 180;

        goal.target_pose.header.frame_id = "odom";

        goal.target_pose.header.stamp = ros::Time::now();

        goal.target_pose.pose.position.x = x;
        goal.target_pose.pose.position.y = y;
        goal.target_pose.pose.position.z = 0;
        goal.target_pose.pose.orientation.x = 0;
        goal.target_pose.pose.orientation.y = 0;
        goal.target_pose.pose.orientation.z = sin(theta / 2);
        goal.target_pose.pose.orientation.w = cos(theta / 2);
        ROS_INFO("%s", "got this");
        ROS_INFO("%f%f%f",goal.target_pose.pose.position.x , goal.target_pose.pose.position.y ,goal.target_pose.pose.orientation.z );

        ac.sendGoal(goal);
        while (cancelled == false && (abs(current_x - x) > xy || abs(current_y - y) > xy || abs((current_orientation * M_PI / 180) - theta) > yaw))
        {
            ros::spinOnce();
        }
        cancelled = false;
        ROS_INFO("%s", "got this");
    }
}
void cancel_pub()
{
    actionlib_msgs::GoalID id;
    cancel_publisher.publish(id);
}
void odom_callback(reel_euro2021::goal goal)
{

    // MoveBaseClient ac("move_base", true);

    t++;
    somme_xy_theta += abs(goal.x - current_x) * 1000 + abs(goal.y - current_y) * 1000 + abs(goal.orientation - current_orientation);
    current_x = goal.x;
    current_y = goal.y;
    current_orientation = goal.orientation;

    if (t == 100)
    {
        if (in_mission)
        {
            ROS_INFO("%f%s%f", current_cmd, "        ", somme_xy_theta);

            if (abs(current_cmd) > 0.1 && somme_xy_theta < 40)
            {

                ROS_INFO("ne7bsssssss");
                cancel_pub();
                cancelled = true;
            }
        }

        t = 0;
        somme_xy_theta = 0;
    }
}

void cmd_callback(geometry_msgs::Twist msg)
{
    current_cmd = msg.linear.x + msg.linear.y + msg.linear.y + msg.angular.z;
}

void obstacleLayer(bool cmd)
{
    dynamic_reconfigure::ReconfigureRequest req;
    dynamic_reconfigure::ReconfigureResponse resp;
    dynamic_reconfigure::Config conf;

    dynamic_reconfigure::BoolParameter enabled;
    enabled.name = "enabled";
    enabled.value = cmd;
    conf.bools.push_back(enabled);
    req.config = conf;

    ros::service::call("/move_base/global_costmap/inflation_layer/set_parameters", req, resp);
}
void hitObject(float x, float y, float theta, float distance)
{
    map_req.action=true;
    map_req.r=0;
    mapClient.call(map_req,map_resp);
    goalSend(x, y, theta);
    in_mission = true;
    ROS_INFO("danger");
    
    map_req.action=false;
    map_req.r=99;
    mapClient.call(map_req,map_resp);
    // while(map_resp.result.length() <2);

    goalSend(x + cos(theta * M_PI / 180) * distance, y + sin(theta * M_PI / 180) * distance, theta);
    in_mission = false;
    ROS_INFO("%s", "in front of object");
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "sequence");
    ros::NodeHandle n;
    ros::Subscriber sub_odom = n.subscribe("/better_odom", 1000, odom_callback);
    ros::Subscriber sub_cmd = n.subscribe("/cmd_vel", 1000, cmd_callback);
    cancel_publisher = n.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 10);
    mapClient = n.serviceClient<reel_euro2021::map>("/updateMapSrv");



    if (ros::param::has("/move_base/TebLocalPlannerROS/xy_goal_tolerance"))
    {
        ros::param::get("/move_base/TebLocalPlannerROS/xy_goal_tolerance", xy);
    }
    else
    {
        xy = 0.02;
    }

    if (ros::param::has("move_base/TebLocalPlannerROS/yaw_goal_tolerance"))
    {
        ros::param::get("move_base/TebLocalPlannerROS/yaw_goal_tolerance", yaw);
    }
    else
    {
        yaw = 0.02;
    }

    ROS_INFO("%s%f", "XY tolerance Param ==  ", xy);
    ROS_INFO("%s%f", "YAW tolerance Param ==  ", yaw);

    /*  goalSend(0.5, 0, 0);
      ros::Duration(1).sleep();
      goalSend(0, 0, 0);
      ros::Duration(1).sleep();
      goalSend(0.5, 0.5, 90);
      ros::Duration(1).sleep();
      goalSend(0, 0, 0);*/
    // hitObject(0.26, 0.757, 135, 0.2);
    // hitObject(2.0, -0.590, -90, 0.2);

      float xxx,yyy,ooo,fff;
      while(xxx!=99){
    std::cout<<"Donner la pos:"<<std::endl;
      std::cin>>xxx>>yyy>>ooo>>fff;
      hitObject(xxx,yyy,ooo,fff);

      }
      
    // obstacleLayer(true);.59
    // ros::spin();

    return 0;
}