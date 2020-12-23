#include <cpr/cpr.h>
#include <iostream>

// g++ -std=c++11 -o http -lcpr http.cc

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                      cpr::Authentication{"user", "pass"},
                      cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    std::cout << r.status_code << " | " << r.header["content-type"] << " | " << r.text << std::endl;
}
