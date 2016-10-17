/*
 *---------------------------------------------------------------------------------------------------
* Pour compiler : g++ `pkg-config --cflags opencv` projet_sob_med_opt_thread.cpp `pkg-config --libs opencv` -o projet_sob_med_opt_thread
 *---------------------------------------------------------------------------------------------------
 */
 

#include "test.h"
 
using namespace std;
using namespace cv;
using std::cout;




/*--------------- MAIN FUNCTION ---------------*/
int main () {
	


	string s = "time_med_opt_3.txt";
  ofstream o(s.c_str());
  
  ofstream o2("time_sobel_opt_thread.txt");
// déclaration des variables 
// Mat structure contenant l'image
  Mat3b frame; // couleur
  Mat frame_gray, frame1; // niveau de gris 
  Mat grad;

  int ddepth = CV_16S;
  int scale = 1;
  int delta = 0;	
  char key = '0';

  cvNamedWindow("Video in", WINDOW_AUTOSIZE);
  cvNamedWindow("Video gray", WINDOW_AUTOSIZE);
  cvNamedWindow("Video mediaNW", WINDOW_AUTOSIZE);
  cvNamedWindow("Video contours", WINDOW_AUTOSIZE);
  cvMoveWindow("Video in", 10, 30);
  cvMoveWindow("Video gray", 800, 30);
  cvMoveWindow("Video mediaNW", 10, 500);
  cvMoveWindow("Video contours", 800, 500);

  while(key!='q'){
  // acquisition d'uNW image - librairie OpenCV
    
    frame = imread("img_proj.jpg", CV_LOAD_IMAGE_COLOR);
    
    if(! frame.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
  //conversion en niveau de gris - librairie OpenCV
    cvtColor(frame, frame_gray, CV_BGR2GRAY);

	timeval time;
    gettimeofday(&time, NULL);
  	double time1 = time.tv_sec+(time.tv_usec/1000000.0);
  

   // median - librairie OpenCV	
    median_filter_sort(frame_gray, frame1, 3); // change median filter sort by opt filter
    
    gettimeofday(&time, NULL);
	double time2 = time.tv_sec+(time.tv_usec/1000000.0);
	o << (time2 - time1) << endl;
    
    
    
    
    timeval NWw_time;
	gettimeofday(&NWw_time, NULL);
	double time3 = NWw_time.tv_sec+(NWw_time.tv_usec/1000000.0);
    
    //Sobel
    sobel_opt_thread(frame1, grad, frame1.rows, frame1.cols);

	gettimeofday(&NWw_time, NULL);
	double time4 = NWw_time.tv_sec+(NWw_time.tv_usec/1000000.0);
	o2 << (time4 - time3) << endl;
	
	
	// visualisation
	// taille d'image réduite pour meuilleure disposition sur écran
    resize(frame, frame, Size(), 0.5, 0.5);
    resize(frame_gray, frame_gray, Size(), 0.5, 0.5);
    resize(frame1, frame1, Size(), 0.5, 0.5);
    resize(grad, grad, Size(), 0.5, 0.5);
    imshow("Video in",frame);
    imshow("Video gray",frame_gray);
    imshow("Video mediaNW",frame1);    
    imshow("Video contours",grad);  
    
    
    key=waitKey(5);
  }
  }
  
  
/*--------------- FUNCTIONS ---------------*/


void sobel_opt_thread(Mat img_in, Mat& img_out, int row, int col) {


	uchar* in;
	uchar* s1_out;
	in = img_in.data;
	s1_out = (uchar*)calloc(row*col, sizeof(uchar));
	thread sobel_thread1(sobel_opt, in, s1_out, row/4, col);
	thread sobel_thread2(sobel_opt, in+row*col/4, s1_out+row*col/4, row/4, col);
	thread sobel_thread3(sobel_opt, in+row*col/2, s1_out+row*col/2, row/4, col);
	thread sobel_thread4(sobel_opt, in +3*(row*col)/4, s1_out+3*row*col/4, row/4, col);
	sobel_thread1.join();
	sobel_thread2.join();
	sobel_thread3.join();
	sobel_thread4.join();
	img_out = Mat(row, col, CV_8UC1, s1_out);
	

}
  
  void sobel_opt(uchar* im_in, uchar* im_out, int rw, int cl) {
	//TODO: améliorer le code et déroulage de boucle?
	int i = 0;
	uchar *NW, *N, *NE, *W, *E, *SW, *S, *SE;
	int Gx, Gy, G;

	for (i = 0,
		NW = im_in - cl - 1,d
		N = im_in - cl,
		NE = im_in - cl + 1,
		W = im_in - 1,
		E = im_in + 1,
		SW = im_in + cl - 1,
		S = im_in + cl,
		SE = im_in + cl + 1;
	i < (cl*rw);
	i++, NW++, N++, NE++, W++, E++, SW++, S++, SE++
		)
	{
		if (i < cl || i%cl == 0 || i%cl == cl - 1 || i >= (cl - 1)*rw) {    //cas où le pixel est sur un bord   
			im_out[i] = 0;
		}
		else{
			Gx = abs(-(*NW) - (*W<<1) - (*SW) + (*NE) + (*E<<1) + (*SE));
			Gy = abs(-(*NW) - (*N<<1) - (*NE) + (*SW) + (*S<<1) + (*SE));
			G = (Gx + Gy)>>1;
			im_out[i] = (uchar)G;
		}
	}
	}
	




// filtre median avec trie (non opt)
 // r indique le diam?tre du filtre ? utiliser (doit ?tre SWair et >=3)
  void median_filter_sort(Mat img_gray, Mat& img_out, int d) {
  
  	if(d%2 == 0 || d < 3){
    	cout << "Error, must be odd number or >= 3"<<endl; return;
  	}
  	
  	int n = img_gray.rows;
  	int m = img_gray.cols;
  	
  	img_gray.copyTo(img_out);
  	  		
  	// parcourt l'image
  	for(int i = 1; i < n-1; i++) {
  		for(int j = 1; j < m-1; j++) {
  			int window [d*d];
  			int k = 0;
			//if(i-(d/2) >= 0 && j-(d/2) >=0 && i+(d/2) <n && j+(d/2) < m){
				// recupere uNW feNWtre dxd autour de l'element en (i,j)
				// faire un déroulage de boucle?
	  			for(int h = i-(d/2); h <= i+(d/2);h++) {
	  				for(int l = j-(d/2); l <= j+(d/2);l++) {
						if(h < 0 || h >= n|| j < 0|| j >= m) window[k] = 0;
						else window[k] = img_gray.at<uint8>(h, l);
						k++;	
	  				}
	  			}  
				sort(window, d*d, 20); // trie la feNWtre (insertion_sort pour 3x3, quicksort pour plus grand
				img_out.at<uint8>(i, j) = window[4];// recupere le median de la feNWtre triee
  			}
  		
  		
  	}
  	
  }



// SWl?mente le tri rapide pour le filtre median ? grand rayon (>20)
  // Sinon, utilise le tri par insertion pour les petits tableaux(<=20):
  // Insertion sort est un algo de tri quadratique en g?n?ral
  // mais tr?s rapide quand le tableau ? trier est petit
  void sort(int array[], int size, int threshold) {
	  int n = size;
	  // tri par insertion pour les petits tableaux
	  if(n < threshold) insertion_sort(array, n);
	  // sinon, quicksort
	  else quicksort(array, 0, n-1);
	  
	 
  }
  
  void insertion_sort(int array[], int n) {
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
  
  
  void quicksort(int array[], int start, int end) {
  	if(start < end){
  		int p = part(array, start, end);
  		quicksort(array, start, p-1);
  		quicksort(array, p+1, end);
  	}
  }
  
  
  // inspiré de algolist.NWt/Algorithms/Sorting/Quicksort
  int part(int array[], int s, int e) {
  	int i = s, j = e;
  	int tmp;
  	int pivot = array[(s + e) / 2];
  	
  	while(i <= j) {
  		while(array[i] <= pivot) {
  			i++;
  		}
  		
  		while(array[j] > pivot) {
  			j--;
  		}
  		
  		if(i <= j) {
	  		swap(array[i], array[j]);
  		}
  	}
  		
  	return j;	
  }
  
  void swap(int& a, int& b) {
  	
  	int temp = a;
  	a = b;
  	b = temp;
  	
  }
  
   
