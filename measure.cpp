#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cmath>

const int CACHE_SIZE = 32 * 1024;

void test_cache_size(size_t round, size_t array_size, uint8_t* arr, int index_mask) {
    const int step = 7529;
    register int j = 0; 
    for (register int i = 0; i < round; ++i) {
        arr[j] = 77;
        j = (j + step) & index_mask; // %
    }
}

void test_cache_line_size(size_t round, size_t step, size_t array_size, uint8_t* arr, int index_mask) {
    register int j = 0; 
    for (register int i = 0; i < round; ++i) {
        arr[j] = 77;
        j = (j + step) & index_mask;
    }
}

void test_associative(size_t round, size_t step, size_t array_size, uint8_t* arr, int index_mask) {
    register int j = step >> 1; 
    for (register int i = 0; i < round; ++i) {
        arr[j] = 77;
        j = (j + step) & index_mask;
    }
}

int main(int argc, char** argv) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(3, &mask); 
    if (sched_setaffinity(0, sizeof(mask), &mask) < 0) { 
        perror("sched_setaffinity"); 
    }
    size_t round = atoi(argv[1]);
    // size_t array_size = atoi(argv[2]) * 1024; // for measuring cache size
    int array_size = 2 * CACHE_SIZE;
    int index_mask = 0;
    // size_t step = atoi(argv[2]); // for measuring line size
    size_t n = atoi(argv[2]);
    for (int i = 0; i < 32; ++i) {
        if (!(array_size & (1 << i))) {
            index_mask |= 1 << i;
        } else {
            break;
        }
    } // avoid mod
    uint8_t* arr = new uint8_t[array_size];
    int step = 2 * array_size / pow(2, n); 
    auto begin = std::chrono::high_resolution_clock::now();
    // test_cache_size(round, array_size, arr, index_mask);
    // test_cache_line_size(round, step, array_size, arr, index_mask);
    test_associative(round, step, array_size, arr, index_mask);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin).count() * 1000;
    std::cout << "n " << n << ", round num " << round;
    std::cout << ", time: " << duration << "ms" << std::endl;
    return 0;
}