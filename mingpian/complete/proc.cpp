#include "proc.hpp"

//test a Point is in the polygon or not
bool testIsIn(vector<Point> v,Point a)
{
    int ans=clockW(v[0],v[1],a);
    for(int i=1;i<4;i++)
    {
        if(ans!=clockW(v[i],v[(i+1)%4],a))
            return false;
    }
    return true;
}

//test one vector is clockwise or counterclockwise
int clockW(Point a,Point b,Point c)
{
    int x0=b.x-a.x,y0=b.y-a.y;
    int x1=c.x-b.x,y1=c.y-b.y;
    x0=x0*y1-x1*y0;
    if(x0>0) return 1;
    else if(x0<0) return 0;
    else return -1;
}
//get the image in src with the polygon ans
Mat getImg(Mat src,vector<Point> ans)
{
    Mat dst=Mat(Size(450,270),src.depth());
    Point a,b=Point((ans[0].x+ans[2].x)/2,(ans[0].y+ans[2].y)/2);
    vector<Point> v;
    int i,j,k=-1,n;
    for(i=0;i<4;i++)
    {
        j=(int)sqrt(ans[i].x*ans[i].x+ans[i].y*ans[i].y);
        if(k==-1 || k>j)
        {
            k=j;
            n=i;
        }
    }
    for(i=0;i<4;i++)
        v.push_back(ans[(n+i)%4]);
    //printf("%d %d,%d %d,%d %d,%d %d\n",v[0].x,v[0].y,v[1].x,v[1].y,v[2].x,v[2].y,v[3].x,v[3].y);
    int x1=v[1].x-v[0].x,y1=v[1].y-v[0].x;
    int x2=b.x-v[1].x,y2=b.y-v[1].y;
    if(x1*y2-x2*y1>0)//假如是四个点是顺时针，调整为逆时针
    {
        a=v[1];
        v[1]=v[3];
        v[3]=a;
    }
    //排序，以离（0，0）最近为起点，调整点位
    if(calD(v[0],v[1])>calD(v[3],v[0]))
    {
        a=v[0];
        v[0]=v[3];
        v[3]=v[2];
        v[2]=v[1];
        v[1]=a;
    }
    
    Point2f srcT[4];
    Point2f dstT[4];
    srcT[0]=v[0];
    srcT[1]=v[1];
    srcT[2]=v[2];
    srcT[3]=v[3];
    dstT[0]=Point2f(0,0);
    dstT[1]=Point2f(0,270);
    dstT[2]=Point2f(450,270);
    dstT[3]=Point2f(450,0);
    Mat warp_mat(3,3,CV_32FC1);
    warp_mat=getPerspectiveTransform(srcT,dstT);
    warpPerspective(src,dst,warp_mat,dst.size());
    return dst;
}


//rotate image for 90 or 90 degree

Mat rotateMat(Mat src,int dg)
{
    Mat dst=Mat(src.cols,src.rows,CV_8UC3);
    Point2f srcT[3];
    Point2f dstT[3];
    Mat warp_mat(3,3,CV_32FC1);
    srcT[0]=Point(0,0);
    srcT[1]=Point(src.cols,0);
    srcT[2]=Point(0,src.rows);
    if(dg==90)
    {
        dstT[0]=Point(0,src.cols);
        dstT[1]=Point(0,0);
        dstT[2]=Point(src.rows,src.cols);
    }
    else if(dg==-90)
    {
        dstT[0]=Point(src.rows,0);
        dstT[1]=Point(src.rows,src.cols);
        dstT[2]=Point(0,0);
    }
    else 
        return Mat();
    warp_mat=getAffineTransform(srcT,dstT);
    warpAffine(src,dst,warp_mat,dst.size());
    return dst;
}

//print one line
void printL(Vec4i a)
{
    printf("(%d,%d),(%d,%d)",a[0],a[1],a[2],a[3]);
}

//test two parallel lines(a,b) and lines(c), whether they are coinside 
bool isNB(Point a,Point b,Vec4i c)
{    
    double x0=c[0]-a.x,y0=c[1]-a.y,x1=c[2]-a.x,y1=c[3]-a.y;
    double x2=c[0]-b.x,y2=c[1]-b.y,x3=c[2]-b.y,y3=c[3]-a.y;

    if((x0*x1<0 || y0*y1<0) || (x2*x3<0 || y2*y3<0)||(x0*x2<0 ||y0*y2<0) || (x1*x3<0 || y1*y3<0))
        return 1;
    if(x0==0 && y0==0 && x1==0 && y1==0 && x2==0 && y2==0 && x3==0 && y3==0)
        return 1;
    return 0;
}
//calculate the cross point of two lines
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
//calculate the distance of one point to one line
double calDPL(Vec4i l,int x0,int y0)
{
    double a=l[3]-l[1];
    double b=l[0]-l[2];
    double c=l[2]*l[1]-l[3]*l[0];
    double dis=fabs(a*x0+b*y0+c)/sqrt(a*a+b*b);
    return dis;
}

//calculate the angle between two lines
double calAngle(Vec4i a,Vec4i b)
{
    int ax=a[2]-a[0],ay=a[3]-a[1];
    int bx=b[2]-b[0],by=b[3]-b[1];
    double ath,bth;
    ath=(ax==0)?90:atan((double)ay/ax)*180/CV_PI;
    bth=(bx==0)?90:atan((double)by/bx)*180/CV_PI;
    return fabs(ath-bth);
}

//calculate the distance of two point
double calD(Point2f a,Point2f b)
{
    double x=a.x-b.x;
    double y=a.y-b.y;
    return sqrt(x*x+y*y);
}
