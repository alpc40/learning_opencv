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
string win0="imageshow",win1="test",win2="canny",win3="HoughLineP",win4="dst";
Mat src,src1,src2,src3,src_gray,src_ero,src_dil,draw,gray,gray_A,dctL,dst;
int dst_c=450,dst_r=270;
int size_elm;
double areaT;

double calAngle(Vec4i a,Vec4i b)
{
    int ax=a[2]-a[0],ay=a[3]-a[1];
    int bx=b[2]-b[0],by=b[3]-b[1];
    double ath,bth;
    ath=(ax==0)?90:atan((double)ay/ax)*180/CV_PI;
    bth=(bx==0)?90:atan((double)by/bx)*180/CV_PI;
    return fabs(ath-bth);
}
/*
double 
{
    double dx1=a.x-b.x;
    double dy1=a.y-b.y;
    double dx2=c.x-b.x;
    double dy2=c.y-b.y;
    double angle=(dx1*dx2+dy1*dy2)/sqrt((dx1*dx1+dy1*dy1)*(dx2*dx2+dy2*dy2)+1e-10);
    return acos(angle)*180/3.141592653;
}
*/

double calD(Point2f a,Point2f b)
{
    double x=a.x-b.x;
    double y=a.y-b.y;
    return sqrt(x*x+y*y);
}

double calDPL(Vec4i l,int x0,int y0)
{
    double a=l[3]-l[1];
    double b=l[0]-l[2];
    double c=l[2]*l[1]-l[3]*l[0];
    double dis=fabs(a*x0+b*y0+c)/sqrt(a*a+b*b);
}

Point calCross(Vec4i a,Vec4i b)
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
double isNB(Point a,Point b,Vec4i c)
{
    double x0=b.x-a.x,y0=b.y-a.y;
    double x1=c[2]-c[1],y1=c[3]-c[2];
    double x2,y2;
    double len=sqrt(x0*x0+y0*y0),len2;
    if(x0*x1>0 || y0*y1>0)//向量同向，b与（c[0],c[1]）比较
    {
        x2=b.x-c[0];y2=b.y-c[1];
        if(x0*x2<0 || y0*y2<0) return 0;
        len2=sqrt(x2*x2+y2*y2);
        return len2>len?len:len2;
    }
    else if(x0*x1<0 || y0*y1<0)//向量反向，a与（c[2],c[3]）比较
    {
        x2=c[2]-a.x;y2=c[3]-a.y;
        if(x0*x2<0 || y0*y2<0) return 0;
        len2=sqrt(x2*x2+y2*y2);
        return len2>len?len:len2;
    }
    return 0;
}

int findLines(vector<Vec4i> lines,double tMax)
{
    int i,j,k,l,ct;
    int s=lines.size();
    double th,dis;
    bool m[100]={0};
    bool mark[100][100]={0};
    Point cross[100][100];
    if(s<4) return 0;
    printf("there is %d lines\n",s);
    //判断重合的线：夹角小于10度，且距离小于10
    for(i=0;i<s;i++)
    {
        if(m[i]==1) continue;
        for(j=i+1;j<s;j++)
        {
            if(m[j]==1) continue;
            th=calAngle(lines[i],lines[j]);
            if(th<10 || th>170)
            {
                dis=calDPL(lines[i],lines[j][0],lines[j][1]);
                if(dis<10)
                {
                    m[j]=1;
                }
            }
        }
    }
    //删除重合的线段
    vector<Vec4i>::iterator it=lines.begin();
    for(i=s;i>=0;i--)
    {
        if(m[i])
            lines.erase(it+i);
    }
    //重新获得vector的长度,计算任意两条线的交点
    s=lines.size();
    printf("after %d lines\n",s);
    if(s<4) return 0;
    if(s>100) return 2;
    printf("b\n");
    Mat src3=Mat(src.size(),CV_8UC1,Scalar(0));
    for(int i=0;i<lines.size();i++)
    {
        line(src3,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255),2,CV_AA);
    }
    imshow(win3,src3);
    for(i=0;i<s;i++)
    {
        for(j=i+1;j<s;j++)
        {
            th=calAngle(lines[i],lines[j]);
            if(fabs(th-90)<tMax )
            {
                mark[i][j]=1;
                mark[j][i]=1;
                cross[i][j]=cross[j][i]=calCross(lines[i],lines[j]);
            }
        }

    }
    printf("c\n");
    //
    memset(m,0,sizeof(m));
    Point p[4];
    for(i=0;i<s;i++)
    {
        m[i]=1;
        for(j=0;j<s;j++)
        {
            if(m[j]==0 && mark[i][j]==1)
            {
                m[j]=1;
                for(k=0;k<s;k++)
                {
                    if(m[k]==0 && mark[j][k]==1)
                    {
                        m[k]=1;
                        for(l=0;l<s;l++)
                        {
                            if(m[l]==0 &&mark[k][l]==1 && mark[l][i]==1)
                            {
                                int g,g1;
                                double len[4];
                                double maxA,A;
                                vector<Point> v;
                                //下面两句判断两个长必须大于两个宽
                                len[0]=calD(cross[i][j],cross[j][k]);
                                len[1]=calD(cross[j][k],cross[k][l]);
                                len[2]=calD(cross[k][l],cross[l][i]);
                                len[3]=calD(cross[l][i],cross[i][j]);
                                if((len[1]-len[0])*(len[3]-len[2])<0) continue;
                                if((len[3]-len[0])*(len[1]-len[2])<0) continue;
                                //printf("%lf %lf %lf %lf\n",len[0],len[1],len[2],len[3]);
                                //控制高长比在0.3~0.7
                                for(g=0;g<4;g++)
                                {
                                    maxA=len[g];A=-1;
                                    for(int g1=g+1;g1<4;g1++)
                                    {
                                        if(len[g1]>maxA)
                                        {
                                            maxA=len[g1];
                                            len[g1]=len[g];
                                            len[g]=maxA;
                                        }
                                    }
                                }
                                if(len[3]/len[0]<0.3 || len[2]/len[1]>0.7) continue;
                                //下面判断边缘线必须在连接线内部,且内部线长必须大于20
                                if(isNB(cross[i][j],cross[l][i],lines[i])<20) continue;
                                if(isNB(cross[j][k],cross[i][j],lines[j])<20) continue;
                                if(isNB(cross[k][l],cross[j][k],lines[k])<20) continue;
                                if(isNB(cross[l][i],cross[k][l],lines[l])<20) continue;
                                //下面判断面积是否符合要求
                                v.push_back(cross[i][j]);
                                v.push_back(cross[j][k]);
                                v.push_back(cross[k][l]);
                                v.push_back(cross[l][i]);
                                double a=contourArea(v); 
                                if(a>areaT*0.9 || a<areaT*0.3) continue;
                                //
                                p[0]=cross[i][j];
                                p[1]=cross[j][k];
                                p[2]=cross[k][l];
                                p[3]=cross[l][i];
                                dst=src.clone();
                                for(int g=0;g<4;g++)
                                {
                                    printf("(%d %d)\n",p[g].x,p[g].y);
                                    line(dst,p[g],p[(g+1)%4],Scalar(0,255,0),2,CV_AA);
                                }
                                imshow("ad",dst);
                                return 1;   
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
    return 0;
}


void imgproc()
{
    int i,j,c,l;
    vector<Vec4i> lines;
    int size_elm=3;//定义腐蚀膨胀窗口大小，与图像大小有关
    Mat srcC;
    Mat element=getStructuringElement(MORPH_RECT,Size(size_elm*2+1,size_elm*2+1),Point(size_elm,size_elm));
    Mat element1=getStructuringElement(MORPH_RECT,Size(3,3),Point(1,1));
    double area=src.cols*src.rows;
    //使图像变清晰
    Mat kernel=(Mat_<char>(3,3)<< 0,-1,0,-1,5,-1,0,-1,0);
    filter2D(src,src,src.depth(),kernel);
    areaT=src.cols*src.rows;
    //blur(src,src1,Size(7,7),Point(-1,-1));//模糊
    Canny(src,src2,1000,3000,5);
    //cvtColor(srcC,gray,CV_BGR2GRAY);
    /*
    dilate(gray,src_dil,element);//膨胀
    erode(src_dil,src_ero,element);//腐蚀
    imshow(win1,src_ero);
    */
    //Canny(gray,src2,1000,3000,5);
    imshow(win2,src2);
    dilate(src2,src_dil,element);//膨胀
    erode(src_dil,src_ero,element1);//腐蚀
    bitwise_not(src_ero,gray);
    imshow(win3,gray);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(src_ero,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
    Mat a=Mat::zeros(src.size(),CV_8UC1);
    for(int i=0;i<contours.size();i++)
    {
        double cA=contourArea(contours[i]);
//        if(cA>area*0.01 && cA<area*0.8)
//        if(cA>1000.0 && cA<area*0.8)
        {
            
            drawContours(a,contours,i,Scalar(255),1,8,hierarchy,0,Point());
        }
    }
    imshow(win4,a);
    HoughLinesP(a,lines,1,CV_PI/180,30,src.cols/10,20);
    findLines(lines,20);
    //膨胀、腐蚀是为了去掉字符等小块物体的影响
    /*
    int size_elm=src.cols/75;//定义腐蚀膨胀窗口大小，与图像大小有关
    Mat element=getStructuringElement(MORPH_RECT,Size(size_elm*2+1,size_elm*2+1),Point(size_elm,size_elm));
    dilate(gray,src_dil,element);//膨胀
    erode(src_dil,src_ero,element);//腐蚀
    imshow("adf",src_ero);
    int maxV=10000,minV=0,v;
    
    while(maxV>minV)
    {
        v=(maxV+minV)/2;
        Canny(src_ero,src2,v,v*3,5);
        imshow(win2,src2);
        
        HoughLinesP(src2,lines,1,CV_PI/180,20,100,10);
        int a=findLines(lines,10);
        if(a==1) break;
        else if(a==0)
            maxV=v;
        else 
            minV=v+1;    
        waitKey(0);
    }*/
    waitKey(0);
}


int main(int argc,char **argv)
{
    DIR *p;
    struct dirent *dirp;
    p=opendir(dir_path.c_str());
    while((dirp=readdir(p))!=NULL)
    {
        if(dirp->d_name[0]=='.') continue;
        img_path=dir_path+dirp->d_name;
        src=imread(img_path,1);
        namedWindow(win0,WINDOW_KEEPRATIO);
        namedWindow(win1,WINDOW_KEEPRATIO); 
        moveWindow(win1,200,0);
        namedWindow(win2,WINDOW_KEEPRATIO);
        moveWindow(win2,400,0);
        namedWindow(win3,WINDOW_KEEPRATIO);
        moveWindow(win3,600,0);
        namedWindow(win4,WINDOW_KEEPRATIO);
        moveWindow(win4,800,0);
        namedWindow("ad",WINDOW_KEEPRATIO);
        moveWindow("ad",0,300);
        imshow(win0,src);
        imgproc();
    }
    exit(0);
}
