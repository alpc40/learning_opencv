#include "highgui.hpp"
#include "imgproc.hpp"
#include <math.h>

using namespace cv;
using namespace std;

int main(int argc,char **argv)
{
    Mat src,dst,color_dst;
    src=imread(argc==2?argv[1]:"lena.jpg",0);
    imshow("src",src);
    Canny(src,dst,50,150);
    cvtColor(dst,color_dst,CV_GRAY2BGR);
    imshow("canny",dst);
/*
    Point pt1,pt2;
    vector<Vec2f> lines;
    HoughLines(dst,lines,1,CV_PI/180,100);
    for(size_t i=0;i<lines.size();i++)
    {
        float r=lines[i][0],th=lines[i][1];
        double a=cos(th),b=sin(th);
        double x0=r*cos(th),y0=r*sin(th);
        pt1.x=cvRound(x0+100*(-b));
        pt1.y=cvRound(y0+100*(a));
        pt2.x=cvRound(x0-100*(-b));
        pt2.y=cvRound(y0-100*(a));
        line(color_dst,pt1,pt2,Scalar(0,255,255));
    }
*/
    vector<Vec4f> lines;
    HoughLinesP(dst,lines,1,CV_PI/180,10,100,10);
    for(size_t i=0;i<lines.size();i++)
    {
        line(color_dst,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(0,0,255),3,8);
    }
    imshow("houghline",color_dst);
    waitKey();
    return 0;

}
