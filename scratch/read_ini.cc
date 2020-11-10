#include <string>
#include <iostream>
#include <fstream>
#include <vector>

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

			if (key == "worker_ipaddr_ports") {
				//split val into individual hosts, put them into a vector
				std::vector<std::string> worker_ipaddr_ports;
				
				//https://stackoverflow.com/a/14266139/4458404
				std::string delimiter = ",";
				size_t pos = 0;
				std::string token;
				std::cout << "worker_ipaddr_ports" << std::endl;
				while ((pos = val.find(delimiter)) != std::string::npos) {
					token = val.substr(0, pos);
					std::cout << token << std::endl;
					worker_ipaddr_ports.push_back(token);
					val.erase(0, pos + delimiter.length());
				}
				std::cout << val << std::endl;
				worker_ipaddr_ports.push_back(val);
			} else if (key == "input_files") {
				//split val into individual files, put them into a vector
				std::vector<std::string> input_files;

				//https://stackoverflow.com/a/14266139/4458404
				std::string delimiter = ",";
				size_t pos = 0;
				std::string token;
				std::cout << "input_files" << std::endl;
				while ((pos = val.find(delimiter)) != std::string::npos) {
					token = val.substr(0, pos);
					std::cout << token << std::endl;
					input_files.push_back(token);
					val.erase(0, pos + delimiter.length());
				}
				std::cout << val << std::endl;
				input_files.push_back(val);
			}
		}
		config_file.close();
	} else {
		std::cout << "Unable to open file";
		exit(1);
	}
    return 0;
}