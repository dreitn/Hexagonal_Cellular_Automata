CXXFLAGS= -I/usr/local/cuda-10.2/targets/x86_64-linux/include/ -L/usr/local/cuda-10.2/targets/x86_64-linux/lib/ -std=c++2a -fconcepts -O2 -fopenmp -lOpenCL
CXX=g++
all: main.cpp
	$(CXX) $(CXXFLAGS) -o game_of_life main.cpp

run: all
	./game_of_life

clean:
	$(RM) game_of_life
