#include "cv.hpp"
#include "highgui.h"
#include "iostream"

using namespace std;
using namespace cv;

char wn[]="blurAll";
int lt=1000,st=100;

int main(int argc,char **argv)
{
    namedWindow(wn,1);
    Mat src,dst;
    src=imread(argv[1],1);
    imshow(wn,src);
    waitKey(lt);

    //Homogeous blur
    for(int i=1;i<31;i=i+2)
    {
        blur(src,dst,Size(i,i),Point(-1,-1));
        imshow(wn,dst);
        waitKey(st);
    }
    waitKey(0);
    //Gaussian blur
    for(int i=1;i<31;i=i+2)
    {
        GaussianBlur(src,dst,Size(i,i),0,0);
        imshow(wn,dst);
        waitKey(st);
    }
    waitKey(0);
    //median blur
    for(int i=1;i<31;i=i+2)
    {
        medianBlur(src,dst,i);
        imshow(wn,dst);
        waitKey(st);
    }
    waitKey(0);
    //bilateral blur
    for(int i=1;i<31;i=i+2)
    {
        bilateralFilter(src,dst,i,i*2,i/2);
        imshow(wn,dst);
        waitKey(st);
    }
    waitKey(0);
    
    return 0;
    
}
