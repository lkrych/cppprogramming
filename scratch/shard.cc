#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <sys/stat.h>

struct mr_spec {
    std::vector<std::string> input_files;
    int map_kilobytes;
};

struct FileData {
     std::string filename;
     std::string buffer;
};

struct FileShard {
     std::vector<FileData> files;
     int bytes_written;
};

float file_size(std::string filename) {
     const char *cstr = filename.c_str();
     struct stat st;
     stat(cstr, &st);
     std::cout << "Stat size of the input file is " << st.st_size << " kb" << std::endl;
     return st.st_size;
}

std::vector<FileShard> createFileShards(const mr_spec& mr_spec, int num_shards) {
    // initialize shards
    std::vector<FileShard> shards(num_shards);
    for(int i = 0; i < num_shards; i++) {
        struct FileShard fs = shards[i];
        fs.files = std::vector<FileData>();
        fs.bytes_written = 0;
    }
    // variables for housekeeping
    int current_shard_idx = 0;
    int current_file_data_idx = 0;
    struct FileShard fs;

    //iterate through all the files
    for(int i = 0; i < mr_spec.input_files.size(); i++) {
        std::string filename = mr_spec.input_files[i];
        std::ifstream file(filename);

        // create fd for fs
        fs = shards[current_shard_idx];
        struct FileData fd;
        fd.filename = filename;
        fs.files.push_back(fd);

        //write the entire file out to FileShards
        if (file.is_open()) {
            std::string line;
            // read the file line by line
            while (std::getline(file, line)) {
                int byte_size = line.length() * sizeof(char);

                // check if we need a new shard
                if (fs.bytes_written + byte_size > mr_spec.map_kilobytes * 1000) {
                    //we are done writing to the current shard and need to transition to a new one
                    current_shard_idx += 1;
                    current_file_data_idx = 0;
                }
                //write to current shard
                fs = shards[current_shard_idx];
                fd = fs.files[current_file_data_idx];
                fd.buffer.append(line);
                fs.bytes_written += byte_size;
            }
            file.close();
            current_file_data_idx += 1;
        }

    }

    return shards;
}

// g++ shard.cc -o shard -std=c++11

int main() {

    std::vector<std::string> input_files = { "input0.txt", "input1.txt", "input2.txt" };
    struct mr_spec mr;
    mr.input_files = input_files;
    mr.map_kilobytes = 2;

    int num_shards = 0;
    float total_file_size = 0;
    // loop through the input files and see how big they are
    for (int i = 0; i < mr.input_files.size(); i++) {
        float fs = file_size(mr.input_files[i]);
        std::cout << "Size of the input file " << mr.input_files[i] << " is " << fs << " kb" << std::endl;
        total_file_size += fs;
    }
    std::cout << "Total file size of the input files is " << total_file_size << " kb" << std::endl;
    // calculate number of shards
    num_shards = ceil((total_file_size/1000.0 / mr.map_kilobytes));

    std::cout << "The number of file shards should be " << num_shards << std::endl;

    std::vector<FileShard> shards = createFileShards(mr, num_shards);

    for (int i = 0; i < shards.size(); i++) {
        struct FileShard fs = shards[i];
        std::cout << "Shard " << i << " is " << fs.bytes_written << " bytes" << std::endl;
        for (int j = 0; j < fs.files.size(); j++) {
            struct FileData fd = fs.files[i];
            std::cout << "Shard " << i << " has fileData for " << fd.filename << std::endl;
        }
    }

}