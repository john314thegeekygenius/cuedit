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
	bool MainMenuTabsSelected = false;
	
	std::vector<CUMenu> menuList;
	std::vector<CU::File> fileList;

	CU::Project project;

public:
	void init();
	void loadSettings();
	void halt(int e);
	void run();
	void close();
	void handleInt();
	void drawGUI();
	void shutdown();

	std::string openFile();

};


