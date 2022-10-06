/*

	CU-Edit - A text based C++ IDE

	Written by: Jonathan Clevenger

	Started: 10/3/2022

	Change log:
	*
	*
*/

#include "cuheads.h"

int main(int argc, char *argv[]){
	CUEditor editor;

	editor.run();
	editor.shutdown();
	return 0;
};

CUMenu mainMenu;
CUMenu_t MM_Sub_File = {
	"File", { "Open", "Save", "Save As", "Reload", "Close", "Exit"},
};
CUMenu_t MM_Sub_Edit = {
	"Edit", { "Undo", "Redo", "Copy", "Cut", "Paste"},
};
CUMenu_t MM_Sub_Settings = {
	"Settings", {},
};
CUMenu_t MM_Sub_Project = {
	"Project", { "Open", "Save", "Save As", "Reload", "Close", "Settings", "Exit"},
};

CUEditor::CUEditor(){
	running = true;

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

void CUEditor::loadSettings(){
	
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

void CUEditor::run(){
	int cx = 0;
	int cy = 0;
	while(running){
/*		videoDriver.setCurPos(cx,cy);
		videoDriver.setFGColor((CU::Color)(cx%16));
		int c = (int)CU::BlockChar::TLCORNER;
		c += (cy%16);
		videoDriver.writeBChar((CU::BlockChar)c);
		videoDriver.flush();
		cx += 1;
		cy += 1;
		if(cx >= videoDriver.getWidth()){
			cx = 0;
		}
		if(cy >= videoDriver.getHeight()){
			cy = 0;
		}*/
		
		drawGUI();
				
		if(videoDriver.kbhit()){
			char ch = videoDriver.getch();
			if(ch == ' '){
				running = false;
			}
		}
		videoDriver.flush();
		//videoDriver.updateDriver();
	}

};

void CUEditor::drawGUI(){
	// Clear the screen
	videoDriver.clear();
	videoDriver.drawBar(0,0,videoDriver.getWidth(), videoDriver.getHeight(), ' ', settings.foreground_color, settings.background_color);
	// Draw the header
	videoDriver.drawBar(0,0,videoDriver.getWidth(), 1, ' ', settings.head_fg_color, settings.head_bg_color);

	// Write the IDE name
	videoDriver.writeStr("CU Edit IDE ",0,0);
/*
	if(videoDriver.getWidth() > CU_EDIT_MIN_WIDTH){
		videoDriver.writeStr("CU Edit IDE ",0,0);
	}else{
		videoDriver.writeStr("CU IDE ",0,0);
	}*/

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
	
	/*
	if(videoDriver.getWidth() <= CU_EDIT_MIN_WIDTH){
		// Remove the seconds
		timestr.pop_back();
		timestr.pop_back();
		timestr.pop_back();
	}*/
	
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
	
	/*
	if(videoDriver.getWidth() <= CU_EDIT_MIN_WIDTH){
		// Remove the year
		datestr.pop_back();
		datestr.pop_back();
		datestr.pop_back();
		datestr.pop_back();
		datestr.pop_back();
	}*/
	
	// Center the time
	videoDriver.writeStr(datestr, (videoDriver.getWidth() - datestr.length()) ,0);

	// Draw the menus
	mainMenu.draw(videoDriver);
	
	for(int i = 0; i < 16; i++){
		videoDriver.setCurPos(4+i,2);
		videoDriver.setBGColor((CU::Color)i);
		videoDriver.setCurPos(4+i,3);
		videoDriver.setFGColor((CU::Color)i);
		videoDriver.putChar('#');
	}
};

void CUEditor::shutdown(){
};
