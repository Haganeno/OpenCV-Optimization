all: projet_base projet_basic projet_opt projet_opt_thread

projet_base: projet_base.cpp
	g++ `pkg-config --cflags opencv` projet_base.cpp `pkg-config --libs opencv` -o projet_base

projet_basic: projet_sob_med_basic.cpp projet_sob_med_basic.h
	g++ `pkg-config --cflags opencv` projet_sob_med_basic.cpp `pkg-config --libs opencv` -o projet_basic
	
projet_opt: projet_sob_med_opt.cpp projet_sob_med_opt.h
	g++ `pkg-config --cflags opencv` projet_sob_med_opt.cpp `pkg-config --libs opencv` -o projet_opt

projet_opt_thread: projet_sob_med_opt.cpp projet_sob_med_opt.h
	g++ -std=c++11 -pthread `pkg-config --cflags opencv` projet_sob_med_opt_thread.cpp `pkg-config --libs opencv` -o projet_opt_thread

mrproper:
	rm projet_base projet_basic projet_opt projet_opt_thread

