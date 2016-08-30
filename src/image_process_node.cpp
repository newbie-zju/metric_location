#include "metric_location/image_process.h"

using namespace std;
int main(int argc, char** argv)
{
	ros::init(argc, argv, "metric_image_process_node");
	ros::NodeHandle nh;
	metric_process::MetricProcess metricprocess;
	ros::spin();
	return 0;
}