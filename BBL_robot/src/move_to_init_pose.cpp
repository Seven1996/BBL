#include <moveit/move_group_interface/move_group_interface.h>
// #include <moveit/planning_scene_interface/planning_scene_interface.h>
// #include <moveit/robot_model_loader/robot_model_loader.h>
// #include <moveit/robot_model/robot_model.h>
// #include <moveit/robot_state/robot_state.h>
#include <geometry_msgs/Pose.h>

#include<ros/ros.h>


int main(int argc,char **argv)
{
    ros::init(argc,argv,"move2initpose_node");
    ros::NodeHandle nh;
    moveit::planning_interface::MoveGroupInterface move_group("arm");

    // moveit::planning_interface::MoveGroup move_group("arm");
    ros::Duration(0.5).sleep();  
   // geometry_msgs::Pose pose_now = move_group.getCurrentPose(move_group.getEndEffectorLink()).pose;
  //  geometry_msgs::Pose pose_now = move_group.getCurrentPose("link").pose;
//   
    move_group.setNamedTarget("init_pose");
     move_group.move();

    //std::cout<<pose_now<<std::endl; 
    return 0;

}
