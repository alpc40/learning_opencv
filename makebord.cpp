#include "cv.hpp"
#include "highgui.h"
using namespace cv;

int main(int argc,char **argv)
{
    Mat src,dst;
    int type,u,d,l,r;
    Scalar value;
    RNG rng(12345);
    src=imread(argc==2?argv[1]:"lena.jpg",1);
    u=d=int(src.rows*0.05);
    l=r=int(src.cols*0.05);
    type=BORDER_CONSTANT;
    namedWindow("makeBorder",1);
    imshow("src",src);
    while(1)
    {
        int c=waitKey(500);
        if(c=='q') break;
        else if(c=='c')
            type=BORDER_CONSTANT;
        else if(c=='r')
            type=BORDER_REPLICATE;
        value=Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        copyMakeBorder(src,dst,u,d,l,r,type,value);
        imshow("makeBorder",dst);
    }
    return 0;
}
