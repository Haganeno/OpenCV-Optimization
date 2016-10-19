# OpenCV-Optimization
Code optimization of median and sobel filters using OpenCV

To compile the program simply use the Makefile by running the command `make` and `make mrproper` to delete the executables.

If there is a problem with the Makefile, use these commands to compile:

    For basic compiling : `g++ -std=c++0x -pthread `pkg-config --cflags opencv` projet_sabre.cpp `pkg-config --libs opencv` -o projet_sabre`
  
    For optimized compiling (O2) : `g++ -O2 -std=c++0x -pthread `pkg-config --cflags opencv` projet_sabre.cpp `pkg-config --libs opencv` -o projet_sabre_O2`
    
To run the program use the command `./executable_name param1 param2 param3`, here `./projet_sabre param1 param2 param3`.

Three paramaters are needeed : 

    - param1 : the median filter type (takes an Integer between 1 and 4)
    
    - param2 : the median window's diameter (takes an Integer superior or equal to 3)
    
    - param3 : the sobel filter type (takes an Integer between 1 and 4)
    
The 4 median filters types are :
    
    - Type 1 : OpenCV's own median filter
    
    - Type 2 : Basic implementation of a median filter using a sorting algorithm to determine the median.
    
    - Type 3 : Huang's linear median filter
    
    - Type 4 : Huand's linear multithreaded median filter 
    
The 4 Sobel filters types are :
    
    - Type 1 : OpenCV's own Sobel filter
    
    - Type 2 : Basic implementation of a Sobel filter, unoptimized
    
    - Type 3 : Optimized Sobel filter (loop unrolling, no double types, etc..)
    
    - Type 4 : Optimized multithreaded Sobel filter


Example :

    To run the program with OpenCV's own median filter (type 1) and the optimized multithreaded Sobel filter (type 4) with a median window diameter of 5 (5x5 window), the command to use is : `./projet_sabre 1 5 4`.

