//////////////////////////////////////
//study how to perspectiveTransform///
//////////////////////////////////////

#include "imgproc.hpp"
#include "highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc,char **argv)
{
    Mat src=imread(argc==2?argv[1]:"lena.jpg",1);
    Mat dst=Mat::zeros(src.size(),src.type());
    int cols=src.cols,rows=src.rows;
    int i,j,k;

    vector<Point2f> corner(4);
    corner[0]=Point2f(0,0);
    corner[1]=Point2f(rows-1,0);
    corner[2]=Point2f(0,cols-1);
    corner[3]=Point2f(rows-1,cols-1);
    vector<Point2f> corner1(4);
    corner1[0]=Point2f(rows/4,cols/4);
    corner1[1]=Point2f(rows-1,0);
    corner1[2]=Point2f(rows*3/4,cols*3/4);
    corner1[3]=Point2f(rows-1,cols/2);
    Mat trans=getPerspectiveTransform(corner,corner1);

    vector<Point2f> pts,pts1;
    for(i=0;i<rows;i++)
    {
        for(j=0;j<cols;j++)
        {
            pts.push_back(Point2f(j,i));
        }
    }

    perspectiveTransform(pts,pts1,trans);
    uchar *p,*p1;
    int x,y,count=0;
    for(i=0;i<rows;i++)
    {
        p=src.ptr<uchar>(i);
        for(j=0;j<cols;j++)
        {
            x=pts1[count].x;
            y=pts1[count].y;
            p1=dst.ptr<uchar>(y);
            p1[3*x]=p[3*j];
            p1[3*x+1]=p[3*j+1];
            p1[3*x+2]=p[3*j+2];
            count++;
        }
    }
    imshow("perspective",dst);
    waitKey();
    return 0;
}
