/*

	CU Edit

	Written by: Jonathan Clevenger

	10/3/2022
*/
#pragma once

class CUEditor {
private:
	CU::Driver videoDriver;
	CUSettings settings;
	bool running = false;
	
	std::vector<CUMenu> menuList;
public:
	CUEditor();
	void loadSettings();
	void halt(int e);
	void run();
	void drawGUI();
	void shutdown();
	void debugWrite(std::string s, CU::DebugMsgType msgType = CU::DebugMsgType::INFO);

};


