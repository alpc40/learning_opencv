#include "cv.h"
#include "highgui.h"
#include<stdio.h>

int main(int argc,char **argv)
{
    IplImage *pImg1,*pImg2;
    if(argc==1) 
    {
        printf("usage:a.out <image_path_name>\n");
        return 0;
    }
    pImg1=cvLoadImage(argv[1],0);
    pImg2=cvCreateImage(cvGetSize(pImg1),IPL_DEPTH_8U,1);
    cvCanny(pImg1,pImg2,50,150,3);
    cvNamedWindow("src",1);
    cvNamedWindow("canny",1);
    cvMoveWindow("canny",200,200);
    cvShowImage("src",pImg1);
    cvShowImage("canny",pImg2);
    cvWaitKey();
    cvDestroyWindow("canny");
    cvDestroyWindow("src");
    cvReleaseImage(&pImg1);
    cvReleaseImage(&pImg2);
    return 0;
}
