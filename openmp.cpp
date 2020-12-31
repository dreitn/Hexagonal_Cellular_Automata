#include "base.cpp"
#include "Timer.cpp"
#include <omp.h>

template<size_t height, size_t width>
struct omp_version : public base<height, width> {

    omp_version(const std::vector <std::vector<cell>> &map, int iterations)
            : base<height, width>(map, iterations), iterations(iterations) {}

    size_t iterations;

    void operator()() {

        cell *current_gen = new cell[height*width] {dead};
        cell *next_gen = new cell[height*width] {dead};

        // copy the values from the map to the
        // temporary array current_gen
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                current_gen[(i * width) + j] = this->map[i][j];
            }
        }

        Timer *t = new Timer;

        for (size_t g = 0; g < iterations; g++) {

            #pragma omp parallel for
            for (size_t i = 1; i < height - 1; i++) {
                for (size_t j = 1; j < width - 1; j++) {
                    cell &c = current_gen[(i * width) + j];
                    u_int8_t life = this->octagonal(current_gen, i, j);

                    next_gen[this->f(i, j)] = this->rule(c, life);
                }
            }
            std::swap(current_gen, next_gen);
        }

        delete t;

        // after the last generation copy the results back
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                this->map[i][j] = current_gen[(i * width) + j];
            }
        }

        delete[] current_gen;
        delete[] next_gen;

    }
};
