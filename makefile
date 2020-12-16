CXXFLAGS= -std=c++20 -O2 -Wall -Wextra -fopenmp -lOpenCL -pedantic 
CXX=g++
all: main.cpp
	$(CXX) $(CXXFLAGS)-o game_of_life main.cpp
	
run: all
	./game_of_life

clean: 
	$(RM) game_of_life
