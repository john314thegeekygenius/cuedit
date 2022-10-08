/*

	CU Edit Driver

	Written by: Jonathan Clevenger

	10/3/2022

*/


#include "cuheads.h"

CU::ErrorCode CU::File::open(std::string o_path, CU::FileMode o_mode){
	path = o_path;

	if(o_path == ""){
		name = "Untitled";
		// It's a new file, so no history
		history.clear();
		// Clear any data
		data.clear();
		fileLoaded = true;
	}

	// Find the filename
	int e = 0;
	for(int i = path.length()-1; i >= 0; i --){
		if(path.at(i)=='/'){ e = i; break; }
	}
	name = "";
	for(int i = e; i < path.length(); i ++){
		name.push_back(path.at(i));
	}
    mode = o_mode;
    fileLoaded = false;

	// TODO:
	// Make this load a history file like vim?
	history.clear(); // We don't remember anything about it!
	
	// Clear any data
	data.clear();

	// Load the data
	std::ifstream f_stream(path);
	if(f_stream.is_open()){
		// Read the data
		char buffer[0x80000];
		while(f_stream.getline(buffer,0x80000) && strlen(buffer) > 0){
			data += (char*)buffer;
		}
		fileLoaded = true;
	}else{
		return CU::ErrorCode::OPEN;
	}
	return CU::ErrorCode::NONE;
};

CU::ErrorCode CU::File::save(std::string s_path){
	return CU::ErrorCode::NONE;
};

void CU::File::getData(std::string & data_in){
	data_in = this->data;
};

std::string CU::File::getName(){
	return name;
};

std::string CU::File::getPath(){
	return path;
};

