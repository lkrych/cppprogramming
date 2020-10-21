#include <thread>

void do_something();
void do_something_else();

class background_task {
    public:
        void operator() () const {
            do_something();
            do_something_else();
        }
};
background_task f;
std::thread my_thread(f);
