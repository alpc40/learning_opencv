#include "imgproc.hpp"
#include "highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc,char **argv)
{
    Mat src,dst;
    src=imread(argc==2?argv[1]:"lena.jpg",1);
    cvtColor(src,dst,CV_BGR2GRAY);
    GaussianBlur(dst,dst,Size(5,5),2,2);
    vector<Vec3f> circles;

    HoughCircles(dst,circles,HOUGH_GRADIENT,1,800,200,100,1,0);
    for(size_t i=0;i<circles.size();i++)
    {
        Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
        int radius=cvRound(circles[i][2]);
        circle(src,center,radius,Scalar(0,0,255),3,8,0);
    }
    imshow("HoughCircles",src);
    waitKey();
    return 0;
}
