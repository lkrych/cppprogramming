#include <iostream>
#include <vector>
#include <chrono>
#include <future>

#include "pool.h"

// g++ -std=c++11 test_pool.cc -o test_pool

int main()
{
    
    thread_pool pool(2);

    for(int i = 0; i < 4; ++i) {
        pool.enqueue([i] {
            std::cout << "hello " << i << " world" << std::endl;
        });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return 0;
}