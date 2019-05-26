#ifndef ROS_H
#define ROS_H

#include "helpers.h"
#include <nav_msgs/Odometry.h>

extern Mat buffer_frame;
extern uint64_t fc;

void publish_vel(MatrixXf vel);
Mat get_new_frame();
#endif