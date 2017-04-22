#include "highgui.hpp"
#include "imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc,char **argv)
{
    Mat src,dst;
    src=imread(argc==1?"lena.jpg":argv[1],1);
/* 
    Mat hist[3];
    int histSize[]={256,256,256};
    float r_range={0,256},b_range={0,256},g_range={0,256};
    const float *range[]={r_range,b_range,g_range};
    calcHist(&src,1,3,Mat(),hist,3,histSize,range);
    double maxVal;
    minMaxLoc(hist,0,&maxVal,0,0);
*/

    //分割成3个通道
    vector<Mat> rgb_planes;
    split(src,rgb_planes);

    //
    int histSize=256;//颜色（0，255）分成几等分
    float range[]={0,256};//统计出来的值，应该放在什么范围，比如1000个红色，对应直方图【0，255】的值
    const float *range1={range};
    Mat r_hist,b_hist,g_hist;
    calcHist(&rgb_planes[0],1,0,Mat(),r_hist,1,&histSize,&range1,true,false);
    calcHist(&rgb_planes[1],1,0,Mat(),b_hist,1,&histSize,&range1,true,false);
    calcHist(&rgb_planes[2],1,0,Mat(),g_hist,1,&histSize,&range1,true,false);


    normalize(r_hist,r_hist,0,400,NORM_MINMAX,-1,Mat());
    normalize(g_hist,g_hist,0,400,NORM_MINMAX,-1,Mat());
    normalize(b_hist,b_hist,0,400,NORM_MINMAX,-1,Mat());

    int x=400,y=400;
    dst=Mat(Size(x,y),CV_8UC3,Scalar::all(0));
    float *pr=r_hist.ptr<float>(0),
          *pb=g_hist.ptr<float>(0),
          *pg=b_hist.ptr<float>(0);
    double ratio=(double)x/histSize;
    for(int i=1;i<256;i++)
    {
        line(dst,Point((i-1)*ratio,400-pr[i-1]),Point(i*ratio,400-pr[i]),Scalar(0,0,255));
        line(dst,Point((i-1)*ratio,400-pg[i-1]),Point(i*ratio,400-pg[i]),Scalar(0,255,0));
        line(dst,Point((i-1)*ratio,400-pb[i-1]),Point(i*ratio,400-pb[i]),Scalar(255,0,0)); 
    }

    imshow("calcHist",dst);
    waitKey();
    return 0;
}
