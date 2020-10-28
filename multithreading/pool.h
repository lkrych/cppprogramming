#include <atomic>
#include <condition_variable>
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>

class thread_pool {
    std::atomic_bool done;
    std::mutex m;
    std::condition_variable cv;
    std::queue<std::function<void()> > work_queue;
    std::vector<std::thread> threads;
    
    void worker_thread() {
        std::thread::id t_id = std::this_thread::get_id();

        while (!done) {
            // std::cout << "WorkerThread " << t_id <<  " is about to check queue\n" << std::endl;
            std::function<void()> task;
            std::unique_lock<std::mutex> lk(m);
            cv.wait(lk, [this] {
                return done || work_queue.size() > 0;
            });
            if (done && work_queue.size() == 0) {
                return;
            }
            task = work_queue.front();
            work_queue.pop();
            lk.unlock();


            std::cout << "Thread " << t_id <<  " is doing some work\n" << std::endl;
            task();
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
                std::cout << "There was an error initializing the workqueue" << std::endl;
                done = true;
                throw;
            }
        }
        ~thread_pool() {
            std::cout << "Cleaning up the threads!\n" << std::endl;
            done = true;
            cv.notify_all();
            for (int i = 0; i < n_threads; i++) {
                if (threads[i].joinable()) {
                    threads[i].join();
                }
            }
        }
        template<typename FunctionType>
        void enqueue(FunctionType f) {
            std::unique_lock<std::mutex> lk(m);
            work_queue.push(std::function<void()>(f));
            lk.unlock();
            cv.notify_one();
        }
};