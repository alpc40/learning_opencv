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
string win0="imageshow",win1="test",win2="test1",win3="dst";
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

double calD(Point2f a,Point2f b)
{
    float x=a.x-b.x;
    float y=a.y-b.y;
    return sqrt(x*x+y*y);
}

bool px(Vec4i a,Vec4i b)
{
    double x1=a[0],y1=a[1],x2=a[2],y2=a[3];
    double xd1=x2-x1,yd1=y2-y1;
    double x3=b[0],y3=b[1],x4=b[2],y4=b[3];
    double xd2=x4-x3,yd2=y4-y3;
    double th1=asin(yd1/sqrt(xd1*xd1+yd1*yd1))*180/CV_PI;
    double th2=asin(yd2/sqrt(xd2*xd2+yd2*yd2))*180/CV_PI;
    if(fabs(th1-th2)<10)
    {
        double d=fabs(yd2*x1+xd2*y1+x4*y3-x3*y4)/sqrt(xd2*xd2+yd2*yd2);
        if(d>80)
            return 1;
    }
    return 0;
}



//在最大轮廓中寻找符合要求的四变形
int find(vector<Vec4i> v,int s)
{
    if(s<4) return 0;//边数太少
    if(s>100) return 2;//如果检测出边的个数大于50，认为边数太多
    int i,j,k;
    int l[100][100];
    int c[100];
    memset(c,0,sizeof(c));
    for(i=0;i<s;i++)
    {
        for(j=i+1;j<s;j++)
        {
            if(jj(v[i],v[j])<10 && dis(v[i],v[j])>60)
                l[i][c[i]++]=j;
        }
    }
    for(i=0;i<s;i++)
    {
        if(c[i]==0) continue;
        for(j=i+1;j<s;j++)
        {
            if(c[j]==0) continue;
            double t=jj(v[i],v[j]);
            if(t>70 && t<110)
            {
                Point 
            }
        }
    }

}

void imgproc()
{
    int i,j,c,l;
    int N=6;//二值化处理的等级
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
    size_elm=src.cols/100;//定义腐蚀膨胀窗口大小，与图像大小有关
    Mat element=getStructuringElement(MORPH_RECT,Size(size_elm*2+1,size_elm*2+1),Point(size_elm,size_elm));
    dilate(src_gray,src_dil,element);//膨胀
    erode(src_dil,src_ero,element);//腐蚀
    imshow(win1,src_ero);
    Canny(src_ero,gray,50,150,5);
    imshow(win2,gray);
    waitKey(0);
    int count=0;
    int len=500;
    int step=250;
    bool flag=0;
    vector<Vec4i> lines;
    while(step>0 && len>20)
    {
        HoughLinesP(gray,lines,1,CV_PI/180,5,len,5);
        int a=find(lines,lines.size());
        if(a==0)
        {
            len=len-step;
        }
        else if(a>1)
        {
            step=step/2;
            len=len+step;
        }
        else
        {
            flag=1;
            break;
        }
    }
}


int main(int argc,char **argv)
{
    for(int i=1;i<argc;i++)
    {
        img_path=dir_path+argv[i];
        src=imread(img_path,1);

        namedWindow(win0,WINDOW_KEEPRATIO);
        namedWindow(win1,WINDOW_KEEPRATIO); 
        moveWindow(win1,200,0);
        namedWindow(win2,WINDOW_KEEPRATIO);
        moveWindow(win2,400,0);
        namedWindow(win3,WINDOW_KEEPRATIO);
        moveWindow(win3,600,0);
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
