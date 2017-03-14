#include "cv.hpp"
#include "highgui.h"
#include<iostream>
#include<stdlib.h>
#include<string>

using namespace std;
using namespace cv;

string name[]={"blur","Gaussian","median","bilateral"};
int btype=0;
int value=0;
Mat src,dst;

void on_trackbar(int ,void *)
{
    string str;
    int v=value*2+1;
    switch(btype){
        case 1:
            blur(src,dst,Size(v,v),Point(-1,-1));
            str=name[0];
            break;
        case 2:
            GaussianBlur(src,dst,Size(v,v),0,0);
            str=name[1];
            break;
        case 3:
            medianBlur(src,dst,v);
            str=name[2];
            break;
        case 4:
            bilateralFilter(src,dst,value,value*2,value/2);
            str=name[3];
            break;
    }
    //str=str;
    putText(dst,str,Point(0,dst.rows/4),5,1,Scalar::all(255));
    imshow("dst",dst);
}

int main(int argc,char **argv)
{
    src=imread(argc==1?"lena.jpg":argv[1],1);
    if(src.empty())
        return 0;
    src.copyTo(dst);
    namedWindow("original",1);
    imshow("original",src);
    namedWindow("dst",1);
    createTrackbar("1 blur\n2 Guassian\n3 median\n4bilateral","dst",&btype,5,on_trackbar);
    createTrackbar("value","dst",&value,30,on_trackbar);
    on_trackbar(0,0);
    waitKey(0);
    imwrite("test.jpg",dst);
    return 0;
}
