#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./create_input_files 3 (number of files) 10 (size in KB)" << std::endl;
    }
    int n_files = atoi(argv[1]);
    int file_size = atoi(argv[2]);

    std::string file_name = "input";
    std::string file_suffix = ".txt";
    std::vector<char> one_kb(1024, 0);
    std::string s;
	for (char c: one_kb) {
		s.push_back(c);
    }

    for (int i = 0; i < n_files; i++) {
        std::string current_file = file_name;
        current_file.append(std::to_string(i)).append(file_suffix);
        std::ofstream myfile;
        myfile.open(current_file);
        for (int j = 0; j < file_size; j++) {
            myfile.write(s.c_str(), s.size());
        }
        myfile.close();
    }
}