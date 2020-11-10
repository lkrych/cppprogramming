#include <string>
#include <iostream>
#include <fstream>

std::string config_filename = "config.ini";

int main() {
    std::cout << "Reading file: " << config_filename << std::endl;
    std::string line;
	std::string delimiter = "=";
	std::ifstream config_file(config_filename);
	if (config_file.is_open()) {
		while(getline(config_file,line)){
			std::string key = line.substr(0, line.find(delimiter));
			std::string val = line.substr(line.find(delimiter) + 1, line.length());
			std::cout << "key: " << key << "   value: " << val << std::endl;
		}
		config_file.close();
	} else {
		std::cout << "Unable to open file";
		exit(1);
	}
    return 0;
}