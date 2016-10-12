#include "highgui.h"
#include "cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
typedef unsigned char uint8;
void sobel(cv::Mat img_in, cv::Mat& img_out, int row, int col);
void median_filter_sort(cv::Mat img_gray, cv::Mat& img_out, int d);
void sort(int array_in[], int size, int threshold);
void insertion_sort(int array_in[], int n);
void quicksort(int array_in[], int start, int end);
int part(int array[], int s, int e);
void swap(int array[], int i, int j);
