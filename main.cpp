#include <iostream>
#include <omp.h>
#include "openCl.cpp"
#include "cell.cpp"
#include "omp.cpp"
#include "serial.cpp"
#include "Timer.cpp"


template<int height, int width>
void setupMap(cell (&map)[height][width]) {
    for (size_t i = 0; i < width; i++) {
        map[0][i] = dead;
        map[height - 1][i] = dead;
    }

    for (size_t i = 0; i < height; i++) {
        map[i][0] = dead;
        map[i][width - 1] = dead;
    }
}

template<int height, int width>
void populateMap(auto (&map)[height][width]) {
    for (size_t i = 1; i < height - 1; i++) {
        for (size_t j = 1; j < width - 1; j++) {
            map[i][j] = static_cast<cell>( rand() % states );
        }
    }
}

void validate_result(const auto& reference, const auto& other) {
    for (size_t i = 0; i < reference.h; i++) {
        for (size_t j = 0; j < reference.w; j++) {
            if ((u_char)reference.map[i][j] != (u_char)other.map[i][j]) {
                std::cout << "i: " << i << " j: " << j << std::endl;
                std::cout << (int)reference.map[i][j] << " " << (int)other.map[i][j] << std::endl;
                std::cout << "error, results are not the same!\n" << std::endl;
                return;
            }
        }
    }
}

int main() {
    constexpr int w = 256, h = 256, it = 2000;

    cell map[h][w];
    setupMap(map);
    populateMap(map);

    serial_version serial(map, it);
    {
        Timer t;
        serial();
    }
    //serial.print();

    std::cout << std::endl;

    omp_version omp(map, it);
    {
        Timer t;
        omp();
    }
    //omp.print();
    validate_result(serial, omp);

    openCL_version openCl(map, it);
    {
        Timer t;
        openCl();
    }
    //openCl.print();
    //validate_result(serial, openCl);
}
