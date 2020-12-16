#include "base.cpp"
#include <omp.h>

template<int height, int width>
struct omp_version : public base<height, width> {

    omp_version(const cell (&map)[height][width], int iterations) : base<height, width>(map, iterations),
                                                              iterations(iterations) {}

    size_t iterations;

    void operator()() {

        // create an 2 dim array
        cell **map_in = new cell* [height];
        for (int i = 0; i < height; i++) {
            map_in[i] = new cell[width];
        }

        // copy the values from the map to the
        // temporary array map_in
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                map_in[i][j] = this->map[i][j];
            }
        }

        // create another 2 dim array
        // where the results calculated for the next iteration
        // are written in
        cell **map_out = new cell* [height];
        for (int i = 0; i < height; i++) {
            map_out[i] = new cell[width]{};
        }

        for (size_t g = 0; g < iterations; g++) {

            #pragma omp parallel for
            for (size_t i = 1; i < height - 1; i++) {
                for (size_t j = 1; j < width - 1; j++) {
                    cell& c = map_in[i][j];
                    u_int8_t life = this->octagonal(map_in, i, j);

                    map_out[i][j] = this->rule(c, life);
                    //map_out[i][j] = life;

                }
            }

            std::swap(map_in, map_out);
            // std::cout << "iteration: " << g << std::endl;
        }

        // after the last generation copy the results back
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                this->map[i][j] = map_in[i][j];
            }
        }

        for (int i = 0; i < height; i++) {
            delete[] map_in[i];
            delete[] map_out[i];
        }
    }
};