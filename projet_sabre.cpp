/*
 *---------------------------------------------------------------------------------------------------
* Pour compiler : g++ `pkg-config --cflags opencv` projet_sob_med_opt_thread.cpp `pkg-config --libs opencv` -o projet_sob_med_opt_thread
 *---------------------------------------------------------------------------------------------------
 */
 

#include "projet_sabre.h"
 
using namespace std;
using namespace cv;
using std::cout;




/*--------------- MAIN FUNCTION ---------------*/
int main (int argc, char* argv[]) {
	
	
	if(argc <= 3) {
	cout << "Need to specify sobel and median filter type"<<endl<<"Need number >=3 for median filter" 		<< endl << "See README for instructions on how to execute with appropriate parameters"<<endl;
	return 0;
	}
	
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Error";
		return -1;
	}
	
	// Handles parameters
	string s, s2;
	switch(stoi(argv[1])) {
		case 1: //opencv
			s = "time_med_opencv_"+string(argv[2])+".txt";
			break;
		case 2: //basic (sort)
			s = "time_med_basic_"+string(argv[2])+".txt";
			break;
		case 3: //opt
			s = "time_med_opt_"+string(argv[2])+".txt";
			break;
		case 4: //thread
			s = "time_med_thread_"+string(argv[2])+".txt";
			break;
		default:
			break; 
    }
    
    switch(stoi(argv[3])) {
		case 1: //opencv
			s2 = "time_sob_opencv_"+string(argv[2])+".txt";
			break;
		case 2: //basic (sort)
			s2 = "time_sob_basic_"+string(argv[2])+".txt";
			break;
		case 3: //opt
			s2 = "time_sob_opt_"+string(argv[2])+".txt";
			break;
		case 4: //thread
			s2 = "time_sob_thread_"+string(argv[2])+".txt";
			break;
		default:
			break; 
    }
    
  ofstream o(s.c_str());
  ofstream o2(s2.c_str());
  
  
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
    
    cap.read(frame);
    /*pour filtrer seulement une image
    frame = imread("img_proj.jpg", CV_LOAD_IMAGE_COLOR);*/
    
    if(! frame.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
  //conversion en niveau de gris - librairie OpenCV
    cvtColor(frame, frame_gray, CV_BGR2GRAY);

	// Calcul du temps d'execution du filtre median
	timeval time;
    gettimeofday(&time, NULL);
  	double time1 = time.tv_sec+(time.tv_usec/1000000.0);
  

   // median
	switch(stoi(argv[1])) {
		case 1: //opencv
			medianBlur(frame_gray, frame1, atoi(argv[2]));
			break;
		case 2: //basic (sort)
			median_basic(frame_gray, frame1, atoi(argv[2])); 
			break;
		case 3: { //opt
			uchar* med_out = (uchar*)calloc(frame_gray.rows*frame_gray.cols, sizeof(uchar));
			median_hist(frame_gray.data, med_out, atoi(argv[2]), frame_gray.rows, frame_gray.cols);
			frame1 = Mat(frame_gray.rows, frame_gray.cols, CV_8UC1, med_out);
			break;
			}
		case 4: //thread
			median_hist_thread(frame_gray, frame1, atoi(argv[2]), frame_gray.rows, frame_gray.cols);
			break;
		default:
			median_hist_thread(frame_gray, frame1, atoi(argv[2]), frame_gray.rows, frame_gray.cols);
			break; 
    }
    

    gettimeofday(&time, NULL);
	double time2 = time.tv_sec+(time.tv_usec/1000000.0);
	o << (time2 - time1) << endl;
    
    
    
    // Calcul du temps d'execution du filtre Sobel
    timeval new_time;
	gettimeofday(&new_time, NULL);
	double time3 = new_time.tv_sec+(new_time.tv_usec/1000000.0);


    //Sobel
    switch(stoi(argv[3])) {
		case 1: //opencv
			Sobel( frame1, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
			convertScaleAbs( grad_x, abs_grad_x );
			Sobel( frame1, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
			convertScaleAbs( grad_y, abs_grad_y );
			addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad ); 	
			break;
		case 2: //basic (naif)
			sobel_basic(frame1, grad, frame1.rows, frame1.cols);
			break;
		case 3: {	//opt
			uchar* out = (uchar*)calloc(frame1.rows*frame1.cols, sizeof(uchar));
			sobel_opt(frame1.data, out, frame1.rows, frame1.cols);
			grad = Mat(frame1.rows, frame1.cols, CV_8UC1, out);
			break;
			}
		case 4: //thread
			sobel_opt_thread(frame1, grad, frame1.rows, frame1.cols);
			break;
		default:
			sobel_opt_thread(frame1, grad, frame1.rows, frame1.cols);
			break; 
    }
    


	gettimeofday(&new_time, NULL);
	double time4 = new_time.tv_sec+(new_time.tv_usec/1000000.0);
	o2 << (time4 - time3) << endl;
	
	
	// visualisation
	// taille d'image réduite pour meilleure disposition sur écran
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
  
  
/*--------------- FUNCTIONS ---------------*/

// Sobel multithreadé
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



  // Sobel optimisé
  // On utilise maintenant des uchar* et pas des Mat pour faciliter la partion
  // en 4 de l'image
  void sobel_opt(uchar* img_in, uchar* img_out, int row, int col) {
	
	
	int NW, N, NE, W, E, SW, S, SE, W2, NE2, SE2, E2;
	int gx, gy, gx2, gy2;
	
	for (int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++) {
			/*if(j < 1 || j >= col - 1 || i < 1|| i >= row - 1) { //cas où le pixel est sur un bord   
				img_out[i*col + j] = img_in[i*col +j];
			}
			else{*/
			
			NW = img_in[i*col + j - col - 1];
			N = img_in[i*col + j - col];
			NE = img_in[i*col + j - col + 1];
			NE2 = img_in[i*col + j - col + 2];
			
			W = img_in[i*col + j - 1];
			W2 = img_in[i*col + j];
			E = img_in[i*col + j + 1];
			E2 = img_in[i*col + j + 2];
			
			
			SW = img_in[i*col + j + col - 1];
			S = img_in[i*col + j + col];
			SE = img_in[i*col + j + col + 1];
			SE2 = img_in[i*col + j + col + 2];
				
				
			gx = abs(NE + (E<<1) + SE - NW - (W<<1) - SW);
			gy = abs(NW + (N<<1) + NE - SW - (S<<1) - SE);
			
			gx2 = abs(NE2 + (E2<<1) + SE2 - N - (W2<<1) - S);
			gy2 = abs(N + (NE<<1) + NE2 - S - (SE<<1) - SE2);				
			
			
			img_out[i*col + j] = (uchar)((gx + gy)>>1);
			img_out[i*col + j + 1] = (uchar)((gx2 + gy2)>>1);
			//}
	}
	}

}

	
	//Sobel naif
  void sobel_basic(Mat img_in, Mat& img_out, int row, int col) {
	
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

	void median_hist_thread(Mat img_in, Mat& img_out, int d, int row, int col) {	
	
		uchar* in = img_in.data;
		uchar* out = (uchar*)calloc(row*col, sizeof(uchar));
		
		thread sobel_thread1(median_hist, in, out, d, row/4, col);
		thread sobel_thread2(median_hist, in+row*col/4, out+row*col/4, d, row/4, col);
		thread sobel_thread3(median_hist, in+row*col/2, out+row*col/2, d, row/4, col);
		thread sobel_thread4(median_hist, in+3*(row*col)/4, out+3*row*col/4, d, row/4, col);
		sobel_thread1.join();
		sobel_thread2.join();
		sobel_thread3.join();
		sobel_thread4.join();
		
		img_out = Mat(row, col, CV_8UC1, out);
	}
	
	
	void median_hist(uchar* img_in, uchar* img_out, int d, int row, int col) {
	
		int r = d/2;
		int hist[256];
		for(int i = 0; i < row; i++) {
			for(int j = 0; j < col; j++) {
				/*if(j < r || j >= col - r || i < r|| i >= row - r) // bords
					img_out[i*col + j] = img_in[i*col+j];
				else {*/
					for(int p = 0; p <= 255; p++) { // (re)initialise l'histogramme
						hist[p] = 0;
					}

					for(int k = 0; k != d; k++) { // remplit l'histogramme
						for(int l = 0; l != d; l++) {
							hist[int(img_in[i*col + j - r*col - r + l + k*col])]++;//i - r*col - r + l + k*col])]++;
						}
					}
					int med = 0, m = 0;
					while(med < d*d/2) {
						med += hist[m];
						m++;
					}

					img_out[i*col + j] = m;
				
			}
		}
	}


// filtre median avec trie (non opt)
 // r indique le diametre du filtre a utiliser (doit etre pair et >=3)
  void median_basic(Mat img_gray, Mat& img_out, int d) {
  
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
				// recupere une fenetre dxd autour de l'element en (i,j)
	  			for(int h = i-(d/2); h <= i+(d/2);h++) {
	  				for(int l = j-(d/2); l <= j+(d/2);l++) {
						if(h < 0 || h >= n|| j < 0|| j >= m) window[k] = 0;
						else window[k] = img_gray.at<uint8>(h, l);
						k++;	
	  				}
	  			}  
				sort(window, d*d, 20); // trie la fenetre (insertion_sort pour 3x3, quicksort pour plus grand
				img_out.at<uint8>(i, j) = window[4];// recupere le median de la fenetre triee
  			}
  		
  		
  	}
  	
  }



// Implemente le tri rapide pour le filtre median à grand rayon (>20)
  // Sinon, utilise le tri par insertion pour les petits tableaux(<=20):
  // Insertion sort est un algo de tri quadratique en général
  // mais très rapide quand le tableau à trier est petit
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
  
  
  // inspiré de algolist.net/Algorithms/Sorting/Quicksort
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
  
   
