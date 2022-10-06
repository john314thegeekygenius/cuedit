/*

	CU Edit - Menu Module

	Written by: Jonathan Clevenger

	10/3/2022
*/


#include "cuheads.h"

CUMenu::CUMenu(){
	menuX = menuY = menuWidth = menuHeight = 0;
};

CUMenu::CUMenu(int mx, int my, int w, int h, bool canResize, bool canMinimize, bool canClose) {
	init(mx,my,w,h,canResize, canMinimize, canClose);
};

void CUMenu::init(int mx, int my, int w, int h, bool resize, bool minimize, bool close) {
	menuX = mx;
	menuY = my;
	menuWidth = w;
	menuHeight = h;
	canResize = resize;
	canMinimize = minimize;
	canClose = close;
	tabSelected = -1;
};

void CUMenu::copySettings(CUSettings &set){
	settings.copy(set);
};

void CUMenu::addTab(CUMenu_t &tab){
	tabs.push_back(tab);
};

void CUMenu::drawTab(CU::Driver &videoDriver, std::string name, int x,int y, bool selected){
	if(selected){
		name = name.insert(0,1,'<');
		name.push_back('>');
	}else{
		name = name.insert(0,1,'[');
		name.push_back(']');
	}
	videoDriver.writeStr(name, x, y);
};


void CUMenu::draw(CU::Driver &videoDriver){
	// Draw the background
	videoDriver.drawBar(menuX,menuY,menuWidth,menuHeight, ' ', settings.menu_fg_color, settings.menu_bg_color);
	// Draw a feild
	videoDriver.drawBox(menuX,menuY,menuWidth,menuHeight, CU::BlockType::SINGLE, settings.menu_fg_color, settings.menu_bg_color);
	// Draw a title bar
	videoDriver.drawBar(menuX,menuY,menuWidth, 1, ' ', settings.menu_bar_fg_color, settings.menu_bar_bg_color);

	// Draw every tab
	int offsetX = 0;
	int tabCount = 0;
	for(CUMenu_t &tab : tabs){
		// Draw a color behind the selected tab	
		videoDriver.drawBar(menuX+offsetX,menuY,tab.name.length()+2, 1, ' ', settings.menu_bar_fg_color, settings.menu_bar_bg_color);
		// Draw the tab
		bool tabS = false;
		if(tabSelected == tabCount){
			tabS = true;
		}
		drawTab(videoDriver, tab.name, menuX+offsetX , menuY, tabS);
		// Offset the x
		offsetX += tab.name.length()+2;
		tabCount += 1;
	}
	
};
