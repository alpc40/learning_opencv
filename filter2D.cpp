////////////////////////////////////////////
//目标：学会像素遍历，并处理像素////////////
//实现filter2D的功能，用图像增强掩码////////
//M={{0,-1,0},{-1,5,-1},{0,-1,0}}///////////
////////////////////////////////////////////

#include "cv.hpp"
#include "highgui.h"
using namespace cv;

Mat src,dst;
int rows,cols,chns;
uchar *pre,*next,*now,*n_dst;

void efficient_way()
{
    int i,j,k;
    for(i=1;i<rows-1;i++)
    {
        pre=src.ptr<uchar>(i-1);
        now=src.ptr<uchar>(i);
        next=src.ptr<uchar>(i+1);
        n_dst=dst.ptr<uchar>(i);
        for(j=chns;j<chns*(cols-1);j++)
        {
            n_dst[j]=saturate_cast<uchar>(5*now[j]-now[j-chns]-now[j+chns]-pre[j]-next[j]);
        }
    }
    dst.row(0).setTo(Scalar(0));
    dst.row(rows-1).setTo(Scalar(0));
    dst.col(0).setTo(Scalar(0));
    dst.col(cols-1).setTo(Scalar(0));
    imshow("filter2D",dst);
}

int main(int argc,char *argv[])
{
    src=imread(argc==1?"lena.jpg":argv[1],1);
    CV_Assert(src.depth()==CV_8U);
    rows=src.rows;
    cols=src.cols;
    //printf("%d %d\n",rows,cols);
    chns=src.channels();
    dst=Mat(src.size(),src.type());
    imshow("src",src);
    efficient_way();
    //下面必须用char,而不是uchar,因为包含负数
    Mat kern=(Mat_<char>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
    filter2D(src,dst,src.depth(),kern);
    imshow("filter2D1",dst);
    waitKey(0);
    return 0;
}
