#include "cv.h"
#include "highgui.h"
using namespace cv;

int main(int argc,char *argv[])
{
    Mat img=imread(argc==2?argv[1]:"/home/alpc40/picture/Lena.jpg");
    
    namedWindow("Image",CV_WINDOW_AUTOSIZE);
    imshow("Image",img);

    while(1)
    {
        if(cvWaitKey(0)=='q')
            break;
    }
    return 0;
}
