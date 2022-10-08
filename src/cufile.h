/*

	CU Edit

	Written by: Jonathan Clevenger

	10/6/2022
*/
#pragma once

#include "cuheads.h"

namespace CU {

enum class FileMode {
    READ_ONLY = 0,
    READ_WRITE = 1,
};

enum class ErrorCode {
    NONE = 0,
    OPEN = 1,
    READ_ONLY = 2,
    READ = 3,
    LARGE = 4,
};

class File {

private:

    std::string name;// Name of the file
    std::string path;// Where the file is located
    FileMode mode;   // What mode is the file opened in
    bool fileLoaded; // Is the file fully read

	std::vector<std::string> history;
	std::string data;
public:
    ErrorCode open(std::string path, FileMode mode = FileMode::READ_ONLY);
    ErrorCode save(std::string path);
    void getData(std::string & data_in);
    std::string getName();
    std::string getPath();

};

};
