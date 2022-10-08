/*

	CU Edit

	Written by: Jonathan Clevenger

	10/6/2022
*/
#pragma once

#include "cuheads.h"

namespace CU {

class Project {
private:
    std::string name;
    std::string path;
    std::vector<CU::File> fileList;
public:
    void open(std::string name);
    void save(std::string name);
    void close();

    std::string getName();
};

};

