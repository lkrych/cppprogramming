#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional> //std::ref

std::mutex m;
std::condition_variable cv;
std::queue<int> q;
bool lock_free = false;

// workers consume from the queue and sum up the number they consume
void worker_thread(int n, int& sum) {
    // spin until the queue is empty
    while (q.size() > 0 || !lock_free) {
        //critical section    
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{return lock_free;});

        lock_free = false;
        if (q.size() == 0) {
            break;
        }

        int r = q.front();
        q.pop();
        std::cout << "Thread " << n << " is adding " << r << " to it's sum." << std::endl;
        sum[n] += r;

        lk.unlock();
        lock_free = true;
        cv.notify_all();
    }
    std::cout << "Thread " << n << " exited with sum " << sum << ".\n";
}

// producers add n random numbers to the queue
void producer_thread(int nums) {
    std::unique_lock<std::mutex> lk(m);
    for (int i = 0; i < nums; i++) {
        int r = rand() % 100;
        std::cout << "Producer Thread putting " << r << " on queue.\n";
        q.push(r);
    }
    lk.unlock();
    lock_free = true;
    cv.notify_all();
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << " Usage ./queue 5 10 (where 5 is number of worker threads and 10 refers to the number of items to put on the queue)  \n";
        exit(1);
    }

    unsigned long const hardware_threads = std::thread::hardware_concurrency();

    std::cout << "Number of supported hardware threads: " << hardware_threads << "\n";

    int n_threads = std::stoi(argv[1]);
    int n_items = std::stoi(argv[2]);

    std::vector<std::thread> threads;
    int sum[n_threads];

    //create producer
    std::thread p(producer_thread, n_items);
    
    // create workers
    for (int i = 0; i < n_threads; i++) {
        threads.push_back(std::thread(worker_thread, i, std::ref(sum)));
    }

    //Join the threads with the main thread
    p.join();
    std::for_each(threads.begin(), threads.end(),
        std::mem_fn(&std::thread::join)); 
    
}