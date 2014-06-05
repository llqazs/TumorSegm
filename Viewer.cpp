#include "Viewer.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Viewer
{

void show(const char* windowName, const MyBasic::Size2D& size)
{
   // cv::Mat image = cv::Mat(size.nRow,size.nCol,CV_8UC1,data);
   cv::Mat image = cv::Mat(size.nRow,size.nCol,CV_8UC1,cv::Scalar::all(0));

    cv::namedWindow( windowName, cv::WINDOW_AUTOSIZE );
    cv::imshow(windowName,image);
    cv::waitKey(0);
}

}
