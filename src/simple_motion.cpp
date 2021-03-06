#include <schunk_kinematics/arm_kinematics.h>
#include <schunk_kinematics/arm_navigation.h>
#include <schunk_kinematics/sdh_navigation.h>

int main(int argc, char** argv)
{
/********************************************************
              related parameter description
  IK0 (7X1)    - Current joint position of arm
  IK  (7X1)    - Desired joint position of arm solved by ik
  Linklen(7x1) - vector to store the link lenth of the arm
  Goal(4x4)    - the end-effector desired Matrix

********************************************************/
    ros::init(argc, argv, "simple_motion");

    VectorXd seta(7), Linklen(7), IK0(7), IK(7), sdh_ik(8);
    MatrixXd Goal(4,4);

    IK0<<0.0,0.0,0.0,0.0,0.0,0.0,0.0;

    //Linklen<<0.167,0.0,0.328,0.0,0.277,0.0,0.179;
    Linklen<<0.3,0.0,0.328,0.0,0.276,0.0,0.2192;
    Goal<<0.0, 0.0, 1.0, 0.6,\
          1.0, 0.0, 0.0, 0.0,\
          0.0, 1.0, 0.0, 0.38,\
          0.0, 0.0, 0.0, 1.0;

    arm_navigation arm_move;
    control_msgs::FollowJointTrajectoryGoal arm_goal;

    inverse_kinematics(IK, Linklen, Goal, false, 0);
    minimum_energy(IK, IK0);
    arm_goal = arm_move.ExtensionTrajectory_lwa(IK);
    arm_goal.trajectory.points[0].time_from_start = ros::Duration(3);
    arm_move.startTrajectory_lwa(arm_goal);

  // Wait for trajectory completion
    while((!arm_move.getState_lwa().isDone()) && ros::ok())
    {
        usleep(5000);
    }

    sdh_navigation sdh_move;
    control_msgs::FollowJointTrajectoryGoal sdh_goal;
    sdh_ik<<0.0, -0.3, 0.3, -0.3, 0.3, -0.3, 0.3, 0.0;
    sdh_goal = sdh_move.ExtensionTrajectory_lwa(sdh_ik);
    sdh_goal.trajectory.points[0].time_from_start = ros::Duration(3);
    sdh_move.startTrajectory_lwa(sdh_goal);

    while((!sdh_move.getState_lwa().isDone())&& ros::ok())
    {
        usleep(5000);
    }

    return 0;
}
