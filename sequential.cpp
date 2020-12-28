#include <cstdlib>
#include <functional>
#include "Timer.cpp"
#include "cell.cpp"
#include "base.cpp"
#include "omp.h"

template<size_t height, size_t width>
struct sequential_version : public base<height, width> {
    size_t iterations;

    sequential_version(const std::vector<std::vector<cell>>& map, int iterations) :
        base<height, width>(map, iterations), iterations(iterations) {}

    void operator()() {
        // create an 2 dim array
        cell **current_gen = new cell *[height];
        for (int i = 0; i < height; i++) {
            current_gen[i] = new cell[width];
        }

        // copy the values from the map to the
        // temporary array map_in
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                current_gen[i][j] = this->map[i][j];
            }
        }

        // create another 2 dim array
        // where the results calculated for the next iteration
        // are written in
        cell **next_gen = new cell *[height];
        for (int i = 0; i < height; i++) {
            next_gen[i] = new cell[width]{};
        }

	Timer* t = new Timer;
        for (size_t g = 0; g < iterations; g++) {

            for (size_t i = 1; i < height - 1; i++) {
                for (size_t j = 1; j < width - 1; j++) {
                    cell& c = current_gen[i][j];
                    u_int8_t life = this->octagonal(current_gen, i, j);

                    next_gen[i][j] = this->rule(c, life);
                    //map_out[i][j] = life; // Write amount of living neigbour cells instead

                }
            }
            std::swap(current_gen, next_gen);
        }
	delete t;
        // after the last generation copy the results back
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                this->map[i][j] = current_gen[i][j];
            }
        }

        for (int i = 0; i < height; i++) {
            delete[] current_gen[i];
            delete[] next_gen[i];
        }
        delete[] current_gen;
        delete[] next_gen;
    }
};
