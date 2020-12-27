#include <iostream>
#include "openCl.cpp"
#include "cell.cpp"
#include "openmp.cpp"
#include "sequential.cpp"
#include "Timer.cpp"


template<int height, int width>
void setupMap(auto& map) {
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
void populateMap(auto& map) {
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
    constexpr size_t h = 1536, w = 1536, it = 1000;
    std::cout << "height: " << h << " width: " << w  << " iterations: " << it << " size: " << ((w * h) / 1024)  /1024 << "Gb\n";
    enum { hex, oct };

    //std::cout << "size: " << h << "*" << w << " iterations: " << it << std::endl;

    std::vector<std::vector<cell>> map = std::vector(h, std::vector<cell>(w, dead));
    setupMap<h, w>(map);
    populateMap<h, w>(map);

    sequential_version<h, w> seq(map, it);
    std::cout << "sequential version: ";
    seq();
    //seq.print();

    {
       std::cout << "openmp version: ";
       omp_version<h, w> omp(map, it);
       // Messure only the calculation, not copying
       omp();
       //omp.print();
       validate_result(seq, omp);
    }
    std::cout << "openCl version: ";
    openCL_version<h, w> openCl(map, it);
    {
	// Time messurement inside the function, reading the kernel and compiling takes long time
        openCl();
    }
    //openCl.print();
    validate_result(seq, openCl);

}
