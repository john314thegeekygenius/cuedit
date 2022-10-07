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
public:
	void init();
	void loadSettings();
	void halt(int e);
	void run();
	void drawGUI();
	void shutdown();

};


