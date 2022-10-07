/*

	CU Edit

	Written by: Jonathan Clevenger

	10/6/2022
*/
#pragma once

#include "cuheads.h"

namespace CU {

typedef enum FileMode {
    READ_ONLY = 0,
    READ_WRITE = 1,
};

typedef enum ErrorCode {
    NONE = 0,
    OPEN = 1,
    READ_ONLY = 2,
    READ = 3,
    LARGE = 4,
};

class File {

private:

    std::string name;// Name of the file
    FileMode mode;   // What mode is the file opened in
    int chunk;       // For opening the file in chunks
    bool fileLoaded; // Is the file fully read

	std::vector<std::string> clipboard;
	std::vector<std::string> history;
	std::vector<std::string> data;
public:
    File();
    ~File();
    ErrorCode open(std::string name);
    ErrorCode openChunks(std::string name);
    ErrorCode save(std::string name);
};

};
