#ifndef IBVS_H
#define IBVS_H

#include <math.h>
#include "helpers.h"


using namespace cv;
using namespace std;
using namespace Eigen;

extern double cx;
extern double cy;
extern float fx;
extern float fy;


vector<Point2d> reference;

MatrixXf get_velocity(vector<Point2f> points, float depth);


#endif