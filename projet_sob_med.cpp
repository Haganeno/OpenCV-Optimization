/*
 *---------------------------------------------------------------------------------------------------
* Pour compiler : g++ `pkg-config --cflags opencv` projet_base.cpp `pkg-config --libs opencv` -o projet
 *---------------------------------------------------------------------------------------------------
 */
 

#include "projet_sob_med.h"
 
using namespace std;
using namespace cv;
using std::cout;




/*--------------- MAIN FUNCTION ---------------*/
int main () {

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
    
    //Sobel
    sobel_opt(frame1, grad, frame.rows, frame.cols);

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
  
  
 /*--------------- FUNCTIONS ---------------*/ 
 
 
 void median_filter_hist_v1(Mat img_gray, Mat& img_out){
	 
 }
 
 
 // filtre median avec trie (non opt)
 // r indique le diamètre du filtre à utiliser (doit être impair et >=3)
  void median_filter_sort_v1(Mat img_gray, Mat& img_out, int d) {
  	
  	if(d%2 != 0){
    	cout << "Error, must be odd number"; return -1;
  	}
  	
  	int n = img_gray.rows;
  	int m = img_gray.cols;
  	
  	img_gray.copyTo(img_out);
  	  	
  	// parcourt l'image
  	for(int i = 1; i < n; i++) {
  		for(int j = 1; j < m; j++) {
  			int window [d*d];
  			int k = 0;
			
			// recupere une fenetre 3x3 autour de l'element en (i,j)
			// faire un déroulage de boucle
  			for(int h = i-r+(d/2); h < i+r-(d/2);h++) {
  				for(int l = j-r+(d/2); l < j+r-(d/2);l++) {
  					window[k] = img_gray.at<int>(h, l);
					k++;
  				}
  			}
			sort(window, 20); // trie la fenetre
			img_out.at<int>(i, j) = window[4]// recupere le median de la fenetre triee
  		}
  	}
  	
  }
  
  
void sobel_opt(Mat img_in, Mat& img_out, int row, int col) {

	int n;
	int m;
	n = row;
	m = col;
	
	img_in.copyTo(img_out);
	
	uint8 N, NE, NW, S, SE, SW, W, E, W2, NE2, SE2, E2;
	int gx, gy;
	int gx2, gy2;
	
	int i, j;
	
	
	for(i = 1; i < n-1; i++) {
	    for(j = 1; j < m-1; j=j+2) {
	    
		NW = img_in.at<uint8>(i-1, j-1);//[i - n - 1];
		N = img_in.at<uint8>(i-1, j);//[i - n];
		NE = img_in.at<uint8>(i-1,j+1);//[i - n + 1];
		NE2 = img_in.at<uint8>(i-1,j+2);//[i - n + 2];
		
		W = img_in.at<uint8>(i,j-1);//[i - 1];
		W2 = img_in.at<uint8>(i,j);//[i];
		E = img_in.at<uint8>(i,j+1);//[i + 1];
		E2 = img_in.at<uint8>(i,j+2);//[i + 2];
		
		
		SW = img_in.at<uint8>(i+1,j-1);//[i + n - 1];
		S = img_in.at<uint8>(i+1,j);//[i + n];
		SE = img_in.at<uint8>(i+1,j+1);//[i + n + 1];
		SE2 = img_in.at<uint8>(i+1,j+2);//[i + n + 2];
		 
		
		gx = abs(NE + (E<<1) + SE - NW - (W<<1) - SW);
		gy = abs(NW + (N<<1) + NE - SW - (S<<1) - SE);
				
		img_out.at<uint8>(i,j) = (gx + gy)>>1;
		
      
		gx2 = abs(NE2 + (E2<<1) + SE2 - N - (W2<<1) - S);
		gy2 = abs(N + (NE<<1) + NE2 - S - (SE<<1) - SE2);
		
		img_out.at<uint8>(i,j+1) = (gx2 + gy2)>>1;
	}
	}
	
}


// Implémente le tri rapide pour le filtre median à grand rayon (>20)
  // Sinon, utilise le tri par insertion pour les petits tableaux(<=20):
  // Insertion sort est un algo de tri quadratique en général
  // mais très rapide quand le tableau à trier est petit
  void sort(int[] array, int threshold) {
  
	  int n = array.size();
	  
	  // tri par insertion pour les petits tableaux
	  if(n <= threshold) insertion_sort(array, n);
	  // sinon, quicksort
	  else quicksort(array, 0, n-1);
  }
  
  void insertion_sort(int[] array, int n) {
	for(int i = 1; i < n; i++) {
			  int j = i;
			  while(j > 0 && array[j-1] > array[j]) {
				  int t = array[j];
				  array[j] = array[j-1];
				  array[j-1] = t;
				  j--;
			  }
				  
		  }
  }
  
  void quicksort(int[] array, int start, int end) {
  	if(start < end){
  		p = part(array, start, end);
  		quicksort(array, start, p);
  		quicksort(array, p, end);
  	}
  }
  
  int part(int[] array, int s, int e) {
  	int pivot = array[e];
  	int i = s;
  	for(int j = s; j < e - 1; j++) {
  		if(array[j] <= pivot) {
  			int temp = array[i];
  			array[i] = array[j];
  			array[j] = temp;
  			i++;
  		}
  	}
  	int temp = array[i];
  	array[i] = array[j];
  	array[j] = temp;
  	
  	return i;	
  }
  


    
