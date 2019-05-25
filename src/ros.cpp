#include "helpers.h"
#include "ros.h"
#include "detect.h"
#include "ibvs.h"

Mat buffer_frame;
uint64_t fc = 0;
uint64_t cfc = -1;
bool cp = false;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    cout << "frame received\n";
    auto i = cv_bridge::toCvCopy(msg);
    while(cp)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    buffer_frame = (*i).image;
    fc++;
}

Mat get_new_frame()
{
    while(fc == cfc || fc==0)
    {
        ros::spinOnce();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    cp = true;
    cfc = fc;
    Mat rv;
    buffer_frame.copyTo(rv);
    cp=false;
    return rv;
}

int main(int argc, char **argv) {
	//VideoCapture Video(1);
	int counter = 0;
 
    cout << "start\n";
	ros::init(argc, argv, "precision_landing");
	ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);

    image_transport::Subscriber sub = it.subscribe("/bebop/image_raw", 1, imageCallback);

    //startDetect();

    ros::spin();
	return 0;
}
