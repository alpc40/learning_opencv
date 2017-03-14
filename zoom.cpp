#include "cv.hpp"
#include "highgui.h"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc,char **argv)
{
    Mat src,dst,tmp;
    int k,cols,rows;
    namedWindow("Zoom",1);
    src=imread(argv[1],CV_WINDOW_AUTOSIZE);
    imshow("Zoom",src);
    src.copyTo(tmp);
    k=waitKey(0);
    cols=src.cols;
    rows=src.rows;
    while(1)
    {
        if(k=='q')
            break;
        else if(k=='d')
        {
            cols=cols/2;
            rows=rows/2;
            pyrDown(src,dst,Size(cols,rows));
        }
        else if(k=='u')
        {
            cols=cols*2;
            rows=rows*2;
            pyrUp(src,dst,Size(cols,rows));
        }
        imshow("Zoom",dst);
        printf("cols=%d,rows=%d\n",cols,rows);
        k=waitKey(0);
    }
    return 0;
}
