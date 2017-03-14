#include "cv.hpp"
#include "highgui.h"
#include<iostream>

using namespace std;
using namespace cv;

Mat src,dst;
int e,d,es,ds;

void on_trackbar(int ,void *)
{
    int type;
    if(e==0) type=MORPH_RECT;
    else if(e==1) type=MORPH_CROSS;
    else type=MORPH_ELLIPSE;
    Mat element=getStructuringElement(type,Size(2*es+1,2*es+1),Point(es,es));
    erode(src,dst,element);
    imshow("erode",dst);
    
}

void on_trackbar2(int ,void *)
{
    int type;
    if(d==0) type=MORPH_RECT;
    else if(d==1) type=MORPH_CROSS;
    else type=MORPH_ELLIPSE;
    Mat element=getStructuringElement(type,Size(2*ds+1,2*ds+1),Point(ds,ds));
    dilate(src,dst,element);
    imshow("dilate",dst);
}

int main(int argc,char **argv)
{
    src=imread(argc==2?argv[1]:"lena.jpg",0);
    namedWindow("erode",1);
    namedWindow("dilate",1);
    createTrackbar("Element:\n0:Rect\n1:Cross\n2:Ellipse","erode",
                    &e,2,on_trackbar);
    createTrackbar("size:\n2n+1","erode",&es,20,on_trackbar);
    createTrackbar("Element:\n0:Rect\n1:Cross\n2:Ellipse","dilate",
                    &d,2,on_trackbar2);
    createTrackbar("size:\n2n+1","dilate",&ds,20,on_trackbar2);
    on_trackbar(0,0);
    on_trackbar2(0,0);
    waitKey(0);
    return 0;
}

