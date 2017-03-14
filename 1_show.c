#include "cv.h"
#include "highgui.h"
#include<stdio.h>

void mouseHandler(int event,int x,int y,int flag,void *param)
{
    switch(event)
    {
        case CV_EVENT_LBUTTONDOWN:
            if(flag & CV_EVENT_FLAG_CTRLKEY)
                printf("left button down with ctrl pressed\n");
            break;
        case CV_EVENT_LBUTTONUP:
            printf("left button up x=%d y=%d\n",x,y);
            break;
    }
}

void trackbarHandler(int pos)
{
    printf("Trackbar position is %d\n",pos);    
}

int main(int argc,char *argv[])
{
    IplImage *img=0;
    int mouseParam=5;
    int trackbarVal=25;
    int maxVal=100;
    if(argc==2)
    {
        img=cvLoadImage(argv[1],1);
        if(!img)
        {
            printf("usage:a.out <image_path_name>\n");
            return 0;
        }   
        cvNamedWindow("Image",1);
        cvShowImage("Image",img);
        cvCreateTrackbar("bar1","Image",&trackbarVal,maxVal,trackbarHandler);
        //cvSetMouseCallback("Image",mouseHandler,&mouseParam);
        cvWaitKey(0);
        cvDestroyWindow("Image");
        cvReleaseImage(&img);
        return 0;
    }
    return -1;
}
