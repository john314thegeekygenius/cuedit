/*

	CU Edit Driver

	Written by: Jonathan Clevenger

	10/3/2022

*/


#include "cuheads.h"

CU::ErrorCode CU::File::open(std::string o_path, CU::FileMode o_mode){
	path = o_path;

	if(o_path == ""){
		return openNew("Untitled");
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

		// Get the file length
		f_stream.seekg (0, f_stream.end);
		int length = f_stream.tellg();

		if(length > CU::MAX_FILE_SIZE){
			f_stream.close();
			return CU::ErrorCode::LARGE;
		}

		f_stream.seekg (0, f_stream.beg);
		char * buffer = new char [length];

		f_stream.read(buffer,length);
		if(f_stream.bad()||f_stream.fail()){
			f_stream.close();
			return CU::ErrorCode::READ;
		}
		data.assign(buffer, buffer+length);

		for( int i = data.size()-1; i >= 0; i--){
			if(data[i] == 13 && data[i+1] == 10){ 
				data.erase(data.begin()+i);
			}
		}
		f_stream.close();
		
		delete[] buffer;

		fileLoaded = true;
	}else{
		return CU::ErrorCode::OPEN;
	}
	return CU::ErrorCode::NONE;
};

CU::ErrorCode CU::File::openNew(std::string fname, CU::FileMode o_mode){
	path = "";
	name = fname;
    mode = o_mode;
	// It's a new file, so no history
	history.clear();
	// Clear any data
	data.clear();
	fileLoaded = true;
	return CU::ErrorCode::NONE;
};

CU::ErrorCode CU::File::save(std::string s_path, FileMode o_mode){
	// Open the file
	std::ofstream f_stream(s_path);
	if(f_stream.is_open()){
		if(data.size()){
			f_stream.write(data.data(),data.size());
			CU::debugWrite(std::string(data.data()));
			if(f_stream.bad()||f_stream.fail()){
				f_stream.close();
				return CU::ErrorCode::WRITE;
			}
		}
		f_stream.close();
		return CU::ErrorCode::NONE;
	}else{
		return CU::ErrorCode::OPEN;
	}

	return CU::ErrorCode::NONE;
};

std::vector<char> &CU::File::getData(){
	return data;
};

std::string CU::File::getName(){
	return name;
};

std::string CU::File::getPath(){
	return path;
};

