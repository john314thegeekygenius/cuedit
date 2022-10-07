/*

	CU-Edit - A text based C++ IDE

	Written by: Jonathan Clevenger

	Started: 10/3/2022

	Change log:
	*
	*
*/

#include "cuheads.h"

CUEditor editor;

void breakHandle(int e){
	editor.halt(e);
	CU::debugWrite("int "+std::to_string(e));
};

int main(int argc, char *argv[]){
	CU::openDebugFile();
	editor.init();
	editor.run();
	editor.shutdown();
	CU::closeDebugFile();
	return 0;
};

CUMenu mainMenu;

int MMS_FOpen(){
	return 0;
};

std::vector<CUSubMenu_t> MM_LFile = {
	{"Open", &MMS_FOpen}, 
	{"Save", &CUMenuFNULL}, 
	{"Save As", &CUMenuFNULL}, 
	{"Reload", &CUMenuFNULL}, 
	{"Close", &CUMenuFNULL}, 
	{"Exit", &CUMenuFNULL},
};

CUMenu_t MM_Sub_File = {
	"File", MM_LFile
};

std::vector<CUSubMenu_t> MM_LEdit = {
	{"Undo", &MMS_FOpen}, 
	{"Redo", &CUMenuFNULL}, 
	{"Copy", &CUMenuFNULL}, 
	{"Cut", &CUMenuFNULL}, 
	{"Paste", &CUMenuFNULL}, 
};

CUMenu_t MM_Sub_Edit = {
	"Edit", MM_LEdit
};

CUMenu_t MM_Sub_Settings = {
	"Settings", {},
};

std::vector<CUSubMenu_t> MM_LProject = {
	{"Open", &MMS_FOpen}, 
	{"Save", &CUMenuFNULL}, 
	{"Save As", &CUMenuFNULL}, 
	{"Reload", &CUMenuFNULL}, 
	{"Close", &CUMenuFNULL}, 
	{"Settings", &CUMenuFNULL}, 
};

CUMenu_t MM_Sub_Project = {
	"Project", MM_LProject
};


void CUEditor::init(){
	running = true;

	// Setup the break handler
	videoDriver.setupHandle(&breakHandle);
	videoDriver.setFPS(30);

	loadSettings();
	
	menuList.clear(); // Remove any windows
	
	// Initalize the Main Menu
	mainMenu.init(0, 1, videoDriver.getWidth(),videoDriver.getHeight()-1, false, false, false);
	// Copy the settings
	mainMenu.copySettings(settings);
	// Add the sub menus
	mainMenu.addTab(MM_Sub_File);
	mainMenu.addTab(MM_Sub_Edit);
	mainMenu.addTab(MM_Sub_Settings);
	mainMenu.addTab(MM_Sub_Project);
};

void CUEditor::run(){
	int cx = 0;
	int cy = 0;
	while(running){
		
		drawGUI();

		CU::keyCode key = videoDriver.getkey();

		if(key == CU::keyCode::k_escape){
			// TODO:
			// Check for cases where this would not work
			MainMenuTabsSelected = !MainMenuTabsSelected;
			if(MainMenuTabsSelected){
				mainMenu.selectTab(0);
			}else{
				mainMenu.closeTab(mainMenu.getTab());
				mainMenu.selectTab(-1);
			}
		}
		if(key == CU::keyCode::s_left){
			if(MainMenuTabsSelected){
				mainMenu.closeTab(mainMenu.getTab());
				int cur = mainMenu.getTab() - 1;
				if(cur < 0) { cur = 0; }
				mainMenu.selectTab(cur);
			}
		}
		if(key == CU::keyCode::s_right){
			if(MainMenuTabsSelected){
				mainMenu.closeTab(mainMenu.getTab());
				int cur = mainMenu.getTab() + 1;
				if(cur >= mainMenu.numTabs()) { cur = mainMenu.numTabs()-1; }
				mainMenu.selectTab(cur);
			}
		}
		if(key == CU::keyCode::k_tab){
			if(MainMenuTabsSelected){
				mainMenu.closeTab(mainMenu.getTab());
				int cur = mainMenu.getTab() + 1;
				if(cur >= mainMenu.numTabs()) { cur = 0; }
				mainMenu.selectTab(cur);
			}
		}
		if(key == CU::keyCode::k_enter){
			if(MainMenuTabsSelected){
				if(mainMenu.tabOpen(mainMenu.getTab())){
					mainMenu.closeTab(mainMenu.getTab());
					// Run the sub function for that menu
				}else{
					mainMenu.openTab(mainMenu.getTab());
				}
			}
		}
		if(key == CU::keyCode::s_up){
			if(MainMenuTabsSelected){
				if(mainMenu.tabOpen(mainMenu.getTab())){
					mainMenu.selectMenu(mainMenu.getTab(), mainMenu.curSubMenu(mainMenu.getTab())-1);
				}
			}
		}
		if(key == CU::keyCode::s_down){
			if(MainMenuTabsSelected){
				if(mainMenu.tabOpen(mainMenu.getTab())){
					mainMenu.selectMenu(mainMenu.getTab(), mainMenu.curSubMenu(mainMenu.getTab())+1);
				}
			}
		}
		if(key == CU::keyCode::k_space){
			running = false;
		}
		// Handle the user breaking the program
		switch((CUBreakType)videoDriver.halted()){
			case CUBreakType::SAVE_EXIT:
				// TODO:
				// Save the project
				running = false;
			break;
			case CUBreakType::COPY:
				// TODO:
				// Copy selected text to clipboard
				break;
			case CUBreakType::UNDO:
				// TODO:
				// Undo changes until the buffer is empty
				break;
		}

		videoDriver.clearHalt();

		videoDriver.flush();
		//videoDriver.updateDriver();

	}

};

void CUEditor::shutdown(){

};

void CUEditor::halt(int e){
	videoDriver.halt(e);
};

void CUEditor::drawGUI(){
	// Clear the screen
	videoDriver.clear();
	videoDriver.drawBar(0,0,videoDriver.getWidth(), videoDriver.getHeight(), ' ', settings.foreground_color, settings.background_color);
	// Draw the header
	videoDriver.drawBar(0,0,videoDriver.getWidth(), 1, ' ', settings.head_fg_color, settings.head_bg_color);

	// Write the IDE name
	videoDriver.writeStr("CU Edit IDE",0,0);

	// Write the current time
	std::time_t t = std::time(0);
    std::tm *tstruct = localtime(&t);
    	
	int hour = tstruct->tm_hour;
	int minute = tstruct->tm_min;
	int second = tstruct->tm_sec;

	std::string timestr = "";
	
	timestr += CU::to_string(hour,2) + ':';
	timestr += CU::to_string(minute,2) +':';
	timestr += CU::to_string(second,2);
	
	// Center the time
	videoDriver.writeStr(timestr,(videoDriver.getWidth()>>1) - (timestr.length() >> 1) ,0);

	// Write the current date
	
	int month = tstruct->tm_mon+1;
	int day = tstruct->tm_mday;
	int year = tstruct->tm_year+1900;

	std::string datestr = "";
	
	datestr += CU::to_string(month,2) + '/';
	datestr += CU::to_string(day,2) +'/';
	datestr += CU::to_string(year,4);
	
	// Center the time
	videoDriver.writeStr(datestr, (videoDriver.getWidth() - datestr.length()) ,0);

	// Draw the menus
	mainMenu.draw(videoDriver);

};


void CUEditor::loadSettings(){
	// Reset the settings
	settings.init();
	
	// Read the settings file if available
	std::ifstream settingsFile;
	settingsFile.open(CU_SETTINGS_FILE_NAME);
	
	if(settingsFile.is_open()){
		
		std::string line;
		while (std::getline(settingsFile, line)){
			
			if(line.length()==0){
				break;
			}

			std::string s_tag = "";
			
			int str_offset = 0;
			
			for(int i = 0; i < line.length(); i++){
				if(line.at(i) != ' '){
					s_tag.push_back(line.at(i));
				}else{
					str_offset = i+1;
					break;
				}
			}
			// Remove the last element if it's a colon
			if(s_tag.at(s_tag.length()-1)==':'){
				s_tag.pop_back();
			}
			
			
			int s_value = 0;
			std::string s_string = "";
			
			bool str_ident = false;
			
			for(int i = str_offset; i < line.length(); i++){
				if(line.at(i) == '\"' || line.at(i) == '\''){
					str_ident = !str_ident;
					if(str_ident){
						s_string.clear();
					}
				}else{
					s_string.push_back(line.at(i));
				}
			}
			
			s_value = CU::stoi(s_string);			
			
			if(s_tag.compare("BG-COLOR")==0){
				settings.background_color = (CU::Color)s_value;
			}
			else if(s_tag.compare("BG-FILL")==0){
				settings.background_pattern = s_string;
			}
			else if(s_tag.compare("FG-COLOR")==0){
				settings.foreground_color = (CU::Color)s_value;
			}
			else if(s_tag.compare("HEAD-BG-COLOR")==0){
				settings.head_bg_color = (CU::Color)s_value;
			}
			else if(s_tag.compare("HEAD-FG-COLOR")==0){
				settings.head_fg_color = (CU::Color)s_value;
			}
			else if(s_tag.compare("MENU-BAR-BG-COLOR")==0){
				settings.menu_bar_bg_color = (CU::Color)s_value;
			}
			else if(s_tag.compare("MENU-BAR-FG-COLOR")==0){
				settings.menu_bar_fg_color = (CU::Color)s_value;
			}
			else if(s_tag.compare("MENU-BG-COLOR")==0){
				settings.menu_bg_color = (CU::Color)s_value;
			}
			else if(s_tag.compare("MENU-FG-COLOR")==0){
				settings.menu_fg_color = (CU::Color)s_value;
			}
		}
	}
	 	
};

