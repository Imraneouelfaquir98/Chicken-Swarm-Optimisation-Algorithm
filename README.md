# Chicken-Swarm-Optimisation-Algorithm
This project aims to implement the chicken swarm optimization algorithm with the programming language C ++ and to apply it to the diagnosis of pneumonia with x-ray images using C++ version of OpenCV.

The implementation of the algorithm will be in the cso.cpp and cso.h files and the sample application will be in the main.cpp file.

The command lines to run this code on Linux system are as follows:

g++ -c main.cpp cso.cpp -std=c++11 `pkg-config --cflags --libs opencv`
g++ main.o cso.o -o output -std=c++11 `pkg-config --cflags --libs opencv`
./output
