#ifndef __IMAGE_PROCESS_H_
#define __IMAGE_PROCESS_H_

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <list>

#include <opencv2/opencv.hpp>

#include "metric_location/points.h"

using namespace std;
using namespace cv;

namespace metric_process{
class MetricProcess{
public:	
    int cannyLowThreshold;
    int houghThreshold;
    int houghMinLineLength;
    int houghMaxLineGap;
    bool showImg;
    string iImgTopic;
    Mat dst,edge,gray,hist;
    metric_location::points endpoint;
    vector<cv::Vec4i> metric_lines;
    
    ros::NodeHandle nh;
    image_transport::ImageTransport it_;
    
    image_transport::Subscriber image_sub;
    
    MetricProcess();
    ~MetricProcess();
    
    void imageCallback(const sensor_msgs::ImageConstPtr &msg);
    void metricCluster(vector<Vec4i> lines);
    void metricLine(Mat image, vector<Vec4i> lines);
};
};

#endif