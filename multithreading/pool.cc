#include <atomic>
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>

#include "pool.h"

class thread_pool {
    std::atomic_bool done;
    std::mutex m;
    std::queue<std::function<void()> > work_queue;
    std::vector<std::thread> threads;
    
    void worker_thread() {
        while (!done) {
            std::function<void()> task;
            if (work_queue.size() > 0) {
                std::unique_lock<std::mutex> lk(m);
                task = work_queue.front();
                work_queue.pop();
                lk.unlock();

                task();
            } else {
                std::this_thread::yield();
            }
        }
    }
    public:
        int n_threads = 0;
        thread_pool(int n_threads): done(false) {
            n_threads = n_threads;
            try {
                for(int i  = 0; i < n_threads; i++) {
                    threads.push_back(
                        std::thread(&thread_pool::worker_thread, this)
                    );
                }
            } catch(...) {
                done = true;
                throw;
            }
        }
        ~thread_pool() {
            done=true;
            for(int i = 0; i < n_threads; i++) {
                threads[i].join();
            }
        }
        template<typename FunctionType>
        void enqueue(FunctionType f) {
            work_queue.push(std::function<void()>(f));
        }
};