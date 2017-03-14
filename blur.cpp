#include "cv.hpp"
#include "highgui.h"
#include "cxcore.hpp"
#include<stdio.h>
using namespace cv;

int main(int argc,char **argv)
{
    Mat src,dst;
    src=imread(argv[1]);
    if(src.empty())
    {
        printf("cannot read image:%s\n",argv[1]);
        return 0;
    }
    blur(src,dst,Size(3,3));
    imshow("3*3",dst);
    blur(src,dst,Size(8,8));
    imshow("8*8",dst);
    waitKey(0);
    return 0;
}
