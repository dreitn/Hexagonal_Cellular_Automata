#pragma once
#import "cell.cpp"
#include <iostream>


template <int height, int width>
struct base {
    static const int h = height;
    static const int w = width;

    cell map[height][width];
    const int iterations;

    base(cell (&m)[height][width], int iterations) : iterations(iterations) {
        std::copy(&m[0][0], &m[0][0]+height*width,&map[0][0]);
    }
    const cell& operator[](int i){
        return &(this->map[i]);
    }


/* classic game of life rule
    * •“ Birth” happens in the centre if there are three neighbours.
    * • The centre cell “Survives” if there are two or three neighbours.
    * • Otherwise, all the exceptions result in death.
    */
    inline cell rule(const cell &cell, u_int8_t life) {
        if (cell == dead && life == 3) return alive;
        if (cell == alive && life == 2 || life == 3) return alive;

        return dead;
    }

    inline u_int8_t count_life(int i, int j) {
        return map[i-1][j-1] + map[i][j-1] + map[i][j-1] + map[i][j+1] + map[i][j+1] + map[i+1][j+1];
    }

    void print() {
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                if (map[i][j] == alive) std::cout << '.' << ' ';
                if (map[i][j] == dead) std::cout << 'X' << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

};