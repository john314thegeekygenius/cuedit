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
	CU::debugWrite(editor.openFile());
	return 0;
};

int MMS_POpen(){
	return 0;
};

int MMS_FExit(){
	editor.close();
	CU::debugWrite("Quiting Program");
	return 0;
};

std::vector<CUSubMenu_t> MM_LFile = {
	{"Open", &MMS_FOpen}, 
	{"Save", &CUMenuFNULL}, 
	{"Save As", &CUMenuFNULL}, 
	{"Reload", &CUMenuFNULL}, 
	{"Close", &CUMenuFNULL}, 
	{"Exit", &MMS_FExit},
};

CUMenu_t MM_Sub_File = {
	"File", MM_LFile
};

std::vector<CUSubMenu_t> MM_LEdit = {
	{"Undo", &CUMenuFNULL}, 
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
	{"Open", &MMS_POpen}, 
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

void CUEditor::close(){
	running = false;
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
					// Run the sub function for that menu
					mainMenu.runSubMenu(mainMenu.getTab(), mainMenu.curSubMenu(mainMenu.getTab()));
					// Close the tab
					mainMenu.closeTab(mainMenu.getTab());
					// We don't want to still be on the main menu tabs
					MainMenuTabsSelected = false;
					mainMenu.selectTab(-1);
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

		// Handle the user breaking the program
		handleInt();

		videoDriver.clearHalt();

		videoDriver.flush();
		//videoDriver.updateDriver();

	}

};

void CUEditor::handleInt(){
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
			else if(s_tag.compare("SUB-MENU-BG-COLOR")==0){
				settings.sub_menu_bg_color = (CU::Color)s_value;
			}
			else if(s_tag.compare("SUB-MENU-FG-COLOR")==0){
				settings.sub_menu_fg_color = (CU::Color)s_value;
			}
		}
	}
	 	
};

std::string CUEditor::openFile(){
	bool dialogOpen = true;
	int menuWidth = 48;
	int menuHeight = 16;
	int menuX = (videoDriver.getWidth()>>1) - (menuWidth>>1);
	int menuY = (videoDriver.getHeight()>>1) - (menuHeight>>1)+1;
	int contentHeight = menuHeight - 5;

	std::string fpath = "";
	std::string lastModifiedString = "";

	std::vector<std::filesystem::directory_entry> folderContents;
	std::vector<std::string> folderContentNames;
	std::vector<std::pair<std::string, std::string>> folderFileTimes;
	std::vector<CU::EXEFileTypes> folderFileTypes;
	std::filesystem::path folderPath = std::filesystem::current_path();
	std::filesystem::path originalPath = std::filesystem::current_path();

	bool openFileBool = true;
	int fileSelected = 0;
	int fileScroll = 0;

	std::string parentPath = "";

	std::error_code filesystemErrorCode;

	auto addTime = [&](std::filesystem::file_time_type ftime) { 
		std::time_t cftime = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(ftime));
		std::tm *tstruct = localtime(&cftime);

		std::string dateString = "";
		dateString += std::to_string(tstruct->tm_mon+1)+"/";
		dateString += std::to_string(tstruct->tm_mday)+"/";
		dateString += std::to_string(tstruct->tm_year+1900);

		std::string timeString = "";
		timeString += std::to_string(tstruct->tm_hour)+":";
		timeString += std::to_string(tstruct->tm_min)+":";
		timeString += std::to_string(tstruct->tm_sec);

		folderFileTimes.push_back({dateString, timeString});
	};

	auto addFakeTime = [&](){
		folderFileTimes.push_back({"1/1/1900", "0:0:0"});
	};

	auto loadDirectory = [&](std::string c_path) {
		filesystemErrorCode.clear();
		if(c_path.length()){
			if(c_path.compare(".") == 0){
				std::filesystem::current_path(std::filesystem::path("/."),filesystemErrorCode); //set the path
			}else{
				std::filesystem::current_path(std::filesystem::path(c_path),filesystemErrorCode); //set the path
			}
		}else{
			std::filesystem::current_path(originalPath,filesystemErrorCode); //set the path
		}

		// TODO:
		// Add error handleing
		if(filesystemErrorCode.value()){
			ErrorMsgBox(filesystemErrorCode.message());
//			CU::debugWrite(filesystemErrorCode.message(),CU::DebugMsgType::ERROR);
		}
		
		// Reset the folder path
		folderPath = std::filesystem::current_path();
		// Reset to 0
		fileSelected = 0;
		// Grab the parent path
		parentPath = folderPath.parent_path(); 
		// Remove any old path files
		folderContents.clear();
		folderContentNames.clear();
		folderFileTimes.clear();
		folderFileTypes.clear();

		if(parentPath.length()){
			std::filesystem::file_time_type ftime;
			// Add a back options
			filesystemErrorCode.clear();
			folderContents.push_back(std::filesystem::directory_entry(std::filesystem::path("."),filesystemErrorCode));
			folderContentNames.push_back(".");
			folderFileTypes.push_back(CU::EXEFileTypes::Directory); // It's a directory

			filesystemErrorCode.clear();
			ftime = folderContents.back().last_write_time(filesystemErrorCode);
			if(filesystemErrorCode.value()){
				addFakeTime();
			}else{
				addTime(ftime);
			}
			filesystemErrorCode.clear();
			folderContents.push_back(std::filesystem::directory_entry(std::filesystem::path(".."),filesystemErrorCode));
			folderContentNames.push_back("..");
			folderFileTypes.push_back(CU::EXEFileTypes::Directory); // It's a directory

			filesystemErrorCode.clear();
			ftime = folderContents.back().last_write_time(filesystemErrorCode);
			if(filesystemErrorCode.value()){
				addFakeTime();
			}else{
				addTime(ftime);
			}
			filesystemErrorCode.clear();
		}

		for (const auto & entry : std::filesystem::directory_iterator(folderPath)){
			folderContents.push_back(entry);
			folderContentNames.push_back(entry.path().filename());
			
			filesystemErrorCode.clear();
			std::filesystem::file_time_type ftime = entry.last_write_time(filesystemErrorCode);
			if(filesystemErrorCode.value()){
				addFakeTime();
			}else{
				addTime(ftime);
			}
			filesystemErrorCode.clear();
			if(entry.is_directory()){
				folderFileTypes.push_back(CU::EXEFileTypes::Directory); // It's a directory
			}else if(entry.is_symlink()){
				folderFileTypes.push_back(CU::EXEFileTypes::Symbolic); // Symbolic Link
			}else if(entry.is_regular_file()){
				// Open the file and read the header
				char headerBuffer[128];
				FILE *fp = fopen(entry.path().c_str(),"rb");
				if(fp!=NULL){
					fread(&headerBuffer,128,1,fp);
					fclose(fp); fp = NULL;
					// Check the first two bytes
					if(headerBuffer[0] == 'M' && headerBuffer[1] == 'Z'){
						folderFileTypes.push_back(CU::EXEFileTypes::EXE); // Windows Executable
					}else if(headerBuffer[0] == 0x7f && headerBuffer[1] == 0x45 && 
							headerBuffer[2] == 0x4c && headerBuffer[3] == 0x46){
						folderFileTypes.push_back(CU::EXEFileTypes::ELF); // ELF Executable
					}else{
						folderFileTypes.push_back(CU::EXEFileTypes::Normal); // Normal file
					}
				}else{
					folderFileTypes.push_back(CU::EXEFileTypes::Broken); // Error opening
				}
			}else{
				folderFileTypes.push_back(CU::EXEFileTypes::Unknown); // Unknown				
			}
			//CU::debugWrite(std::string(entry.path().filename())+"   "+timeString);
		}
	};

	loadDirectory("");

	while(dialogOpen){
		// Draw the window

		// Draw the background
		videoDriver.drawBar(menuX,menuY,menuWidth,menuHeight, ' ', settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a feild
		videoDriver.drawBox(menuX,menuY,menuWidth,menuHeight, CU::BlockType::SINGLE, settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		videoDriver.drawSubBox(menuX,menuY+contentHeight,menuWidth, 5, CU::BlockType::SINGLE, settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a title bar
		videoDriver.drawBar(menuX,menuY,menuWidth, 1, ' ', settings.menu_bar_fg_color, settings.menu_bar_bg_color);
		videoDriver.writeStr("Open File", menuX + (menuWidth>>1)-5, menuY);

		// Write all the files
		for(int i = 0; i < contentHeight-1; i++){
			int fileOffset = i + fileScroll; //std::max(fileSelected-(contentHeight-1),0);
			if(fileSelected-fileScroll < 0){
				fileScroll -= 1;
			}
			if(fileSelected-fileScroll >= contentHeight-1){
				fileScroll += 1;
			}

			if(fileOffset >= folderContentNames.size()){
				break;
			}
			videoDriver.writeStrW(folderContentNames[fileOffset], menuX + 2, menuY+i+1, 8);
			// Test if it's a directory
			if(folderFileTypes[fileOffset] == CU::EXEFileTypes::Directory){
				videoDriver.writeStr("<DIR>", menuX + menuWidth-18, menuY+i+1);
			}else
			// Check if it's an executable
			if(folderFileTypes[fileOffset] == CU::EXEFileTypes::EXE){
				videoDriver.writeStr("[EXE]", menuX + menuWidth-18, menuY+i+1);
			}else
			if(folderFileTypes[fileOffset] == CU::EXEFileTypes::ELF){
				videoDriver.writeStr("[ELF]", menuX + menuWidth-18, menuY+i+1);
			}else
			if(folderFileTypes[fileOffset] == CU::EXEFileTypes::Symbolic){
				videoDriver.writeStr("<SYM>", menuX + menuWidth-18, menuY+i+1);
			}else
			if(folderFileTypes[fileOffset] == CU::EXEFileTypes::Broken){
				videoDriver.writeStr("[???]", menuX + menuWidth-18, menuY+i+1);
			}
			
			videoDriver.writeStr(folderFileTimes[fileOffset].second, menuX + menuWidth-4-(folderFileTimes[fileOffset].second.length()), menuY+i+1);
			
			if(fileSelected == fileOffset){
				videoDriver.writeStr("[",menuX+1,menuY+i+1);
				videoDriver.writeStr("]",menuX+menuWidth-3,menuY+i+1);
			}
		}

		// Draw a scroll bar
		int filescale = std::max((int)folderContents.size()-contentHeight,1);
		float scrollBarScale = 1.0f / (float)(filescale);
		int scrollBarHeight = contentHeight*scrollBarScale;
		if(scrollBarHeight < 1){
			scrollBarHeight = 1;
		}
		for(int i = 0; i < contentHeight-1; i++){
			if(scrollBarHeight){
				videoDriver.writeStr("#",menuX+menuWidth-2,menuY+i+1);
				scrollBarHeight -= 1;
			}else{
				videoDriver.writeStr(":",menuX+menuWidth-2,menuY+i+1);
			}
		}

		// Write some text
		videoDriver.writeStrW("Open: "+std::string(folderContents[fileSelected].path().filename()), menuX + 1, menuY + menuHeight-4,30);

		std::string permissionString = "";
		std::filesystem::file_status file_status = status(folderContents[fileSelected].path());
		if( (file_status.permissions() & std::filesystem::perms::group_read) != std::filesystem::perms::none){
			permissionString += "Read ";
		}
		if( (file_status.permissions() & std::filesystem::perms::group_write) != std::filesystem::perms::none){
			permissionString += "Write ";
		}
		if( (file_status.permissions() & std::filesystem::perms::group_exec) != std::filesystem::perms::none){
			permissionString += "Execute";
		}

		videoDriver.writeStrW("Permisions: "+permissionString, menuX + 1, menuY + menuHeight-3,30);

		videoDriver.writeStr("Last modified:"+folderFileTimes[fileSelected].first+" "+folderFileTimes[fileSelected].second, menuX + 1, menuY + menuHeight-2);

		CU::keyCode key = videoDriver.getkey();

		if(key == CU::keyCode::k_escape){
			dialogOpen = false;
			// Reset the directory
			filesystemErrorCode.clear();
			std::filesystem::current_path(originalPath,filesystemErrorCode); //set the path
		}
		if(key == CU::keyCode::k_tab){
		}
		if(key == CU::keyCode::s_up){
			fileSelected -= 1;
			if(fileSelected < 0){ fileSelected = 0; }
		}
		if(key == CU::keyCode::s_down){
			fileSelected += 1;
			if(fileSelected >= folderContents.size()) { fileSelected = folderContents.size()-1; }
		}

		if(key == CU::keyCode::k_enter){
			// If it's a directory, enter it
			if(folderContents[fileSelected].is_directory()){
				CU::debugWrite("Entering:"+std::string(folderContents[fileSelected].path()));
				loadDirectory(std::string(folderContents[fileSelected].path()));
			}else{
				// Open the file if we can
				dialogOpen = false;
				// TODO:
				// Open the file
			}
		}

		// Handle the user breaking the program
		handleInt();

		videoDriver.clearHalt();

		videoDriver.flush();
		//videoDriver.updateDriver();
	}
	return fpath;
};

void CUEditor::ErrorMsgBox(std::string error){
	int menuWidth = 32;
	int menuHeight = 8;
	int menuX = (videoDriver.getWidth()>>1) - (menuWidth>>1);
	int menuY = (videoDriver.getHeight()>>1) - (menuHeight>>1)+1;

	bool userAck = false;
	while(!userAck){

		CU::keyCode key = videoDriver.getkey();

		if(key == CU::keyCode::k_escape || key == CU::keyCode::k_return){
			userAck = true;
		}

		// Draw a window

		// Draw the background
		videoDriver.drawBar(menuX,menuY,menuWidth,menuHeight, ' ', settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a feild
		videoDriver.drawBox(menuX,menuY,menuWidth,menuHeight, CU::BlockType::SINGLE, settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a title bar
		videoDriver.drawBar(menuX,menuY,menuWidth, 1, ' ', settings.menu_bar_fg_color, settings.menu_bar_bg_color);
		videoDriver.writeStr("Error!", menuX + (menuWidth>>1)-5, menuY);

		videoDriver.writeStr(error, menuX + (menuWidth>>1)-(error.length()>>1), menuY+(menuHeight>>1));

		videoDriver.clearHalt();

		videoDriver.flush();
		//videoDriver.updateDriver();
	}
};

