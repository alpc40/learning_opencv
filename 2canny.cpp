#include"cv.hpp"
#include"cxcore.hpp"
#include"highgui.h"

using namespace std;
using namespace cv;

int edgeThresh=1;

Mat image,cedge;
Mat gray,edge;

void onTrackbar(int,void *)
{
    blur(gray,edge,Size(3,3));
    Canny(gray,edge,edgeThresh,edgeThresh*3,3);
    cedge=Scalar::all(0);
    image.copyTo(cedge,edge);
    imshow("Edge map",edge);
}

int main(int argc, char **argv)
{
    image=imread(argv[1],1);
    if(image.empty())
    {
        printf("cannot read the image:%s\n",argv[1]);
        exit(-1);
    }
    cedge.create(image.size(),image.type());
    cvtColor(image,gray,CV_BGR2GRAY);
    namedWindow("Edge map",1);
    createTrackbar("canny Threshold","Edge map",&edgeThresh,100,onTrackbar);
    onTrackbar(0,0);
    waitKey(0);
    return 0;
}
