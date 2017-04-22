#include "highgui.hpp"
#include "imgproc.hpp"
using namespace cv;

int main(int argc,char **argv)
{
    Mat src,dst;
    src=imread(argc==1?"lena.jpg":argv[1],0);
    //cvtColor(src,src,CV_BGR2GRAY);
    equalizeHist(src,dst);
    imshow("src",src);
    imshow("dst",dst);
    waitKey();
    return 0;
}
