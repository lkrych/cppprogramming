#include <atomic>
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>

class thread_pool {
    std::atomic_bool done;
    std::mutex m;
    std::queue<std::function<void()> > work_queue;
    std::vector<std::thread> threads;
    
    void worker_thread() {
        std::thread::id t_id = std::this_thread::get_id();
        // std::cout << "WorkerThread " << t_id <<  " is about to check done " << done << "\n" << std::endl;

        while (!done) {
            // std::cout << "WorkerThread " << t_id <<  " is about to check queue\n" << std::endl;
            std::function<void()> task;
            if (work_queue.size() > 0) {
                std::unique_lock<std::mutex> lk(m);
                task = work_queue.front();
                work_queue.pop();
                lk.unlock();


                std::cout << "Thread " << t_id <<  " is doing some work\n" << std::endl;
                task();
            } else {
                std::cout << "WorkerThread " << t_id <<  " is yielding\n" << std::endl;
                std::this_thread::yield();
            }
        }
        // std::cout << "WorkerThread " << t_id <<  " is done \n" << std::endl;
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
            for(int i = 0; i < n_threads; i++) {

                threads[i].join();
            }
        }
        template<typename FunctionType>
        void enqueue(FunctionType f) {
            work_queue.push(std::function<void()>(f));
        }
};