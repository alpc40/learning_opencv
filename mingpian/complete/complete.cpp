#include "highgui.hpp"
#include"proc.hpp"
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
string win0="win0",win1="win1",win2="win2",win3="win3",win4="win4";
Mat src;

vector<Point> findLines(vector<Vec4i> lines,double tMax)
{
    int i,j,k,l,ct;
    int s=lines.size();//记录多少根线
    double th,dis;
    bool m[1000]={0};//记录某线是否符合要求
    bool mark[100][100]={0};//
    Point cross[100][100];//记录各线之间的交点
    vector<Point> v;//记录结果
    double areaT=src.cols*src.rows;
    if(s<4) return v;
    if(s>1000) return v;
    //printf("there is %d lines\n",s);
    //printf("mark=%d\n",mark[0][2]);
    //排除图像边缘轮廓线
    for(i=0;i<s;i++)
    {
        if((lines[i][0]<15 && lines[i][2]<15)||(src.cols-lines[i][0]<15 && src.cols-lines[i][2]<15))
            m[i]=1;
        if((lines[i][1]<15 && lines[i][3]<15)||(src.rows-lines[i][1]<15 && src.rows-lines[i][3]<15))
            m[i]=1;
        
    }
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
                if(dis<30)
                {
                    //printf("d");
                    m[j]=1;
                }
            }
        }
    }
    //printf("mark=%d\n",mark[0][2]);
    //删除重合的线段
    vector<Vec4i>::iterator it=lines.begin();
    for(i=s;i>=0;i--)
    {
        if(m[i])
            lines.erase(it+i);
    }
    //重新获得vector的长度,计算任意两条线的交点
    s=lines.size();
    //printf("after %d lines\n",s);
    if(s<4) return v;
    if(s>100) return v;
    //printf("b\n");
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
    //printf("c\n");
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
                                    maxA=len[g];
                                    for(g1=g+1;g1<4;g1++)
                                    {
                                        if(len[g1]>maxA)
                                        {
                                            maxA=len[g1];
                                            len[g1]=len[g];
                                            len[g]=maxA;
                                        }
                                    }
                                    //printf("%lf ",len[g]);
                                }
                               // printf("\n");
                                if(len[3]/len[0]<0.3 || len[2]/len[1]>0.8) continue;
                               /*      
                                printf("e\n");
                                printf("%d,%d,%d,%d",i,j,k,l);
                                printf("(%d,%d),(%d,%d)\n",cross[i][j].x,cross[i][j].y,cross[j][k].x,cross[j][k].y);
                                printf("(%d,%d),(%d,%d)\n",cross[k][l].x,cross[k][l].y,cross[l][i].x,cross[l][i].y);
                                printf("\n");
                               */ 
                                //下面判断边缘线必须在连接线内部,且内部线长必须大于20
                                /*
                                if(isNB(cross[i][j],cross[l][i],lines[i])==0) continue;
                                if(isNB(cross[j][k],cross[i][j],lines[j])==0) continue;
                                if(isNB(cross[k][l],cross[j][k],lines[k])==0) continue;
                                if(isNB(cross[l][i],cross[k][l],lines[l])==0) continue;
                                */
                                //printf("(%d,%d),(%d,%d)\n",cross[l][i].x,cross[l][i].y,cross[k][l].x,cross[k][l].y);
                                //printf("(%d,%d),(%d,%d)\n\n",lines[l][0],lines[l][1],lines[l][2],lines[l][3]);
                                //下面判断面积是否符合要求,判断图像中心（src.cols/2,src.row/2)是否处于长方形内部
                                v.push_back(cross[i][j]);
                                v.push_back(cross[j][k]);
                                v.push_back(cross[k][l]);
                                v.push_back(cross[l][i]);
                                double a=contourArea(v);
                                //printf("area=%lf\n",a/areaT);
                                if(a>areaT*0.9 || a<areaT*0.1) 
                                {
                                    v.clear();
                                    continue;
                                }
                                if(testIsIn(v,Point(src.cols/2,src.rows/2))==false) 
                                {
                                    v.clear();
                                    continue;
                                }
                                //printf("%d %d\n",src.cols/2,src.rows/2);
                                return v;   
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
    return v;
}


vector<Point> imgproc()
{
    int i,j,l,canny_c=800,step=15;
    vector<Vec4i> lines;
    vector<Point> ans;
    int size_elm=3;//定义腐蚀膨胀窗口大小，与图像大小有关
    Mat element=getStructuringElement(MORPH_RECT,Size(size_elm*2+1,size_elm*2+1),Point(size_elm,size_elm));
    Mat element1=getStructuringElement(MORPH_RECT,Size(3,3),Point(1,1));
    Mat src2,src_ero,src_dil,src_bw;
    vector<Point> contours_poly;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    while(canny_c>100)
    {
        ans.clear();
        Canny(src,src2,canny_c,canny_c*3,5);
        imshow(win1,src2);
        dilate(src2,src_dil,element);
        erode(src_dil,src_ero,element1);
        imshow(win2,src_ero);
        bitwise_not(src_ero,src_bw);

        contours.clear();
        hierarchy.clear();
        findContours(src_bw,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));

        Mat a=Mat::zeros(src.size(),CV_8UC1);
        for(i=0;i<contours.size();i++)
        {
            contours_poly.clear();
            approxPolyDP(Mat(contours[i]),contours_poly,5,true);
            l=contours_poly.size();
            for(j=0;j<l;j++)
            {
                line(a,contours_poly[j],contours_poly[(j+1)%l],Scalar(128),2,CV_AA);
            }
        }
        imshow(win4,a);
        step=15;
        while(step<20)
        {
            HoughLinesP(a,lines,1,CV_PI/180,50,src.cols/step,3);
            ans=findLines(lines,20);
            step++;
        }
        if(ans.size()!=0)
            break;
        canny_c=canny_c/2;
    }
    return ans;
}


int main(int argc,char **argv)
{
    DIR *p;
    struct dirent *dirp;
    vector<Point> ans;
    Mat kernel=(Mat_<char>(3,3)<< 0,-1,0,-1,5,-1,0,-1,0);

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
    namedWindow("ts",WINDOW_KEEPRATIO);
    moveWindow("ts",200,300);

    if((p=opendir(dir_path.c_str()))==NULL)
    {
        fprintf(stderr,"open directory %s:%s\n",dir_path.c_str(),strerror(errno));
        return 0;
    }
    while((dirp=readdir(p))!=NULL)
    {
        if(dirp->d_name[0]=='.') continue;
        img_path=dir_path+dirp->d_name;
        //img_path=dir_path+"22103613.jpg";
        src=imread(img_path,1);
        if(src.empty())
            continue;
        imshow(win0,src);
        if(src.cols<src.rows)
            src=rotateMat(src,90);
        //使图像变清晰
        filter2D(src,src,src.depth(),kernel);
        ans=imgproc();
        if(ans.empty())
            imshow("ad",src);
        else 
        {
            imshow("ad",getImg(src,ans));
            for(int i=0;i<4;i++)
            {
                line(src,ans[i],ans[(i+1)%4],Scalar(0,255,0),2,CV_AA);
            }
            imshow("ts",src);
        }
        waitKey(0);
    }
    exit(0);
}
