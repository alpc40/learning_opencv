#include "imgproc.hpp"
#include "highgui.hpp"

using namespace cv;
using namespace std;

Mat src,dst;
Mat x,y;
void init_map(int ct)
{
    int i,j;
    float *px,*py;
    for(i=0;i<src.rows;i++)
    {
        px=x.ptr<float>(i);
        py=y.ptr<float>(i);
        for(j=0;j<src.cols;j++)
        {
            if(ct==0)//缩小2倍
            {
                if(j>src.cols/4 && j<3*src.cols/4 && i>src.rows/4 &&i<3*src.rows/4)
                {
                    px[j]=2*(j-src.cols/4)+0.5;
                    py[j]=2*(i-src.rows/4)+0.5;

                }
                else
                {
                    px[j]=0;
                    py[j]=0;
                }
            }
            else if(ct==1)//左右翻转
            {
                px[j]=src.cols-j;
                py[j]=i;
            }
            else if(ct==2)//上下翻转
            {
                px[j]=j;
                py[j]=src.rows-i;
            }
            else//对角翻转
            {
                px[j]=src.cols-j;
                py[j]=src.rows-i;
            }
        }
    }
}

int main(int argc,char **argv)
{
    src=imread(argc==1?"lena.jpg":argv[1],1);
    dst=Mat(src.size(),src.type());
    x=Mat(src.size(),CV_32FC1);
    y=Mat(src.size(),CV_32FC1);
    namedWindow("remap",1);
    imshow("remap",src);
    int ct=0;
    while(1)
    {
        int c=waitKey(2000);
        if(c=='q')
            break;
        init_map(ct%4);
        remap(src,dst,x,y,CV_INTER_LINEAR);
        imshow("remap",dst);
        ct++;
    }
    return 0;
}

