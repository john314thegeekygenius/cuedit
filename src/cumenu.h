/*

	CU Edit

	Written by: Jonathan Clevenger

	10/3/2022
*/
#pragma once

#include "cuheads.h"

typedef int (*CUMenuFunction)();

extern int CUMenuFNULL();

typedef struct CUSubMenu_t {
	std::string title;
	CUMenuFunction callback = nullptr;
}CUSubMenu_t;

typedef struct CUMenu_t{
	std::string name;
	std::vector<CUSubMenu_t> submenu;
	int subMenuWidth;

	bool open;
	int subselect = 0;
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
	int getTab();
	int numTabs();

	bool tabOpen(int tab);
	void closeTab(int tab);
	void openTab(int tab);

	void selectMenu(int tab, int sub);
	int curSubMenu(int tab);
	int runSubMenu(int tab, int sub);

	void drawSub(CU::Driver &videoDriver, int x, int y, CUMenu_t &tab, int subselect);
	void drawTab(CU::Driver &videoDriver, std::string name, int x,int y, bool selected);
	void draw(CU::Driver &videoDriver);
};

