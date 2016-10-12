all: projet_base projet_sobel_basic projet_sobel_opt

projet_base: projet_base.cpp
	g++ `pkg-config --cflags opencv` projet_base.cpp `pkg-config --libs opencv` -o projet_base

projet_sobel_basic: projet_sobel_basic.cpp projet_sobel_basic.h
	g++ `pkg-config --cflags opencv` projet_sobel_basic.cpp `pkg-config --libs opencv` -o projet_sobel_basic
	
projet_sobel_opt: projet_sobel_opt.cpp projet_sobel_opt.h
	g++ `pkg-config --cflags opencv` projet_sobel_opt.cpp `pkg-config --libs opencv` -o projet_sobel_opt

