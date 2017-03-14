#include "cv.h"
#include "highgui.h"
#include<stdlib.h>

void mouseHandler(int event,int x,int y,int flag,void *param)
{
    switch(event){
        case CV_EVENT_LBUTTONDOWN:
            if(flag & CV_EVENT_FLAG_CTRLKEY)
                printf("left button down with CTRL press\n");
            break;
        case CV_EVENT_LBUTTONUP:
            printf("left button up\n");
            break;
    }
}

int main(int argc,char *argv[])
{
    IplImage *img=0;
    int x,y,step,chan;
    uchar *data;
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
    
    printf("Processing a %d*%d image with %d channels\n",x,y,chan);
    cvNamedWindow("win",CV_WINDOW_AUTOSIZE);
    cvMoveWindow("win",100,100);

    for(i=0;i<x;i++)
        for(j=0;j<y;j++)
            for(k=0;k<chan;k++)
                data[i*step+j*chan+k]=255-data[i*step+j*chan+k];

    cvShowImage("win",img);

    cvSetMouseCallback("win",mouseHandler,NULL);

    while(1)
    {
        key=cvWaitKey(0);
        if(key==int('q'))
            break;
    }
//cvWaitKey(0);
    cvReleaseImage(&img);
    return 0;
}
