#include <iostream>
#include "cv.hpp"
#include "highgui.h"

using namespace std;
using namespace cv;

Mat src,dst;
int type,e,es;

void on_trackbar(int ,void *)
{
    int opt=type+2;
    Mat element=getStructuringElement(e,Size(2*es+1,2*es+1),Point(es,es));
    morphologyEx(src,dst,opt,element);
    imshow("eroding",dst);

}

int main(int argc,char **argv)
{
    src=imread(argc==1?"lena.jpg":argv[1],1);
    namedWindow("eroding",CV_WINDOW_NORMAL);
    createTrackbar("type:\n0:opening\n1:closing\n2:Morphological\n3:tophat\n4:blackhat","eroding",&type,4,on_trackbar);
    createTrackbar("element:\n0:rect\n1:cross\n2:ellipse","eroding",&e,2,on_trackbar);
    createTrackbar("kernel size:\n2n+1","eroding",&es,20,on_trackbar);
    on_trackbar(0,0);
    waitKey(0);
    return 0;
}
