#include <cstdlib>
#include "cell.cpp"
#include "base.cpp"

template <int height, int width>
struct serial_version : public base<height,width> {
    int iterations;

    serial_version(cell (&map)[height][width], int iterations) : base<height, width>(map, iterations), iterations(iterations) {}

    void operator()() {
        for (size_t g = 0; g < iterations; g++) {
            for (size_t i = 1; i < height-1; i++) {
                for (size_t j = 1; j < width-1; j++) {
                    cell &c = this->map[i][j];
                    u_int8_t life = this->count_life(i, j);

                    c = this->rule(c, life);
                }
            }
        }
    }
};
