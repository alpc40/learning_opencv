#include "imgproc.hpp"
#include <vector>
using namespace cv;
using namespace std;

//test a Point is in the polygon or not
bool testIsIn(vector<Point> ,Point );

//test one vector is clockwise or counterclockwise
int clockW(Point ,Point ,Point );

//calculate the angle between two lines
double calAngle(Vec4i,Vec4i);
//caculate the distance between points
double calD(Point2f,Point2f);

//calculate the distance of one point to one line
double calDPL(Vec4i,int ,int );


//calculate the cross point of two lines
Point calCross(Vec4i ,Vec4i );


//test two parallel lines(Point(a),Point(b)) and lines(c), whether they are coinside 
bool isNB(Point ,Point ,Vec4i );
//print one line
void printL(Vec4i );

//rotate image for 90 or -90 degree
Mat rotateMat(Mat,int);


//get the image in src with the polygon ans
Mat getImg(Mat ,vector<Point> );
