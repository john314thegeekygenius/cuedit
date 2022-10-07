/*

	CU Edit - Menu Module

	Written by: Jonathan Clevenger

	10/3/2022
*/


#include "cuheads.h"

// Default function that does nothing
int CUMenuFNULL(){
	return 0;
};

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
	settings.init();
	CU::debugWrite("Initalized window at " + std::to_string(mx)+","+std::to_string(my));
};

void CUMenu::copySettings(CUSettings &set){
	settings.copy(set);
};

void CUMenu::addTab(CUMenu_t &tab){
	tab.open = false;

	tabs.push_back(tab);

	// Find the max size string
	int max = 0;
	for(int i = 0; i < tab.submenu.size(); i++){
		if(tab.submenu[i].title.length() > max){
			max = tab.submenu[i].title.length();
		}
	}
	tabs.back().subMenuWidth = max;
};

void CUMenu::selectTab(int tabid){
	tabSelected = tabid;
};

int CUMenu::getTab(){
	return tabSelected;
};

int CUMenu::numTabs(){
	return tabs.size();
};

bool CUMenu::tabOpen(int tab){
	if(tab >= 0 && tab < tabs.size()){
		return tabs[tab].open;
	}
	return false;
};

void CUMenu::closeTab(int tab){
	if(tab >= 0 && tab < tabs.size()){
		tabs[tab].open = false;
		tabs[tab].subselect = 0; // Un-needed?
	}
};

void CUMenu::openTab(int tab){
	if(tab >= 0 && tab < tabs.size()){
		tabs[tab].open = true;
		tabs[tab].subselect = 0;
	}	
};

void CUMenu::selectMenu(int tab, int sub){
	if(tab >= 0 && tab < tabs.size()){
		if(sub < 0) { sub = 0; }
		if(sub >= tabs[tab].submenu.size()) { sub = tabs[tab].submenu.size()-1; }
		tabs[tab].subselect = sub;
	}
};

int CUMenu::curSubMenu(int tab){
	if(tab >= 0 && tab < tabs.size()){
		return tabs[tab].subselect;
	}
	return 0; // ???
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

void CUMenu::drawSub(CU::Driver &videoDriver, int x, int y, CUMenu_t &tab, int subselect){
	// Draw the background
	videoDriver.drawBar(x,y,tab.subMenuWidth+4,tab.submenu.size()+2, ' ', settings.menu_fg_color, settings.menu_bg_color);
	// Draw a feild
	videoDriver.drawBox(x,y,tab.subMenuWidth+4,tab.submenu.size()+2, CU::BlockType::SINGLE, settings.menu_fg_color, settings.menu_bg_color);

	for(int i = 0; i < tab.submenu.size(); i++){
		std::string tabstr = "";
		if(subselect == i){
			tabstr += '[';
		}else{
			tabstr += ' ';
		}
		tabstr += tab.submenu[i].title;
		if(subselect == i){
			tabstr += ']';
		}else{
			tabstr += ' ';
		}
		videoDriver.writeStr(tabstr, x+1, y+1+i);
	}
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
//		videoDriver.drawBar(menuX+offsetX,menuY,tab.name.length()+2, 1, ' ', settings.menu_bar_fg_color, settings.menu_bar_bg_color);
		// Draw the tab
		bool tabS = false;
		if(tabSelected == tabCount){
			tabS = true;
		}
		// Draw the main tab
		drawTab(videoDriver, tab.name, menuX+offsetX , menuY, tabS);

		// Draw the sub menu (if open)
		if(tab.open){
			drawSub(videoDriver,menuX+offsetX,menuY+1, tab, tab.subselect);
		}
		// Offset the x
		offsetX += tab.name.length()+2;
		tabCount += 1;
	}
	
};
