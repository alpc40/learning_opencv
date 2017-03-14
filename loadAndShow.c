#include "cv.h"
#include "highgui.h"

int main(int argc,char *argv[])
{
    IplImage *pImg=0;
    pImg=cvLoadImage(argc==2?argv[1]:"/home/alpc40/picture/Lena.jpg",1);
    cvNamedWindow("Image",CV_WINDOW_AUTOSIZE);
    cvShowImage("Image",pImg);
    while(1)
    {
        if(cvWaitKey(0)=='q')
            break;
    }
    cvDestroyWindow("Image");
    cvReleaseImage(&pImg);
    return 0;
}
