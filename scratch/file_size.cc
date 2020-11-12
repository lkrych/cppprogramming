#include <sys/stat.h>
#include <iostream>
#include <math.h>

float file_size(std::string filename) {
     const char *cstr = filename.c_str();
     struct stat st;
     stat(cstr, &st);
     return ceil(st.st_size/(float)1000);
}

int main() {
    int size = file_size("read_ini.cc");
    std::cout << "file size of read_ini.cc is " << size  << std::endl;
    std::cout << "hypothetical test = 55/16 -> " << ceil((float)55/16) << std::endl;
}
