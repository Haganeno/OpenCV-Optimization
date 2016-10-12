/*
 *---------------------------------------------------------------------------------------------------
* Pour compiler : g++ `pkg-config --cflags opencv` projet_base.cpp `pkg-config --libs opencv` -o projet
 *---------------------------------------------------------------------------------------------------
 */
 

#include "projet_sobel_basic.h"
 
using namespace std;
using namespace cv;
using std::cout;


void sobel(Mat img_in, Mat& img_out, int row, int col) {

	
	
	
	
	int n;
	int m;
	n = row;
	m = col;
	
	img_in.copyTo(img_out);
	
	uint8 N, NE, NW, S, SE, SW, W, E, W2, NE2, SE2, E2;
	double gx, gy;
	
	int i, j;
	
	
	for(i = 1; i < n-1; i++) {
	    for(j = 1; j < m-1; j++) {
	    
		NW = img_in.at<uint8>(i-1, j-1);
		N = img_in.at<uint8>(i-1, j);
		NE = img_in.at<uint8>(i-1,j+1);
		
		W = img_in.at<uint8>(i,j-1);
		E = img_in.at<uint8>(i,j+1);
		
		
		SW = img_in.at<uint8>(i+1,j-1);
		S = img_in.at<uint8>(i+1,j);
		SE = img_in.at<uint8>(i+1,j+1);
		 
		
		gx = abs(NE + (E<<1) + SE - NW - (W<<1) - SW);
		gy = abs(NW + (N<<1) + NE - SW - (S<<1) - SE);
				
		img_out.at<uint8>(i,j) = sqrt(gx*gx + gy*gy);
		
	}
	}
	
	
}


/*--------------- MAIN FUNCTION ---------------*/
int main () {
// getTime()



// préparation d'acquisition de flux vidéo
  VideoCapture cap(0);
  if(!cap.isOpened()){
    cout << "Errore"; return -1;
  }
  
  ofstream o("time_sobel_basic.txt");

// déclaration des variables 
// Mat structure contenant l'image
  Mat3b frame; // couleur
  Mat frame_gray, frame1; // niveau de gris 
  Mat grad_x, grad_y;
  Mat abs_grad_y, abs_grad_x, grad;

  int ddepth = CV_16S;
  int scale = 1;
  int delta = 0;	
  char key = '0';

  cvNamedWindow("Video in", WINDOW_AUTOSIZE);
  cvNamedWindow("Video gray", WINDOW_AUTOSIZE);
  cvNamedWindow("Video mediane", WINDOW_AUTOSIZE);
  cvNamedWindow("Video contours", WINDOW_AUTOSIZE);
  cvMoveWindow("Video in", 10, 30);
  cvMoveWindow("Video gray", 800, 30);
  cvMoveWindow("Video mediane", 10, 500);
  cvMoveWindow("Video contours", 800, 500);
  
  
	
  while(key!='q'){
  // acquisition d'une image - librairie OpenCV
    
    
    frame = imread("img_proj.jpg", CV_LOAD_IMAGE_COLOR);
    
    if(! frame.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
  //conversion en niveau de gris - librairie OpenCV
    cvtColor(frame, frame_gray, CV_BGR2GRAY);

   // median - librairie OpenCV	
    medianBlur(frame_gray, frame1, 3);
    
    timeval time;
    gettimeofday(&time, NULL);
  double time1 = time.tv_sec+(time.tv_usec/1000000.0);
  
    //Sobel
    sobel(frame1, grad, frame1.rows, frame1.cols);
	
	gettimeofday(&time, NULL);
	double time2 = time.tv_sec+(time.tv_usec/1000000.0);
	o << (time2 - time1) << endl;
	// visualisation
	// taille d'image réduite pour meuilleure disposition sur écran
    resize(frame, frame, Size(), 0.5, 0.5);
    resize(frame_gray, frame_gray, Size(), 0.5, 0.5);
    resize(frame1, frame1, Size(), 0.5, 0.5);
    resize(grad, grad, Size(), 0.5, 0.5);
    imshow("Video in",frame);
    imshow("Video gray",frame_gray);
    imshow("Video mediane",frame1);    
    imshow("Video contours",grad);  
    
    
    key=waitKey(5);
  }
  // getTime()
  }
  


    
