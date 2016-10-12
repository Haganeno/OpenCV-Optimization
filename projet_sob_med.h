#include "highgui.h"
#include "cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

typedef unsigned char uint8;

void median_filter_hist_v1(cv::Mat img_gray, cv::Mat& img_out);
void median_filter_v1(cv::Mat img_gray, cv::Mat& img_out);
void sobel_opt(cv::Mat img_in, cv::Mat& img_out, int row, int col);
void sort(int[] array_in, int threshold);
void insertion_sort(int[] array_in, int n);
void quicksort(int[] array_in, int start, int end);
int part(int[] array, int s, int e);

