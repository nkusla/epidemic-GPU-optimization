#include "struct.cl"

__kernel void MakeIeractions(__global int* locations, __global int* width) {
    int id = get_global_id(0);

    int w = *width;
    for(int i = 0; i < w; ++i){
        data[id*w+i] = 69;
    }
}
