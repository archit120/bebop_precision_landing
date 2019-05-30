#ifndef ROS_H
#define ROS_H

#include "helpers.h"
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Empty.h>

extern Mat buffer_frame;
extern uint64_t fc;

void rqt_plot(float);
void publish_vel(MatrixXf vel);
Mat get_new_frame();
#endif