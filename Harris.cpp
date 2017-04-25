#include "highgui.hpp"
#include "imgproc.hpp"
#include<iostream>
#include<math.h>

using namespace std;
using namespace cv;

int thresh=254;
Mat src,dst,dst_norm,dst_norm_scaled;

void Handler(int,void *)
{
    //dst计算的是值，有符号的浮点
    cornerHarris(src,dst,3,9,0.04);
    //浮点归一化为0-255的值
    normalize(dst,dst_norm,0,255,NORM_MINMAX,CV_32FC1,Mat());
    //cout<<dst_norm<<endl;
    dst_norm_scaled=Mat::zeros(dst_norm.size(),CV_8UC1);
    convertScaleAbs(dst_norm,dst_norm_scaled);
    imshow("w",dst_norm_scaled);
    
    //cout<<dst_norm.depth();
    //cout<<dst_norm_scaled.depth();
    uchar *p;
    int i,j;
    for(i=0;i<src.rows;i++)
    {
        p=dst_norm.ptr(i);
        for(j=0;j<src.cols;j++)
        {
            if(int(p[j])>thresh)
            {
                circle(dst_norm_scaled,Point(j,i),5,Scalar(255),2,8,0);
            }
        }
    }
    imshow("Harris",dst_norm_scaled);
}

int main(int argc,char **argv)
{
    src=imread(argc==1?"lena.jpg":argv[1],0);
    dst=Mat(src.size(),CV_32FC1);
    namedWindow("Harris",1);
    namedWindow("w",1);
    createTrackbar("Threshold","Harris",&thresh,255,Handler);
    Handler(0,0);
    waitKey();
    return 0;
}
