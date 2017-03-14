#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include"cv.h"
#include"highgui.h"

int main(int argc,char *argv[])
{
    IplImage *img=0;
    int height,width,step,channels;
    uchar *data;
    int i,j,k;
    if(argc<2)
    {
        printf("Usage:a.out <image-file-name>");
        exit(0);
    }
    img=cvLoadImage(argv[1]);
    if(!img)
    {
        printf("could not load image file:%s\n",argv[1]);
        exit(0);
    }
    height=img->height;
    width=img->width;
    step=img->widthStep;
    channels=img->nChannels;
    data=(uchar *)img->imageDate;
    printf("Processing a %d*%d image with %d channels\n",
            height,width,channels);
    cvNameWindow("mainWin",CV_WINDOW_AUTOSIZE);
    cvMoveWindow("mainWin",100,100);
    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
            for(k=0;k<channels;k++)
                data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
    cvShowImage("mainWin",img);
    cvWaitKey(0);
    cvReleaseImage(&img);
    return 0;
    
}
