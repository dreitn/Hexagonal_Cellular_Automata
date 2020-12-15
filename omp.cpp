#include "base.cpp"
#include <omp.h>

template <int height, int width>
struct omp_version : public base <height, width> {

    omp_version(cell (&map) [height][width], int iterations) : base<height, width> (map, iterations), iterations(iterations) {}
    int iterations;

    void operator()() {
        for (int g = 0; g < iterations; g++) {
            #pragma parallel for
            for (int i = 1; i < height-1; i++) {
                for (int j = 1; j < width-1; j++) {
                    auto& cell = this->map[i][j];
                    auto life = this->count_life(i, j);

                    cell = this->rule(cell, life);
                }
            }
        }
    }
};