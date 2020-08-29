__kernel void test(__global int* data){
    int id = get_global_id(0);
    data[id] += 69;
}