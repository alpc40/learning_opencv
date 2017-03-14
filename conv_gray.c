#include "cv.h"
#include "highgui.h"
#include<stdlib.h>


int main(int argc,char *argv[])
{
    IplImage *img=0,*img1=0;
    int x,y,step,chan,step1;
    uchar *data,*data1;
    int i,j,k;
    int key;

    if(argc!=2)
    {
        printf("usage:./a.out <image_path_name>\n");
        exit(0);
    }
    img=cvLoadImage(argv[1]);
    if(!img)
    {
        printf("cannot load %s\n",argv[1]);
        exit(0);
    }
    
    x=img->height;
    y=img->width;
    step=img->widthStep;
    chan=img->nChannels;
    data=(uchar *)img->imageData;
    
    img1=cvCreateImage(cvSize(y,x),IPL_DEPTH_8U,1);
    data1=(uchar *)img1->imageData;
    step1=img1->widthStep;

    printf("Processing a %d*%d image with %d channels\n",x,y,chan);
    cvNamedWindow("win",CV_WINDOW_AUTOSIZE);
    cvMoveWindow("win",100,100);

    for(i=0;i<x;i++)
        for(j=0;j<y;j++)
        {
          //  data[i*step+j*chan+k]=255-data[i*step+j*chan+k];
          k=i*step+j*chan;
          data1[i*step1+j]=data[k]*0.114+data[k+1]*0.587+data[k+2]*0.299;
        }

    cvShowImage("win",img1);

    cvWaitKey(0);
//cvWaitKey(0);
    cvReleaseImage(&img1);
    cvReleaseImage(&img);
    return 0;
}
