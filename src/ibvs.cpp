#include <stdio.h>
#include <iostream>
#include "helpers.h"
#include "ibvs.h"

//using Eigen::completeOrthogonalDecomposition
//sift parameters for detection(f2d)
double cx = 0;
double cy = 0;
float fx = 1;
float fy = 1;

float error_threshold = 0.1;

int lambda = 1; //for setting the extra speed of camera

vector<Point2d> reference;

MatrixXf get_velocity(vector<Point2f> points, float depth)
{

    int number = reference.size();

    MatrixXf velocity(4, 1);
    MatrixXf L1(1, 4);
    L1.resize(2 * reference.size(), 4);
    MatrixXf LInv(4, 1);
    LInv.resize(4, 2 * reference.size());

    MatrixXf s_ref(1, 1);
    s_ref.resize(2 * reference.size(), 1);
    MatrixXf s(1, 1);
    s.resize(2 * reference.size(), 1);
    int i = 0;
    for (int i = 0; i < reference.size(); i += 2)
    {
        s_ref(i, 0) = (reference[i].x - cx) / fx;
        s_ref(i + 1, 0) = (reference[i].y - cy) / fy;
    }

    auto pt = points;
    auto d = depth;

    for (int i = 0; i < reference.size(); i += 2)
    {
        s(i, 0) = (pt[i].x - cx) / fx;
        s(i + 1, 0) = (pt[i].y - cy) / fy;
    }

    for (int i = 0; i < reference.size(); i += 2)
    {
        float x = (pt[i].x - cx) / fx;
        float y = (pt[i].y - cy) / fy;
        L1(i, 0) = -1 / d;
        L1(i, 1) = 0;
        L1(i, 2) = x / d;
        L1(i, 3) = y;

        L1(i + 1, 0) = 0;
        L1(i + 1, 1) = -1 / d;
        L1(i + 1, 2) = y / d;
        L1(i + 1, 3) = -x;
    }

    LInv = (L1.transpose() * L1).inverse() * L1.transpose();

    velocity = -lambda * LInv * (s - s_ref);

    cout << "ibvs out:" << velocity(0, 0) << " " << velocity(1, 0) << " " << velocity(2, 0) << " " << velocity(3, 0) << "\n";

    return velocity;
}
