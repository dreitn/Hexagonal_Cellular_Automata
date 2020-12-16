
char get(__global const char *map, int y, int x, int gw) {
    return map[y * gw + x];
}

__kernel void octagonal(__global const char *map, int height, int width, __global char *map_out) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    if(x < 1 || x >= width-1 || y < 1 || y >= height-1) return;

    int gw = get_global_size(0); // gobal width
    //int gh = get_global_size(1); // global height

    char life = get(map, y-1, x-1, gw) +
                get(map, y-1, x  , gw) +
                get(map, y-1, x+1, gw) +

                get(map, y, x-1, gw) +
                get(map, y, x+1, gw) +

                get(map, y+1, x-1, gw) +
                get(map, y+1, x  , gw) +
                get(map, y+1, x+1, gw);


    // write the amount of living neighbour cells instead
    // map_out[y * gw + x] = life;
    // return;

   // if the cell is dead and has 3 neighbors then cell comes alive
    if(get(map, y, x, gw) == 0) {
        if (life == 3) {
            map_out[y*gw + x] = 1;
            return;
        }
    }

    // if the cell is alive and has 2 or 3 neighbors, cell stays alive
    if(get(map, y, x, gw) == 1) {
        if (life == 2 || life == 3) {
            map_out[y*gw + x] = 1;
            return;
        }
    }

    // in all others cases the cell dies
    map_out[y*gw + x] = 0;
}

