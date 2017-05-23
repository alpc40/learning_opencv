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
string win0="imageshow",win1="erode",win2="threshold",win3="polygonTest",win4="dst",win5="HoughLine";
Mat src,src1,src2,src_gray,src_ero,src_dil,draw,gray,gray_A,dctL,dst;
int dst_c=450,dst_r=270;
int size_elm;
int cn;//记录金字塔缩小次数

double calAngle(Vec4i a,Vec4i b)
{
    int ax=a[2]-a[0],ay=a[3]-a[1];
    int bx=b[2]-b[0],by=b[3]-b[1];
    double ath,bth;
    ath=(ax==0)?90:atan((double)ay/ax)*180/CV_PI;
    bth=(bx==0)?90:atan((double)by/bx)*180/CV_PI;
    return fabs(ath-bth);
}

Point calD(Vec4i a,Vec4i b)
{
    Point p;
    if(a[0]-a[2]==0)//假如a线水平
    {
        p.x=a[0];
        p.y=b[1]-b[3]==0?b[1]:
           double(b[1]-b[3])*(p.x-b[0])/(b[0]-b[2])+b[1];//假如b为垂直线
    }
    else if(b[0]-b[2]==0)
    {
        p.x=b[0];
        p.y=a[1]-a[3]==0?a[1]:double(a[1]-a[3])*(p.x-a[0])/(a[0]-a[2])+a[1];
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

double calDP(Point a,Point b)
{
    double dx=a.x-b.x,dy=a.y-b.y;
    return sqrt(dx*dx+dy*dy);
}

//在最大轮廓中寻找符合要求的四变形
//ss[j][k].x-cross[i][j].x)*(cross[k][l].y-cross[j][k].y)
bool findHoughLines(double minArea,double maxArea)
{
    Mat cny;
    vector<Vec4i> lines;
    Canny(gray_A,cny,50,150,5); 
    HoughLinesP(cny,lines,1,CV_PI/180,20,40,6);
    //printf("lines=%d\n",lines.size());
    if(lines.size()<4 || lines.size()>50) return 0;
    
    int i,j,k,l,s=lines.size();
    Point cross[50][50];//储存线段a/b的交点
    bool mark[50][50]={0};//判断线段a/b是否有交点
    bool m[50]={0},flag;
    
    for(i=0;i<s;i++)
    {
        for(j=i+1;j<s;j++)
        {
            double th=calAngle(lines[i],lines[j]);
            if(th>60 && th<140)
            {
                mark[i][j]=1;
                mark[j][i]=1;
                cross[i][j]=calD(lines[i],lines[j]);
                cross[j][i]=cross[i][j];
            }
        }
    }
    /*
    for(i=0;i<s;i++)
    {
        for(j=0;j<s;j++)
        {
            //printf("line1:(%d,%d),(%d,%d)\n",lines[i][0],lines[i][1],lines[i][2],lines[i][3]);
            //printf("line1:(%d,%d),(%d,%d)\n",lines[j][0],lines[j][1],lines[j][2],lines[j][3]);
            //printf(" (%d,%d)",cross[i][j].x,cross[i][j].y);
            if(mark[i][j]==1)
                circle(cny,cross[i][j],3,Scalar(128),3,8,0);
        }
        printf("\n");
    }
    */
    
    for(i=0;i<s;i++)
    {
        m[i]=1;
        for(j=0;j<s;j++)
        {
            if(m[j]==0&&mark[i][j]==1)
            {
                m[j]=1;
                for(k=0;k<s;k++)
                {
                    if(m[k]==0 && mark[j][k]==1)
                    {
                        m[k]=1;
                        for(l=0;l<s;l++)
                        {
                            if(m[l]==0&&mark[k][l]==1&&mark[l][i]==1)
                            {
                                vector<Point> v;
                                v.push_back(cross[i][j]);
                                v.push_back(cross[j][k]);
                                v.push_back(cross[k][l]);
                                v.push_back(cross[l][i]);
                                double a=contourArea(v);
                                if(a>minArea && a<maxArea)
                                {
                                    /*
                                    printf("%d %d %d %d\n",lines[k][0],lines[k][1],lines[k][2],lines[k][3]);
                                    printf("%d %d %d %d\n",lines[l][0],lines[l][1],lines[l][2],lines[l][3]);
                                    printf("%d %d\n",cross[k][l].x,cross[k][l].y); 
                                    printf("%d %d\n",cross[l][i].x,cross[l][i].y);
                                    */
                                    
                                    line(cny,cross[i][j],cross[j][k],Scalar(128),2,CV_AA);
                                    line(cny,cross[j][k],cross[k][l],Scalar(128),2,CV_AA);
                                    line(cny,cross[k][l],cross[l][i],Scalar(128),2,CV_AA);
                                    line(cny,cross[l][i],cross[i][j],Scalar(128),2,CV_AA);
                                    imshow(win5,cny);
                                    
                                    Point2f srcD[4],dstD[4];
                                    Mat matrix=Mat(3,3,CV_32FC1);
                                    if((cross[j][k].x-cross[i][j].x)*(cross[k][l].y-cross[j][k].y)-(cross[j][k].y-cross[i][j].y)*(cross[k][l].x-cross[j][k].x)<0)//逆时针
                                    {
                                        srcD[0]=cross[i][j]*cn;
                                        srcD[1]=cross[j][k]*cn;
                                        srcD[2]=cross[k][l]*cn;
                                        srcD[3]=cross[l][i]*cn;
                                    }
                                    else 
                                    {
                                        srcD[0]=cross[i][j]*cn;
                                        srcD[3]=cross[j][k]*cn;
                                        srcD[2]=cross[k][l]*cn;
                                        srcD[1]=cross[l][i]*cn;
                                    }
                                    if(calDP(srcD[0],srcD[1])<calDP(srcD[1],srcD[2]))
                                    {
                                        dstD[0].y=0;dstD[0].x=0;
                                        dstD[3].y=0;dstD[3].x=dst_c;
                                        dstD[2].y=dst_r;dstD[2].x=dst_c;
                                        dstD[1].y=dst_r;dstD[1].x=0;
                                    }
                                    else
                                    {
                                        dstD[1].y=0;dstD[1].x=0;
                                        dstD[0].y=0;dstD[0].x=dst_c;
                                        dstD[3].y=dst_r;dstD[3].x=dst_c;
                                        dstD[2].y=dst_r;dstD[2].x=0;
                                    }
                                    matrix=getPerspectiveTransform(srcD,dstD);
                                    warpPerspective(src1,dst,matrix,dst.size());
                                    return 1;
                                }
                            }
                        }
                        m[k]=0;
                    }
                }
                m[j]=0;
            }
        }
        m[i]=0;
    }
/*
    for(i=0;i<s;i++)
    {
        line(cny,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(128),2,CV_AA);
    }
    imshow(win4,cny);   
    */
    return 0;
}

void imgproc()
{
    int i,j,c,l;
    int N=10;//二值化处理的等级
    //轮廓的最大最小面积
    int area=src.cols*src.rows;
    double maxArea=0.9*area,minArea=0.3*area;
    double area1,area2;
    int k_maxArea;
    bool flag=0;
    Mat src_sp[3];

    //轮廓保存
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    
    //双边滤波
    bilateralFilter(src,src2,10,25,25);//双边滤波，保持边缘的清晰度

    for(c=0;c<3 && flag==0;c++)
    {
    split(src2,src_sp);
    src_gray=src_sp[c].clone();
    equalizeHist(src_gray,src_gray);//直方图均化，使对比度更加明显

    //膨胀、腐蚀是为了去掉字符等小块物体的影响
    size_elm=src.cols/75;//定义腐蚀膨胀窗口大小，与图像大小有关
    Mat element=getStructuringElement(MORPH_RECT,Size(size_elm*2+1,size_elm*2+1),Point(size_elm,size_elm));
    dilate(src_gray,src_dil,element);//膨胀
    erode(src_dil,src_ero,element);//腐蚀
    imshow(win1,src_ero);
    for(l=0;l<N && flag==0;l++)//N是指把255分割成几份，用来做二值化处理
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
            int k=k_maxArea,c_in=0;
            uchar *p;
            double dis;
            vector<Point> contours_poly(contours[k].size());
            approxPolyDP(Mat(contours[k]),contours_poly,3,true);
            //保持轮廓内图形，如果内部为黑色，则外部为白色，反之为黑色
            gray_A=gray.clone();
            int x=contours_poly[0].x;
            int y=contours_poly[0].y;
            c_in=gray.ptr<uchar>(y)[x];
        //    printf("%d",c_in);
            for(i=0;i<gray.rows;i++)
            {
                p=gray_A.ptr<uchar>(i);
                for(j=0;j<gray.cols;j++)
                {
                    dis=pointPolygonTest(contours_poly,Point2f(j,i),true);
                    if(dis<0)
                    {
                        p[j]=255-c_in;
                    }
                }
            }
            imshow(win3,gray_A);
            flag=findHoughLines(minArea,maxArea);
        }
    }
    }
    if(flag==0)
    {
        imshow(win4,src);
    }
    else
    {
        imshow(win4,dst);
    }
}


int main(int argc,char **argv)
{
    DIR *dp;
    struct dirent *dirp;
    dp=opendir(dir_path.c_str());
    dst=Mat(dst_r,dst_c,CV_8UC3);
    while((dirp=readdir(dp))!=NULL)
    {
        if(dirp->d_name[0]=='.') continue;
        img_path=dir_path+dirp->d_name;
        //img_path=dir_path+"0522103554.jpg";
        src=imread(img_path,1);
        namedWindow(win0,WINDOW_KEEPRATIO);
        namedWindow(win1,WINDOW_KEEPRATIO); 
        moveWindow(win1,300,0);
        namedWindow(win2,WINDOW_KEEPRATIO);
        moveWindow(win2,0,200);
        namedWindow(win3,WINDOW_KEEPRATIO);
        moveWindow(win3,300,200);
        namedWindow(win4,WINDOW_KEEPRATIO);
        moveWindow(win4,300,400);
        namedWindow(win5,WINDOW_KEEPRATIO);
        moveWindow(win5,0,400);
        src1=src.clone();
        cn=1;
        while(src.cols>500 || src.rows>500)
        {
            cn=cn*2;
            pyrDown(src,src,Size(src.cols/2,src.rows/2));
        }
        imshow(win0,src);
        imgproc();
        waitKey(0);
    }
    exit(0);
}
