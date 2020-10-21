#include <iostream>
#include <thread>

void hello() {
    std::cout << "Hello Concurrent World from spawned thread\n";
}

int main() {
    std::thread t(hello);
    std::cout << "Hello Concurrent World from main thread\n";
    t.join();
}