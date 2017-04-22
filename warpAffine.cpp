#include "imgproc.hpp"
#include "highgui.hpp"

using namespace cv;

int main(int argc,char *argv[])
{
    Mat src,warp_dst,rot_dst;
    src=imread(argc==2?argv[1]:"lena.jpg",1);
    warp_dst=Mat(src.size(),src.type());
    rot_dst=Mat::zeros(src.size(),src.type());
    int cols=src.cols,rows=src.rows;
    //定义角点，用于获得变换函数
    Point2f srcm[3],dstm[3];
    srcm[0]=Point2f(0,0);
    srcm[1]=Point2f(cols-1,0);
    srcm[2]=Point2f(0,rows-1);
    dstm[0]=Point2f(0,rows/2);
    dstm[1]=Point2f(cols/2,0);
    dstm[2]=Point2f(cols/2,rows-1);
    //先计算变换矩阵，再进行变换
    Mat warp_mat=getAffineTransform(srcm,dstm);
    warpAffine(src,warp_dst,warp_mat,warp_dst.size());
    //定义变换圆心、角度和缩放因子
    Point2f center(cols/2,rows/2);
    float angle=-60;
    float scale=0.6;

    Mat rot_mat=getRotationMatrix2D(center,angle,scale);
    warpAffine(src,rot_dst,rot_mat,rot_dst.size());
    
    imshow("src",src);
    imshow("warp_src",warp_dst);
    imshow("rot_src",rot_dst);
    waitKey();
    return 0;


}
