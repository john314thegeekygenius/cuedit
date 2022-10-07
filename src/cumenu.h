/*

	CU Edit

	Written by: Jonathan Clevenger

	10/3/2022
*/
#pragma once

#include "cuheads.h"

typedef struct CUMenu_t{
	std::string name;
	std::vector<std::string> submenu;
}CUMenu_t;

class CUMenu {
private:
	bool canResize;
	bool canMinimize;
	bool canClose;
	std::vector<CUMenu_t> tabs;
	
	int menuX = 0;
	int menuY = 0;
	int menuWidth = 0;
	int menuHeight = 0;

	CUSettings settings;
	
	int tabSelected = -1;
public:
	CUMenu();
	CUMenu(int mx, int my, int w, int h, bool canResize, bool canMinimize, bool canClose);
	void init(int mx, int my, int w, int h, bool canResize, bool canMinimize, bool canClose);
	void addTab(CUMenu_t &tab);
	void copySettings(CUSettings &set);
	void selectTab(int tabid);
	void drawTab(CU::Driver &videoDriver, std::string name, int x,int y, bool selected);
	void draw(CU::Driver &videoDriver);
};

