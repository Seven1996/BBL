#include<ros/ros.h>
#include<std_msgs/Float64MultiArray.h>
#include<sensor_msgs/JointState.h>

#include<iostream>
#include<cmath>

#include<Eigen/Core>
#include<Eigen/Dense>

Eigen::Matrix<double,6,6> Ad_T(Eigen::Matrix<double,4,4> T)
{
    Eigen::Matrix<double,3,3> p33,zeros33;
    p33<<0,-T(2,3),T(1,3),
       T(2,3), 0,-T(0,3),
       -T(1,3),T(0,3),0;
    Eigen::Matrix<double,3,3> R=T.block(0,0,3,3);
    Eigen::Matrix<double,6,6> AdT;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++)  zeros33(i,j)=0;
    }
    AdT.block(0,0,3,3)=R;
    AdT.block(3,3,3,3)=R;
    AdT.block(0,3,3,3)=p33*R;
    AdT.block(3,0,3,3)=zeros33;
    return AdT;
}


Eigen::Matrix<double,6,6> jacobian(float theta[])
{
    Eigen::Matrix<double,6,1> twist[6];

    twist[0]<<0,0,0,0,0,-1;
    twist[1]<<-0.2261,0,0,0,1,0;
    twist[2]<<-0.6011,0,0,0,1,0;
    twist[3]<<0,0,0,0,0,-1;
    twist[4]<<-1.0361,0,0,0,1,0;
    twist[5]<<0,0,0,0,0,-1;
//    float theta[6];
//    for(int i=0;i<6;i++) theta[i]=0.5;
    Eigen::Matrix<double,3,3> SO3[6];
    Eigen::Matrix<double,4,4> SE3[6];
    Eigen::Matrix<double,3,3> W;
    Eigen::Matrix<double,3,3> I33;
    Eigen::Matrix<double,3,1> p[6];
    Eigen::Matrix<double,3,1> v;
    Eigen::Matrix<double,1,4> zeros_one;
    I33<<1,0,0,0,1,0,0,0,1;
    zeros_one<<0,0,0,1;
    
    for(int i=0;i<6;i++){
        //SO3[i]= 
    W<<0,-twist[i](5,0),twist[i](4,0),
       twist[i](5,0), 0,-twist[i](3,0),
       -twist[i](4,0),twist[i](3,0),0;
    v<<twist[i](0,0),twist[i](1,0),twist[i](2,0);
    SO3[i]=I33+sin(theta[i])*W+(1-cos(theta[i]))*W*W;

    p[i]=(theta[i]*I33+(1-cos(theta[i]))*W+(theta[i]-sin(theta[i]))*W*W)*v;
    SE3[i].block(0,0,3,3)=SO3[i];
    SE3[i].block(0,3,3,1)=p[i];
    SE3[i].block(3,0,1,4)=zeros_one;
}
   // SO3=eye(3,3)+sin(theta)*W+(1-cos(theta))*W^2;

    Eigen::Matrix<double,4,4> I44;
    Eigen::Matrix<double,6,6> J;
    I44<<1,0,0,0,
         0,1,0,0,
  	 0,0,1,0,
	 0,0,0,1;
    for(int i=0;i<6;i++){
      J.block(0,i,6,1)=Ad_T(I44)*twist[i];
      I44=SE3[i]*I44;
    } 
//    std::cout<<J<<std::endl;
//    std::cout<<J.inverse()<<std::endl;
    return J;
}



void callback(const sensor_msgs::JointState::ConstPtr &msg)
{
     float theta[6];
     for(int i=0;i<6;i++)  theta[i]=msg->position[i];        
     Eigen::Matrix<double,6,6> J=jacobian(theta);     
     Eigen::Matrix<double,6,6> M=J*J.transpose();       
  //  ROS_INFO("%f",msg->position[2]);
    // ROS_INFO("the");
     std::cout<<"the determinant of the M is"<<std::endl;
     std::cout<<M.determinant()<<std::endl; 
     std::cout<<"the eigenvalues of the M is"<<std::endl; 
     Eigen::SelfAdjointEigenSolver<Eigen::Matrix<double,6,6> >  es(M);
     Eigen::Matrix<double,6,1> values=es.eigenvalues();
     std::cout<<es.eigenvalues()<<std::endl;
}


int main(int argc,char**argv)
{
    ros::init(argc,argv,"test_node");
    ros::NodeHandle nh;
    ROS_INFO("1");
    //ros::Publisher pub=nh.advertise<std_msgs::Float64MultiArray>("/velocity_controller_group/command",10);
    ros::Subscriber sub=nh.subscribe<sensor_msgs::JointState>("/joint_states",10,callback);
    std_msgs::Float64MultiArray msg;
    float joint_velocity[7];
   ros::Rate looprate(10);
   float v=0.5;
   int i=-1;
   ros::spin();
    return 0;
}
