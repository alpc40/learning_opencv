#include "imgproc.hpp"
#include "highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int eg=1;
Mat img,gray,cedge,edge;

void trackbarHandler(int ,void *)
{
    blur(gray,edge,Size(3,3));
    Canny(edge,cedge,eg,eg*3,3);
    edge=Scalar::all(0);
    img.copyTo(edge,cedge);
    imshow("canny",edge);
}

int main(int argc,char **argv)
{
    img=imread(argv[1],1);
    if(img.empty())
    {
        cout<<"uage:a.out <image_path_name>"<<endl;
        return 0;
    }
    edge.create(img.size(),img.type());
    cvtColor(img,gray,COLOR_BGR2GRAY);
    namedWindow("canny",0);
    createTrackbar("track","canny",&eg,1000,trackbarHandler);
    trackbarHandler(0,0);
    waitKey();
    return 0;
}
