#include "/usr/include/opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap("video.avi");
    Mat frame;
    cap.read(frame);

    imwrite("TEST1.jpg", frame);
}


