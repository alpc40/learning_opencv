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
string win0="imageshow",win1="test",win2="test1",wind="dst";
Mat src,src1,src_sp[3],src_ero,src_dil,draw,gray,dst;
int dst_c=450,dst_r=270;

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

void imgproc()
{
    int i,j,c,l,area_k;
    double area1,area2;
    int N=6;
    int area=src.cols*src.rows;
    double maxArea=9.0*area/10,minArea=0.3*area;
    int count=0;
    Mat element=getStructuringElement(MORPH_RECT,Size(9,9),Point(4,4));
    Mat matrix=Mat(3,3,CV_32FC1);
    vector<vector<Point> > find;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    uchar *p,*p1;
    bilateralFilter(src,src1,10,25,25);//双边滤波，保持边缘的清晰度
    //imshow(win1,src1);
    split(src1,src_sp);
    bool flag=0; 
    dst=Mat(dst_r,dst_c,CV_8UC3,Scalar(0,0,0));
    for(c=0;c<3;c++)
    {
        if(flag==1) break;
        //imshow(win_name,src_sp[c]);
        dilate(src_sp[c],src_dil,element);//膨胀
        erode(src_dil,src_ero,element);//腐蚀
        imshow(win2,src_ero);
        for(l=0;l<N;l++)
        {
            if(flag==1) break;
            threshold(src_ero,gray,(l+1)*255/N,255,CV_THRESH_BINARY);
            imshow(win1,gray);
            waitKey(0);
            findContours(gray,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
            vector<vector<Point> > contours_poly(contours.size());
            draw=src.clone();
            area1=0,area_k=-1;
            for(j=0;j<contours.size();j++)
            {
                approxPolyDP(Mat(contours[j]),contours_poly[j],3,true);
                area2=contourArea(contours_poly[j]);
                if(contours_poly[j].size()==4 && area2>area1 && area2>minArea && area2<maxArea)
                {
                    double angleM=0,angleS=180,angle;
                    for(i=0;i<4;i++)
                    {

                        angle=calAngle(contours_poly[j][(i+3)%4],contours_poly[j][i],contours_poly[j][(i+1)%4]);
                        if(angle>angleM) angleM=angle;
                        if(angle<angleS) angleS=angle;
                    }
                    if(angleM<120 && angleS>60)
                    {
                         area_k=j;
                         area1=area2;
                    }
                }
            }
            
            if(area_k!=-1)
            {
                i=area_k;
                Point2f srcD[4],dstD[4];
                for(int a=0;a<4;a++)
                {
                    srcD[a].x=contours_poly[i][a].x;
                    srcD[a].y=contours_poly[i][a].y;
                    circle(draw,Point(srcD[a].x,srcD[a].y),3,Scalar(0,0,255),3,8);
                    //printf("%f %f\n",srcD[a].x,srcD[a].y);
                }
                if(calD(srcD[0],srcD[1])<calD(srcD[1],srcD[2]))
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
                warpPerspective(src,dst,matrix,dst.size());
                flag=1;
                /*
                printf("size=%d\n",contours_poly[i].size());
                for(j=0;j<contours_poly[i].size();j++)
                {
                        int x=contours_poly[i][j].x;
                        int y=contours_poly[i][j].y;
                        //printf("(%d %d)\n",x,y);
                        circle(draw,Point(x,y),3,Scalar(0,0,255),1,8);
                }
                */
                //drawContours(draw,contours_poly,area_k,Scalar(0,255,0),1,8,vector<Vec4i>(),0,Point());
                

            }
        }
    }
    imshow(wind,dst);
    waitKey(0);
}


int main(int argc,char **argv)
{
    DIR *dp;
    struct dirent *dirp;
    //namedWindow(win_name,1);
    if((dp=opendir(dir_path.c_str()))==NULL)
    {
        perror("open directory:");
        return 0;
    }
    while((dirp=readdir(dp))!=NULL)
    {
        //printf("opadf");
        if(dirp->d_name[0]=='.')
            continue;
        //img_path=dir_path+dirp->d_name;
        img_path=dir_path+"testa.jpg";
        //cout<<img_path<<endl;
        src=imread(img_path,1);
    //    printf("%d %d\n",src.cols,src.rows);
        imshow(win0,src);
        imgproc();        
    }
    exit(0);
}
