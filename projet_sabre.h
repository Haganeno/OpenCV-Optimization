#include "highgui.h"
#include "cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <pthread.h> 
#include <thread> 
#include <stdint.h>
#include <sys/time.h>

typedef unsigned char uint8;
void sobel_opt_thread(cv::Mat img_in, cv::Mat& img_out, int row, int col);
void sobel_opt(uchar*, uchar*, int row, int col);
void sobel_basic(cv::Mat frame1, cv::Mat& grad, int rows, int cols);
void median_basic(cv::Mat img_gray, cv::Mat& img_out, int d);
void median_hist(uchar* img_in, uchar* img_out, int d, int row, int col);
void median_hist_thread(cv::Mat img_in, cv::Mat& img_out, int d, int row, int col);
void sort(int array_in[], int size, int threshold);
void insertion_sort(int array_in[], int n);
void quicksort(int array_in[], int start, int end);
int part(int array[], int s, int e);
void swap(int array[], int i, int j);
void threshold(cv::Mat in, cv::Mat& ou, int rows, int cols, int s);
