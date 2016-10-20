all: projet_sabre projet_sabre_O2

projet_sabre: projet_sabre.cpp projet_sabre.h
	g++ -std=c++0x -pthread `pkg-config --cflags opencv` projet_sabre.cpp `pkg-config --libs opencv` -o projet_sabre
	
projet_sabre_O2: projet_sabre.cpp projet_sabre.h
	g++ -O2 -std=c++0x -pthread `pkg-config --cflags opencv` projet_sabre.cpp `pkg-config --libs opencv` -o projet_sabre_O2

mrproper:
	rm projet_sabre projet_sabre_O2

