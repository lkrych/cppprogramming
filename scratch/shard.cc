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
     int offset_begin;
     int offset_end;
};

struct FileShard {
     std::vector<FileData> files;
     int shard_size;
};

float file_size(std::string filename) {
     const char *cstr = filename.c_str();
     struct stat st;
     stat(cstr, &st);
     return st.st_size;
}

struct FileShard initShard(std::string filename, int line_no) {
    struct FileShard fs;
    struct FileData fd;
    fs.files = std::vector<FileData>();
    fs.shard_size = 0;
    fd.filename = filename;
    fd.offset_begin = line_no;
    fs.files.push_back(fd);
    return fs;
}

struct FileShard initFileData(struct FileShard fs, std::string filename) {
    struct FileData fd;
    fd.filename = filename;
    fs.files.push_back(fd);
    return fs;
}

void registerEndOffset(struct FileShard *fs, int fd_index, int line_no) {
    struct FileData fd;
    fd = fs->files[fd_index];
    fd.offset_end = line_no;
    fs->files[fd_index] = fd;
}

void registerBytesWritten(struct FileShard *fs, int fd_index, int byte_size) {
    struct FileData fd;
    fd = fs->files[fd_index];
    fs->shard_size += byte_size;
    fs->files[fd_index] = fd;
}

std::vector<FileShard> createFileShards(const mr_spec& mr_spec) {
    std::vector<FileShard> shards;
    // variables for housekeeping
    int current_shard_idx = 0;
    int current_file_data_idx = 0;
    struct FileShard fs;
    struct FileData fd;

    //iterate through all the files
    for(int i = 0; i < mr_spec.input_files.size(); i++) {
        std::string filename = mr_spec.input_files[i];
        std::ifstream file(filename);
        int line_no = 0;
        // create fd for fs
        if (int i = 0) {
            // we only need to init the shard on the first go around
            // every other time it will be initialized when the byte limit is reached
            fs = initShard(filename, line_no);
        } else {
            // when we start reading a new file, create a new FileData structure
            fs = initFileData(fs, filename);
        }

        //write the entire file out to FileShards
        if (file.is_open()) {
            std::string line;
            // read the file line by line
            while (std::getline(file, line)) {
                int byte_size = line.length() * sizeof(char); //include null terminator
                // check if we need a new shard
                if (fs.shard_size + byte_size > mr_spec.map_kilobytes * 1024) {
                    // std::cout << "move onto next shard" << std::endl;
                    //we are done writing to the current shard and need to transition to a new one
                    registerEndOffset(&fs, current_file_data_idx, line_no);
                    shards.push_back(fs);
                    fs = initShard(filename, line_no);
                    current_file_data_idx = 0;
                }
                line_no++;
                //keep track of how much has been writtenn to shard
                registerBytesWritten(&fs, current_file_data_idx, byte_size);
            }
            file.close();
            registerEndOffset(&fs, current_file_data_idx, line_no);
            current_file_data_idx += 1;
        }
    }
    shards.push_back(fs);
    return shards;
}

// g++ shard.cc -o shard -std=c++11

int main() {

    std::vector<std::string> input_files = { "input0.txt", "input1.txt", "input2.txt" };
    struct mr_spec mr;
    mr.input_files = input_files;
    mr.map_kilobytes = 3;

    int num_shards = 0;
    float total_file_size = 0;
    // loop through the input files and see how big they are
    for (int i = 0; i < mr.input_files.size(); i++) {
        float fs = file_size(mr.input_files[i]);
        total_file_size += fs;
    }

    std::cout << "Total data size of the input files is " << total_file_size << " kb" << std::endl;

    std::vector<FileShard> shards = createFileShards(mr);

    std::cout << "Total shards created " << shards.size() << " for divisions of "<< mr.map_kilobytes << " kb" << std::endl;
    int sum_of_data = 0;
    for (int i = 0; i < shards.size(); i++) {
        struct FileShard fs = shards[i];
        std::cout << "Shard " << i << " is " << fs.shard_size << " bytes" << std::endl;
        sum_of_data += fs.shard_size;
        for (int j = 0; j < fs.files.size(); j++) {
            struct FileData fd = fs.files[j];
            std::cout << "Shard " << i << " has fileData for " << fd.filename  << " it starts at offset " << fd.offset_begin << " and ends at " << fd.offset_end << std::endl;
        }
    }

    std::cout << "Total data size of the shards is " << sum_of_data << " kb" << std::endl;

}