/*

	CU Edit

	Written by: Jonathan Clevenger

	10/6/2022
*/
#pragma once

#include "cuheads.h"

namespace CU {

const int MAX_FILE_SIZE = 0x800000;

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
    WRITE = 5,
};

class File {

private:

    std::string name;// Name of the file
    std::string path;// Where the file is located
    std::string extention; // What is the file extention
    FileMode mode;   // What mode is the file opened in

	std::vector<std::string> history;
	std::vector<char> data;
public:
    bool fileLoaded; // Is the file fully read
    bool modified; // Is the file modified since last save
    bool hasBeenSaved; // Has the file been saved yet

public:
    ErrorCode open(std::string path, FileMode mode = FileMode::READ_ONLY);
    ErrorCode openNew(std::string fname, FileMode mode = FileMode::READ_ONLY);
    ErrorCode save(std::string path, FileMode mode = FileMode::READ_ONLY);
    std::vector<char> &getData();
    std::string getName();
    std::string getPath();
    std::string getExt();
};

};
