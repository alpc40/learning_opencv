#include "imgproc.hpp"
#include "highgui.hpp"
#include<iostream>
#include<dirent.h>
#include<errno.h>
#include<string>
#include<fcntl.h>
#define uchar unsigned char
using namespace std;
using namespace cv;

string dir_path= "/home/alpc40/opencv/mingpian/picture/";
string img_path;
string win0="imageshow",win1="test",win2="test1",win3="dst",win4="hi";
Mat src,src1,src_gray,src_ero,src_dil,draw,gray,gray_A,dctL,dst;
int dst_c=450,dst_r=270;
int size_elm;

double calAngle(Point a,Point b,Point c)
{
    double dx1=a.x-b.x;
    double dy1=a.y-b.y;
    double dx2=c.x-b.x;
    double dy2=c.y-b.y;
    double angle=(dx1*dx2+dy1*dy2)/sqrt((dx1*dx1+dy1*dy1)*(dx2*dx2+dy2*dy2)+1e-10);
    return acos(angle)*180/3.141592653;
}

Point calD(Vec4i a,Vec4i b)
{
    Point p;
    if(fabs(a[0]-a[2])<0.0001)//假如a线水平
    {
        p.x=a[0];
        p.y=fabs(b[1]-b[3])<0.0001?b[1]:
           double(b[1]-b[3])*(p.x-b[0])/(b[0]-b[2])+b[1];//假如b为垂直线
    }
    else if(fabs(b[0]-b[2])<0.0001)
    {
        p.x=b[0];
        p.y=fabs(a[1]-a[3])<0.0001?a[1]:double(a[1]-a[3])*(p.x-a[0])/(a[0]-a[2])+a[1];
    }
    else 
    {
        double k1=double(a[3]-a[1])/(a[2]-a[0]);
        double b1=double(a[1])-k1*a[0];
        double k2=double(b[3]-b[1])/(b[2]-b[0]);
        double b2=double(b[1])-k2*b[0];
        p.x=(b2-b1)/(k1-k2);
        p.y=k1*p.x+b1;
    }
    return p;
}



//在最大轮廓中寻找符合要求的四变形
bool findHoughLines()
{
    Mat cny;
    vector<Vec4i> lines;
    Canny(gray_A,cny,50,150,5); 
    HoughLinesP(cny,lines,1,CV_PI/180,15,50,5);
    printf("lines=%d\n",lines.size());
    if(lines.size()<4) return 0;
    int i,j,k,l;
    /*
    for(i=0;i<4;i++)
    {
        v[i]=calD(lines[i],lines[(i+1)%4]);
    }
    for(i=0;i<4;i++)
    {
        line(cny,v[i],v[(i+1)%4],Scalar(128),2,CV_AA);
    }*/
    for(i=0;i<4;i++)
    {
        line(cny,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(128),2,CV_AA);
    }
    imshow(win4,cny);
    return 1;
}

void imgproc()
{
    int i,j,c,l;
    int N=16;//二值化处理的等级
    //轮廓的最大最小面积
    int area=src.cols*src.rows;
    double maxArea=0.9*area,minArea=0.3*area;
    double area1,area2;
    int k_maxArea;

    Mat matrix=Mat(3,3,CV_32FC1);
    //轮廓保存
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    //双边滤波
    bilateralFilter(src,src1,10,25,25);//双边滤波，保持边缘的清晰度
    cvtColor(src1,src_gray,CV_BGR2GRAY);//转换为灰度图
    
    //膨胀、腐蚀是为了去掉字符等小块物体的影响
    size_elm=src.cols/75;//定义腐蚀膨胀窗口大小，与图像大小有关
    Mat element=getStructuringElement(MORPH_RECT,Size(size_elm*2+1,size_elm*2+1),Point(size_elm,size_elm));
    dilate(src_gray,src_dil,element);//膨胀
    erode(src_dil,src_ero,element);//腐蚀
    imshow(win1,src_ero);
    for(l=0;l<N;l++)//N是指把255分割成几份，用来做二值化处理
    {
        threshold(src_ero,gray,(l+1)*255/N,255,CV_THRESH_BINARY);
        imshow(win2,gray);
        findContours(gray,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
        area1=0,k_maxArea=-1;//记录最大轮廓面积，和最大轮廓索引
        for(j=0;j<contours.size();j++)
        {
            area2=contourArea(contours[j]);
            if(area2>minArea && area2<maxArea)
            {
                if(area2>area1)
                {
                    area1=area2;
                    k_maxArea=j;
                }
            }
        }
        //printf("b\n");
        //存在这样的最大轮廓
        if(k_maxArea!=-1)
        {
            int k=k_maxArea;
            uchar *p;
            double dis;
            vector<Point> contours_poly(contours[k].size());
            approxPolyDP(Mat(contours[k]),contours_poly,3,true);
            //保持轮廓内图像，轮廓外均为黑色
            gray_A=gray.clone();
            for(i=0;i<gray.rows;i++)
            {
                p=gray_A.ptr<uchar>(i);
                //printf("d\n");
                for(j=0;j<gray.cols;j++)
                {
                    dis=pointPolygonTest(contours_poly,Point2f(j,i),true);
                    if(dis<0)
                    {
                        p[j]=0;
                    }
                }
            }
            imshow(win3,gray_A);
            findHoughLines();
        }
        waitKey(0);
    }
    waitKey(0);
}


int main(int argc,char **argv)
{
    for(int i=1;i<argc;i++)
    {
        img_path=dir_path+argv[i];
        src=imread(img_path,1);

        namedWindow(win0,WINDOW_KEEPRATIO);
        namedWindow(win1,WINDOW_KEEPRATIO); 
        moveWindow(win1,300,0);
        namedWindow(win2,WINDOW_KEEPRATIO);
        moveWindow(win2,0,200);
        namedWindow(win3,WINDOW_KEEPRATIO);
        moveWindow(win3,300,200);
        namedWindow(win4,WINDOW_KEEPRATIO);
        moveWindow(win4,0,400);
        while(src.cols>500 || src.rows>500)
        {
            pyrDown(src,src1,Size(src.cols/2,src.rows/2));
            src=src1.clone();
        }
        imshow(win0,src);
        imgproc();
    }
    exit(0);
}
