# Hexagonal_Cellular_Automata
Conways game of life hexagonal

make run

g++ -std=c++20 -O2 -Wall -Wextra -fopenmp -lOpenCL -pedantic -o game_of_life main.cpp ./game_of_life

size: 1024*1024 iterations: 2000

sequential version: 4.274664s

openmp version: 1.189948s

+ result seems to be ok

openCl version: 0.226511s

+ result seems to be ok
 
