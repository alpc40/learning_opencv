#include "highgui.hpp"
#include "imgproc.hpp"
#include <iostream>
#include <string>
using namespace cv;
using namespace std;

Mat src,src_f,src_gray,src_canny,src_p,dst;
string path="/home/alpc40/picture/mingpian/test";
string w_src="show_src";
string w_dst="show_dst";
int no,thresh;
vector<vector<Point> > cont;
vector<Vec4i> hierarchy;

void openfile(int ,void *);
void contours(int ,void *);

int main()
{
    namedWindow(w_src,1);
    namedWindow(w_dst,1);
    createTrackbar("open testX.jpg(X:0~10)",w_src,&no,10,openfile);
    createTrackbar("canny:thresh(0,3000)",w_dst,&thresh,3000,contours);
    openfile(0,0);
//    contours(0,0);
    waitKey(0);
    return 0;
}

void contours(int,void *)
{
    //检测边缘
    Canny(src_f,src_canny,thresh,thresh*3,5);
    imshow("canny",src_canny);

    //直线检测
    vector<Vec4i> lines;
    HoughLinesP(src_canny,lines,1,CV_PI/180,50,50,10);
    src_p=Mat::zeros(src_canny.size(),src_canny.type());
    for(size_t i=0;i<lines.size();i++)
    {
        line(src_p,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255),1,CV_AA);
    }
    imshow("HoughlinesP",src_p);
/*
    //寻找轮廓
    findContours(src_canny,cont,hierarchy,CV_RETR_TREE,
                CV_CHAIN_APPROX_SIMPLE,Point(0,0));
    dst=Mat::zeros(src.size(),src.type());
    double area=0;
    int loc=-1;
    for(int i=0;i<cont.size();i++)
    { 
       // drawContours(dst,cont,i,Scalar(0,0,255),1,8,hierarchy,0,Point());
       //Rect rr=boundingRect(cont[i]);
       
    }
    imshow(w_dst,dst);
*/
}

void openfile(int ,void *)
{
    char ss[5];
    sprintf(ss,"%d",no);
    string pathX(ss);
    pathX=path+pathX+".jpg";
    src=imread(pathX,1);
    imshow(w_src,src);
    //滤波降噪，为了保留边缘，这里采用双边滤波
    bilateralFilter(src,src_f,9,10,2);
   // imshow("after bilateralFilter",src_f);
    contours(0,0);
}

