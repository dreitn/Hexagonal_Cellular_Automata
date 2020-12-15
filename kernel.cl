
char get(__global char *map, int x, int y, int w, int h, int gw, int gh) {
    //if( x < 0 || y < 0 || x >= w || y >= h ) return 0;
    //else
    return map[y * gw + x];
}

__kernel void calculate(__global const char *map, int height, int width, __global char *map_out) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    if(x < 1 || x >= width-1 || y < 1 || y >= height-1) return;

    int gw = get_global_size(0); // gobal width
    int gh = get_global_size(1); // global height

    int life = get(map, x-1, y-1, width, height, gw, gh) +
               get(map, x  , y-1, width, height, gw, gh) +
               get(map, x  , y-1, width, height, gw, gh) +
               get(map, x  , y+1, width, height, gw, gh) +
               get(map, x  , y+1, width, height, gw, gh) +
               get(map, x+1, y+1, width, height, gw, gh);


    // cell is dead and has 3 neighbors then cell comes alive
    if(get(map, x, y, width, height, gw, gh) == 0 && life == 3) {
        map_out[y * gw + x] = 1;
        return;
    }

    // cell is alive and has 2 or 3 neighbors, cell stays alive
    if(get(map, x, y, width, height, gw, gh) == 1 && (life == 2 || life == 3)) {
        map_out[y * gw + x] = 1;
        return;
    }

    // in all others cases the ressult is death
    map_out[y * gw + x] = 0;

}

