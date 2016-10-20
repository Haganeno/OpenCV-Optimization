all: projet_base projet_basic projet_opt projet_opt_thread projet_sabre

projet_base: projet_base.cpp
	g++ `pkg-config --cflags opencv` projet_base.cpp `pkg-config --libs opencv` -o projet_base

projet_basic: projet_sob_med_basic.cpp projet_sob_med_basic.h
	g++ `pkg-config --cflags opencv` projet_sob_med_basic.cpp `pkg-config --libs opencv` -o projet_basic
	
projet_opt: projet_sob_med_opt.cpp projet_sob_med_opt.h
	g++ -std=c++0x `pkg-config --cflags opencv` projet_sob_med_opt.cpp histograms.cpp `pkg-config --libs opencv` -o projet_opt

projet_opt_thread: projet_sob_med_opt.cpp projet_sob_med_opt.h
	g++ -std=c++0x -pthread `pkg-config --cflags opencv` projet_sob_med_opt_thread.cpp histograms.cpp `pkg-config --libs opencv` -o projet_opt_thread

projet_sabre: projet_sabre.cpp projet_sabre.h
	g++ -std=c++0x -pthread `pkg-config --cflags opencv` projet_sabre.cpp histograms.cpp `pkg-config --libs opencv` -o projet_sabre
	
#projet_sabre_O2: projet_sabre.cpp projet_sabre.h
#	g++ -O2 -std=c++0x -pthread `pkg-config --cflags opencv` projet_sabre.cpp histograms.cpp `pkg-#config --libs opencv` -o projet_sabre_O2

mrproper:
	rm projet_base projet_basic projet_opt projet_opt_thread projet_sabre projet_sabre_O2

