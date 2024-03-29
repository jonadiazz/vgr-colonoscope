#include <string>

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>

using namespace std;
using namespace cv;

/* Global variables */
Mat src, erosion_dst, dilation_dst;

int erosion_elem = 0;
int erosion_size = 4;
int dilation_elem = 0;
int dilation_size = 12;
int const max_elem = 2;
int const max_kernel_size = 21;
RNG rng(12345);

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

/** Function Headers */
void Erosion( int, void* );
void Dilation( int, void* );

int main(int, char**)
{
  cv::VideoCapture cap;
  // open the default camera
  cap.open( string("/home/pi/Desktop/colonoscpy.m4v")); 
  
  // check if we succeeded
  if(!cap.isOpened()) 
    return -1;

  namedWindow("Source file",1);
  namedWindow("Darkess area",1);
  namedWindow("blurred",1);
  int erosion_type = MORPH_RECT;
  Mat e_element = 
    getStructuringElement( erosion_type,
			   Size( 2*erosion_size+1, 2*erosion_size+1 ),
			   Point( erosion_size, erosion_size ) );
  int dilation_type = MORPH_RECT;
  Mat d_element =
    getStructuringElement( dilation_type,
			   Size( 2*dilation_size +1, 2*dilation_size+1),
			   Point( dilation_size, dilation_size) );

  for(;;) {
    Mat frame, original;

    // get a new frame from camera
    cap >> frame >> original;
    frame = frame > 50;
    /* Apply erosion op */
    cvtColor( frame, frame, COLOR_BGR2GRAY );
    //GaussianBlur(frame, frame, Size(7,7), 1.5, 1.5);
    imshow("blurred", frame);
    erode( frame, frame, e_element );
    
    dilate( frame, frame, d_element );
    e_element = 
      getStructuringElement( erosion_type,
			     Size( 2*16+1, 2*14+1 ),
			     Point( erosion_size, erosion_size ) );
    erode( frame, frame, e_element );
    //cvtColor(frame, edges, CV_BGR2GRAY);

    //Canny(edges, edges, 0, 30, 3);
    
   
    blur( frame, frame, Size(3,3));

    frame = frame > 55;
    //findContours(frame,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE, Point(0,0));
    //frame = frame < 60;
    /* uncomment next line when ready with trackbarslide */
    /*for( size_t i = 0; i< contours.size(); i++ )
    {
       // rng random generator of color from passed interval 
       Scalar color = Scalar( rng.uniform(00,00), rng.uniform(00,00), rng.uniform(00,00) );
       drawContours( frame, contours, (int)i, color, 2, 8, hierarchy, 200, Point() );
    } */   

    imshow("Darkess area", frame);
    imshow("Source file", original);
    if(waitKey(30) >= 0)
      break;

    // the camera will be deinitialized automatically in VideoCapture destructor
  }
  return 0;
}
