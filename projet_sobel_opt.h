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
void sobel_opt(cv::Mat img_in, cv::Mat& img_out, int row, int col);
