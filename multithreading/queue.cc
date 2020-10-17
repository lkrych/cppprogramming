#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex m;
std::condition_variable cv;
std::queue<int> q;

// workers consume from the queue and sum up the number they consume
int worker_thread(int n, int *sum) {
    const std::lock_guard<std::mutex> lock(m);
    int r = q.front();
    q.pop();
    std::cout << "Thread " << n << " is adding " << r << " to it's sum." << std::endl;
    *sum += r;
}

// producers add n random numbers to the queue
void producer_thread(int nums) {
    const std::lock_guard<std::mutex> lock(m);
    for (int i = 0; i < nums; i++) {
        int r = rand() % 100;
        q.push(r);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << " Usage ./queue 5 10 (where 5 is number of worker threads and 10 refers to the number of items to put on the queue)  \n";
        exit(1);
    }
    int n_threads = std::stoi(argv[1]);
    int n_items = std::stoi(argv[2]);

    std::thread t[n_threads];
    int sum[n_threads];

    //create producer
    std::thread p(producer_thread, n_items);
    
    //create workers
    for (int i = 0; i < n_threads; i++) {
        t[i] = std::thread(worker_thread, i, sum[i]);
    }

    //Join the threads with the main thread
    p.join();
    for (int i = 0; i < n_threads; ++i) {
        t[i].join();
    }
    
}