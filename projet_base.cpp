/*
 * Fichier source pour le projet d'unité
 *  INF-4101C
 *---------------------------------------------------------------------------------------------------
 * Pour compiler : g++ `pkg-config --cflags opencv` projet.cpp `pkg-config --libs opencv` -o projet
 *---------------------------------------------------------------------------------------------------
 * auteur : Eva Dokladalova 09/2015
 */
 
#include <fstream>
#include <stdlib.h>

/* Libraries OPENCV */
#include "highgui.h"
#include "cv.h"
#include <sys/time.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
 
using namespace std;
using namespace cv;
using std::cout;
/*--------------- MAIN FUNCTION ---------------*/
int main () {

  ofstream o("time_base.txt");
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

    // calcul du gradient- librairie OpenCV
    /// Gradient Y
    Sobel( frame1, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    /// Gradient Y
    Sobel( frame1, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad ); 	
    
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
}

    
