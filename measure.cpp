#include <iostream>
#include <chrono>
#include <cstdlib>

void test_cache_size(size_t round, size_t array_size, uint8_t* arr) {
    const int step = 32;
    register int j = 0; 
    for (register int i = 0; i < round; ++i) {
        arr[j] = 77;
        j = (j + step) % array_size;
    }
}

void test_cache_line_size(size_t round, size_t step, size_t array_size, uint8_t* arr) {
    register int j = 0; 
    for (register int i = 0; i < round; ++i) {
        arr[j] = 77;
        j = (j + step) % array_size;
    }
}

int main(int argc, char** argv) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(3, &mask); 
    if (sched_setaffinity(0, sizeof(mask), &mask) < 0) { 
        perror("sched_setaffinity"); 
    }
    // srand(time(NULL));
    // printf("%d\n", argc);
    size_t round = atoi(argv[1]);
    // size_t array_size = atoi(argv[2]) * 1024;
    int array_size = 2048 * 1024;
    size_t step = atoi(argv[2]);
    uint8_t* arr = new uint8_t[array_size];
    auto begin = std::chrono::high_resolution_clock::now();
    // test_cache_size(round, array_size, arr);
    test_cache_line_size(round, step, array_size, arr);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin).count() * 1000;
    std::cout << "step size " << step << ", round num " << round;
    std::cout << ", time: " << duration << "ms" << std::endl;
    return 0;
}