#pragma once
#include "cell.cpp"
#include <iostream>
#include <vector>

template <size_t height, size_t width>
struct base {
    static const int h = height;
    static const int w = width;

    const int iterations;

    std::vector<std::vector<cell>> map;

    base(const std::vector<std::vector<cell>>& map, int iterations) : map(map), iterations(iterations) {}

/* classic game of life rule
    * •“ Birth” happens in the centre if there are three neighbours.
    * • The centre cell “Survives” if there are two or three neighbours.
    * • Otherwise, all the exceptions result in death.
    */
    inline cell rule(const cell &cell, u_int8_t life) const {
        if (cell == dead && life == 3) return alive;
        if ((cell == alive) && (life == 2 || life == 3)) return alive;

        return dead;
    }

    inline uint8_t octagonal(cell** map_in, int i, int j) const {
        return map_in[i-1][j-1] + map_in[i-1][j] + map_in[i-1][j+1] +
               map_in[ i ][j-1] +                  map_in[ i ][j+1] +
               map_in[i+1][j-1] + map_in[i+1][j] + map_in[i+1][j+1];
    }

    inline int f(int i, int j) const {
        return (i * w) + j;
    };

    inline uint8_t octagonal(cell* map, int i, int j) const {
        return map[f(i-1, j-1 )] + map[f(i-1, j )] + map[f(i-1, j+1)] +
               map[f( i , j-1 )] +                  map[f( i , j+1)] +
               map[f(i+1, j-1 )] + map[f(i+1, j )] + map[f(i+1, j+1)];
    }

    inline uint8_t hexagonal(cell* map, int i, int j) const {
        return map[f(i-1, j-1)] + map[f(i-1, j )] +
               map[f( i , j-1)]                           + map[f(i, j+1)] +
                                         + map[f(i+1, j)] + map[f(i+1, j+1)];
    }

    inline uint8_t hexagonal(cell** map_in, int i, int j) const {
        return map_in[i-1][j-1] + map_in[i-1][j] +
               map_in[ i ][j-1]                  + map_in[ i ][j+1] +
                                + map_in[i+1][j] + map_in[i+1][j+1];
    }
};
