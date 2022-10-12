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

	name = CU::filenameString(o_path);

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
		modified = false;
		hasBeenSaved = true;
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
	// Nothing has been changed since the last save
	modified = false;
	// We just saved the file
	hasBeenSaved = false;

	return CU::ErrorCode::NONE;
};

CU::ErrorCode CU::File::save(std::string s_path, FileMode o_mode){
	// Open the file
	std::ofstream f_stream(s_path);
	if(f_stream.is_open()){
		if(data.size()){
			f_stream.write(data.data(),data.size());
			if(f_stream.bad()||f_stream.fail()){
				f_stream.close();
				return CU::ErrorCode::WRITE;
			}
		}
		f_stream.close();
		// Reset the file path
		path = s_path;
		// Rename the file
		name = CU::filenameString(s_path);
		// Nothing has been changed since the last save
		modified = false;
		// We just saved the file
		hasBeenSaved = true;

		CU::debugWrite("Saved file to: "+s_path);

		// TODO:
		// Make this save a history file like vim?

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

