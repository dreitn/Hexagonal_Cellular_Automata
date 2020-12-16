#include <iostream>
#include "openCl.cpp"
#include "cell.cpp"
#include "openmp.cpp"
#include "sequential.cpp"
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

void validate_result(const auto &reference, const auto &other) {
    for (size_t i = 0; i < reference.h; i++) {
        for (size_t j = 0; j < reference.w; j++) {
            if ((u_char) reference.map[i][j] != (u_char)other.map[i][j]) {
                std::cerr << "- error, results are not the same!" << std::endl;
                return;
            }
        }
    }
    std::cout << "+ result seems to be ok" << std::endl;
}

int main() {
    constexpr size_t w = 1024, h = 1024, it = 2000;
    enum { hex, oct };

    std::cout << "size: " << h << "*" << w << " iterations: " << it << std::endl;

    cell map[h][w];
    setupMap(map);
    populateMap(map);

    sequential_version seq(map, it);
    std::cout << "sequential version: ";
    {
        Timer t;
        seq();
    }
    //seq.print();

    std::cout << "openmp version: ";
    omp_version omp(map, it);
    {
        Timer t;
        omp();
    }
    //omp.print();
    validate_result(seq, omp);

    std::cout << "openCl version: ";
    openCL_version openCl(map, it);
    {
	// Time messrement inside the function, reading the kernel and compiling takes long time 
        openCl();
    }
    //openCl.print();
    validate_result(seq, openCl);

}
