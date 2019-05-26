#include "helpers.h"
#include "ros.h"
#include "detect.h"
#include "ibvs.h"

Mat buffer_frame;
uint64_t fc = 0;
uint64_t cfc = -1;
bool cp = false;
ros::Publisher motion;
void imageCallback(const sensor_msgs::ImageConstPtr &msg)
{
    cout << "frame received\n";
    auto i = cv_bridge::toCvCopy(msg);
    while (cp)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    buffer_frame = (*i).image;
    fc++;
}

void get_quatbebo(const nav_msgs::Odometry::ConstPtr &msg)
{

    depth = msg->pose.pose.position.z;
    if(depth == 0);
    depth = 1;
}

void publish_vel(MatrixXf vel)
{
 //   if(vel.norm() > 0.1)
 //       vel = vel/vel.norm()*0.1;
    geometry_msgs::Twist rot;
    geometry_msgs::Vector3 m;
    m.x = -vel(1,0);
    m.y = -vel(0,0);
    m.z = -vel(2,0);
    geometry_msgs::Vector3 s;
    if(abs(m.x) > 0.02)  m.x = abs(m.x)/m.x*0.02;
    if(abs(m.y) > 0.02)  m.y = abs(m.y)/m.y*0.02;
    if(abs(m.z) > 0.02)  m.z = abs(m.z)/m.z*0.02;
    s.x=s.z=s.y=0;
    cout << "Velocity: " << m.x << " " << m.y << " " << m.z << "\n";
    s.z = -vel(3,0);
    if(abs(s.z) > 0.02)  s.z = abs(s.z)/s.z*0.02;

    rot.linear = m;
    rot.angular = s;
    motion.publish(rot);
}

Mat get_new_frame()
{
    while (fc == cfc || fc == 0)
    {
        ros::spinOnce();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    cp = true;
    cfc = fc;
    Mat rv;
    buffer_frame.copyTo(rv);
    cp = false;
    return rv;
}

int main(int argc, char **argv)
{
    //VideoCapture Video(1);
    int counter = 0;
    reference.push_back(Point2d(320, 119));
    reference.push_back(Point2d(497, 117));
    reference.push_back(Point2d(500, 332));
    reference.push_back(Point2d(312, 332));
    reference.push_back(Point2d(365, 117));
    reference.push_back(Point2d(450, 117));
    reference.push_back(Point2d(450, 334));
    reference.push_back(Point2d(360, 332));
    reference.push_back(Point2d(436, 191));
    reference.push_back(Point2d(436, 259));
    reference.push_back(Point2d(377, 259));
    reference.push_back(Point2d(380, 191));
    cout << "start\n";
    ros::init(argc, argv, "precision_landing");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    ros::Subscriber sub2 = nh.subscribe("/bebop/odom", 1, get_quatbebo);
    image_transport::Subscriber sub = it.subscribe("/bebop/image_raw", 1, imageCallback);
    motion = nh.advertise<geometry_msgs::Twist>("/bebop/cmd_vel", 1);
    startDetect();

    ros::spin();
    return 0;
}
