#include "cv.hpp"
#include "highgui.h"

using namespace cv;
Mat src,src_gray,dst;
int type,value;

void on_trackbar(int ,void *)
{
    threshold(src_gray,dst,value,128,type);
    imshow("threshold",dst);
}

int main(int argc,char **argv)
{
    src=imread(argc==1?"lena.jpg":argv[1],1);
    cvtColor(src,src_gray,CV_BGR2GRAY);
    namedWindow("threshold",1);
    createTrackbar("type:\n0:binary\n1:unbinary\n2:cut\n3:zero\n4:unzero","threshold",
                    &type,4,on_trackbar);
    createTrackbar("value","threshold",&value,255,on_trackbar);
    on_trackbar(0,0);
    waitKey(0);
    return 0;
}
