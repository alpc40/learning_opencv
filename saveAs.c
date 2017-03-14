#include "cv.h"
#include "highgui.h"
#include<stdio.h>

int main(int argc,char *argv[])
{
    IplImage *pImg1,*pImg2;
    if(argc!=3)
    {
        printf("usage: a.out srcImage dstImage\n");
        exit(0);
    }
    pImg1=cvLoadImage(argv[1],0);
    if(!pImg1)
    {
        printf("cannot load image:%s\n",argv[1]);
        exit(0);
    }
    pImg2=cvCreateImage(cvGetSize(pImg1),pImg1->depth,pImg1->nChannels);
    cvCopy(pImg1,pImg2,NULL);
    cvSaveImage(argv[2],pImg1,NULL);
    cvShowImage("Image",pImg2);
    while(1)
    {
        if(cvWaitKey(0)=='q')
            break;
    }
    cvDestroyWindow("Image");
    cvReleaseImage(&pImg1);
    cvReleaseImage(&pImg2);
    return 0;
}
