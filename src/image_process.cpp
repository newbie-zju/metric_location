#include "metric_location/image_process.h"

#define HOUGH_RHO 1
#define HOUGH_THETA CV_PI/180

metric_process::MetricProcess::MetricProcess():it_(nh)
{
    cannyLowThreshold = 50;
    houghThreshold = 140;
    houghMinLineLength = 65;
    
    
    nh.param("image_input_topic",iImgTopic,string("/camera/image_raw"));
    image_sub= it_.subscribe(iImgTopic.c_str(),10,&MetricProcess::imageCallback,this);
    
    cv::namedWindow("iamge");
}


metric_process::MetricProcess::~MetricProcess()
{
    ROS_INFO("destroying the metric_process_node ...");
    cv::destroyWindow("image");
}


void metric_process::MetricProcess::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    dst.create(cv_ptr->image.size(),cv_ptr->image.type());
    
    cvtColor(cv_ptr->image, gray, cv::COLOR_BGR2GRAY); //convert image from RGB to gray
    //cvEqualizeHist(gray,hist);//直方图均值化，锐化
    medianBlur(gray,edge,5);//smooth the image using median filter
    Canny(edge,edge,cannyLowThreshold*2.5, cannyLowThreshold,3);
    //use the canny edge detection and produce the edge map
    //TODO:输入单通道灰度图；输出单通道黑白图；阈值用来控制边缘连接，大的阈值用来控制强边缘的初始分割即如果一个像素的梯度大与上限值，则被认为是边缘像素，如果小于下限阈值，则被抛弃。如果该点的梯度在两者之间则当这个点与高于上限值的像素点连接时我们才保留
    
    HoughLinesP(edge,metric_lines,HOUGH_RHO,HOUGH_THETA,houghThreshold,houghMinLineLength,houghMaxLineGap);
    //TODO:依次,输入图像,输出直线信息，像素分辨率，角度分辨率，直线长度阈值，直线最低长度，点与点之间最大距离
    
    metricLine(dst,metric_lines);
    
    imshow("image",dst);
    waitKey(3);
  
    
}

void metric_process::MetricProcess::metricCluster(vector< Vec4i > lines)
{
   
}

void metric_process::MetricProcess::metricLine(Mat image, vector< Vec4i > lines)
{
    for (size_t i=0; i<lines.size(); i++)
    {
	line(image,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(186,88,255),2,CV_AA);
    }
}




