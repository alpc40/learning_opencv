#include "cv.h"
#include "highgui.h"
#include<stdio.h>

int main(int argc,char *argv[])
{
    IplImage *img;
    int i,j,k;
    int x,y,step,chan;
    uchar *data;
    img=cvCreateImage(cvSize(600,400),IPL_DEPTH_8U,3);

    //img=cvLoadImage(argv[1]);
    if(img==0)
    {
        printf("create error!\n");
        exit(0);
    }
    x=img->height;
    y=img->width;
    data=(uchar *)img->imageData;
    step=img->widthStep;
    chan=img->nChannels;
    printf("x=%d,y=%d,step=%d,chan=%d\n",x,y,step,chan);
    //printf("%d\n",sizeof(data)/sizeof(short));
    for(i=0;i<400;i++)
        for(j=0;j<600;j++)
            for(k=0;k<3;k++)
            {
                data[i*step+j*chan+k]=255;
            }
    cvRectangle(img,cvPoint(100,100),cvPoint(200,200),cvScalar(0,0,255),3);
    cvCircle(img,cvPoint(200,300),20,cvScalar(0,255,0),3);
    cvLine(img,cvPoint(150,100),cvPoint(150,200),cvScalar(255,0,0),3);

    CvPoint curve1[]={10,10,10,200,200,200,200,10};
    CvPoint curve2[]={130,130, 130,330, 330,330 ,330,130,230,40};
    CvPoint * curveArr[2]={curve1,curve2};
    int nCurvePts[2]={4,5};
    int nCurves=2;
    int isCurveClosed=1;
    int lineWidth=1;
    cvPolyLine(img,curveArr,nCurvePts,nCurves,isCurveClosed,cvScalar(255,255,0),lineWidth,10,2);
    cvFillPoly(img,curveArr,nCurvePts,nCurves,cvScalar(155,155,0));

    cvNamedWindow("image",CV_WINDOW_AUTOSIZE);
    cvShowImage("image",img);
    cvWaitKey(0);
    cvReleaseImage(&img);

    return 0;
}
