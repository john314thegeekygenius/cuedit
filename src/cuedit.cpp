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
	editor.init(argc, argv);

	// Make CUEdit open files from the arguments list
	editor.loadArgFiles(argc, argv);

	editor.run();
	editor.shutdown();
	CU::closeDebugFile();
	return 0;
};

CUMenu mainMenu;

int MMS_FOpen(){
	std::string path = editor.openFile();
	if(path.length()){
		// We want to edit the file now
		editor.EditorSelected = true;
		editor.EditorOpen = true;
	}
	return 0;
};

int MMS_FNew(){
	editor.createFile();
	// We want to edit the file now
	editor.EditorSelected = true;
	editor.EditorOpen = true;
	return 0;
};

int MMS_FSave(){
	editor.saveFile(true);
	return 0;
};

int MMS_FSaveAs(){
	editor.saveFile(false);
	return 0;
};

int MMS_FReload(){
	editor.reloadCurrentFile();
	return 0;
};

int MMS_FClose(){
	// Close the current file
	editor.closeCurrentFile();
	return 0;
};

int MMS_FExit(){
	editor.close();
	CU::debugWrite("Quiting Program");
	return 0;
};

std::vector<CUSubMenu_t> MM_LFile = {
	{"New", &MMS_FNew}, 
	{"Open", &MMS_FOpen}, 
	{"Save", &MMS_FSave}, 
	{"Save As", &MMS_FSaveAs}, 
	{"Reload", &MMS_FReload}, 
	{"Close", &MMS_FClose}, 
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


int MMS_POpen(){
	return 0;
};

std::vector<CUSubMenu_t> MM_LProject = {
	{"Open", &MMS_POpen}, 
	{"Save", &CUMenuFNULL}, 
	{"Save As", &CUMenuFNULL}, 
	{"Reload", &CUMenuFNULL}, 
	{"Close", &CUMenuFNULL}, 
	{"Settings", &CUMenuFNULL}, 
};

std::vector<CUSubMenu_t> MM_LHelp = {
	{"About", &CUMenuFNULL}, 
	{"Help", &CUMenuFNULL}, 
};

CUMenu_t MM_Sub_Project = {
	"Project", MM_LProject
};

CUMenu_t MM_Sub_Help = {
	"Help", MM_LHelp
};


void CUEditor::init(int argc, char *argv[]){
	running = true;

	// Setup the break handler
	videoDriver.setupHandle(&breakHandle);
	videoDriver.setFPS(30);

	MainMenuTabsSelected = false;
	EditorSelected = false;
	TerminalSelected = false;

	EditorOpen = false;
	TerminalOpen = false;

	shuttingDown = false;

	clipboard.clear();

	loadSettings();

	fileTabSelected = 0;
	cursorTime = 0;
	fileList.clear(); // Remove any files open
	
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
	mainMenu.addTab(MM_Sub_Help);

	// Open any files in the arguments
	for(int i = 1; i < argc; i++){
		std::string path = editor.loadFile(argv[i], false);
		if(path.length()){
			// We want to edit the file now
			editor.EditorSelected = true;
			editor.EditorOpen = true;
		}else{
			// Open a new file
			editor.createFile(argv[i]);
			// We want to edit the file now
			editor.EditorSelected = true;
			editor.EditorOpen = true;
		}
	}	
};

void CUEditor::close(){
	shuttingDown = true;
	fileTabSelected = 0;
};

void CUEditor::run(){
	int cx = 0;
	int cy = 0;
	while(running){
		
		drawGUI();
		if(shuttingDown){
			// Close all the files
			if(fileList.size()){
				if(closeCurrentFile() == false){
					shuttingDown = false;
				}
			}else{
				running = false;
			}
		}else{

			CU::keyCode key = videoDriver.getkey();

			if(key == CU::keyCode::k_escape){
				MainMenuTabsSelected = !MainMenuTabsSelected;
				if(MainMenuTabsSelected){
					mainMenu.selectTab(0);
				}else{
					mainMenu.closeTab(mainMenu.getTab());
					mainMenu.selectTab(-1);
				}
			}

			if(MainMenuTabsSelected){
				if(key == CU::keyCode::s_left){
					mainMenu.closeTab(mainMenu.getTab());
					int cur = mainMenu.getTab() - 1;
					if(cur < 0) { cur = 0; }
					mainMenu.selectTab(cur);
				}
				if(key == CU::keyCode::s_right){
					mainMenu.closeTab(mainMenu.getTab());
					int cur = mainMenu.getTab() + 1;
					if(cur >= mainMenu.numTabs()) { cur = mainMenu.numTabs()-1; }
					mainMenu.selectTab(cur);
				}
				if(key == CU::keyCode::k_tab){
					mainMenu.closeTab(mainMenu.getTab());
					int cur = mainMenu.getTab() + 1;
					if(cur >= mainMenu.numTabs()) { cur = 0; }
					mainMenu.selectTab(cur);
				}
				if(key == CU::keyCode::k_enter){
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
				if(key == CU::keyCode::s_up){
					if(mainMenu.tabOpen(mainMenu.getTab())){
						mainMenu.selectMenu(mainMenu.getTab(), mainMenu.curSubMenu(mainMenu.getTab())-1);
					}
				}
				if(key == CU::keyCode::s_down){
					if(mainMenu.tabOpen(mainMenu.getTab())){
						mainMenu.selectMenu(mainMenu.getTab(), mainMenu.curSubMenu(mainMenu.getTab())+1);
					}
				}
			}else{
				if(EditorSelected){
					doEditor(key);
				}else if(TerminalSelected){
					// TODO:
					// Add terminal
				}
			}
		}

		// Make the cursor blink every half a second
		using namespace std::chrono;
		uint64_t vtime_now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		if(vtime_now >= cursorTime){
			cursorTime = vtime_now + 500;
			cursorBlink = !cursorBlink; 
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

	// Draw the editor
	if(EditorOpen){
		drawEditor();
	}

	// Draw the menus
	mainMenu.draw(videoDriver,0);

};


void CUEditor::loadSettings(){
	// Reset the settings
	settings.init();

	// Get the executable location
	std::string exe_location = std::filesystem::canonical(std::filesystem::path("/proc/self/exe"));
	exe_location = std::filesystem::path(exe_location).parent_path();

	CU::debugWrite("Execuatble Location:"+exe_location);

	// Read the settings file if available
	std::ifstream settingsFile;
	settingsFile.open(exe_location+"/"+CU_SETTINGS_FILE_NAME);
	
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
			else if(s_tag.compare("LINE-NUM-BG-COLOR")==0){
				settings.editor_line_bg_color = (CU::Color)s_value;
			}
			else if(s_tag.compare("LINE-NUM-FG-COLOR")==0){
				settings.editor_line_fg_color = (CU::Color)s_value;
			}
			
		}
	}
	 	
};

std::string CUEditor::openFileDialog(std::string WinName, CU::FileAccess access_type){
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

	using namespace std::chrono_literals;

	auto addTime = [&](std::filesystem::file_time_type ftime) { 
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(ftime - std::filesystem::file_time_type::clock::now() + system_clock::now());

		std::time_t cftime = system_clock::to_time_t(sctp);
		std::tm *tstruct = std::localtime(&cftime);

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

	int folderMaxScroll = 0;

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
		// Warn the user
		if(filesystemErrorCode.value()){
			ErrorMsgBox(filesystemErrorCode.message());
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
			//CU::debugWrite(entry.path().c_str());
			
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


		int fcount = 0;
		// Reset this
		folderMaxScroll = 0;
		// Count the number of scrolls
		while(fcount < folderContents.size()){
			if((fcount - folderMaxScroll) >= contentHeight-1){
				folderMaxScroll += 1;
			}
			fcount ++;
		}

	};

	loadDirectory("");

	bool fileSelectSelected = true;

	std::string WriteFileName = "dummy.txt";

	while(dialogOpen){
		// Draw the window

		// Draw the background
		videoDriver.drawBar(menuX,menuY,menuWidth,menuHeight, ' ', settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a feild
		videoDriver.drawBox(menuX,menuY,menuWidth,menuHeight, CU::BlockType::SINGLE, settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		videoDriver.drawSubBox(menuX,menuY+contentHeight,menuWidth, 5, CU::BlockType::SINGLE, settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a title bar
		videoDriver.drawBar(menuX,menuY,menuWidth, 1, ' ', settings.menu_bar_fg_color, settings.menu_bar_bg_color);
		videoDriver.writeStr(WinName, menuX + (menuWidth>>1)-5, menuY);

		// Write all the files
		for(int i = 0; i < contentHeight-1; i++){
			int fileOffset = i + fileScroll; //std::max(fileSelected-(contentHeight-1),0);

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
			
			if(fileSelected == fileOffset && fileSelectSelected){
				videoDriver.writeStr("[",menuX+1,menuY+i+1);
				videoDriver.writeStr("]",menuX+menuWidth-3,menuY+i+1);
			}
		}

		// Draw a scroll bar
		float scrollBarScale = (float)contentHeight / (float)folderContents.size();//std::max((int)folderContents.size()-contentHeight,1);
		int scrollBarHeight = round(contentHeight*scrollBarScale)-2;

		if(scrollBarHeight < 1){
			scrollBarHeight = 1;
		}
		if(scrollBarHeight >= contentHeight){
			scrollBarHeight = contentHeight-1;
		}

		int fsy = round((float)fileScroll * ( scrollBarScale));
		
		// Force the position to be lower (because of oddly numbered folder content)
		if(fileScroll == folderMaxScroll && scrollBarHeight==2){
			fsy += 1;
		}

		for(int i = 0; i < contentHeight-1; i++){
			if(scrollBarHeight && i >= fsy){
				videoDriver.writeStr("#",menuX+menuWidth-2,menuY+i+1);
				scrollBarHeight -= 1;
			}else{
				videoDriver.writeStr(":",menuX+menuWidth-2,menuY+i+1);
			}
		}


		// Write some text
		// std::string(folderContents[fileSelected].path().filename())
		
		if(access_type == CU::FileAccess::READ){
			videoDriver.writeStrW("Open: "+std::string(folderContents[fileSelected].path().filename()), menuX + 1, menuY + menuHeight-4,30);
		}else{
			std::string writestr = "";
			if(!fileSelectSelected) { writestr += '['; }
			writestr += "Write";
			if(!fileSelectSelected) { writestr += ']'; }
			writestr += ": ";
			writestr += WriteFileName;
			videoDriver.writeStrW(writestr, menuX + 1, menuY + menuHeight-4,30);
		}

		std::string permissionString = "";
		std::filesystem::file_status file_status = status(folderContents[fileSelected].path());
		std::filesystem::file_status folder_status = status(folderContents[fileSelected].path().parent_path());

		if(access_type == CU::FileAccess::READ){
			if(access(folderContents[fileSelected].path().c_str(), R_OK) == 0){
//			if( (file_status.permissions() & std::filesystem::perms::owner_read) != std::filesystem::perms::none){
				permissionString += "Read ";
			}
			if(access(folderContents[fileSelected].path().c_str(), W_OK) == 0){
//			if( (file_status.permissions() & std::filesystem::perms::group_write) != std::filesystem::perms::none){
				permissionString += "Write ";
			}
			if(access(folderContents[fileSelected].path().c_str(), X_OK) == 0){
//			if( (file_status.permissions() & std::filesystem::perms::group_exec) != std::filesystem::perms::none){
				permissionString += "Execute";
			}

			videoDriver.writeStrW("Permisions: "+permissionString, menuX + 1, menuY + menuHeight-3,30);
		}

		videoDriver.writeStr("Last modified:"+folderFileTimes[fileSelected].first+" "+folderFileTimes[fileSelected].second, menuX + 1, menuY + menuHeight-2);

		CU::keyCode key = videoDriver.getkey();

		if(key == CU::keyCode::k_escape){
			dialogOpen = false;
			// Reset the directory
			filesystemErrorCode.clear();
			std::filesystem::current_path(originalPath,filesystemErrorCode); //set the path
		}
		if(key == CU::keyCode::k_tab){
			fileSelectSelected = !fileSelectSelected;
		}
		if(access_type == CU::FileAccess::WRITE){
			if(key == CU::keyCode::k_space){
				WriteFileName = getUserString("Enter filename to save to");
			}
		}
		if(fileSelectSelected){
			if(key == CU::keyCode::s_up){
				fileSelected -= 1;
				if(fileSelected < 0){ fileSelected = 0; }
			}
			if(key == CU::keyCode::s_down){
				fileSelected += 1;
				if(fileSelected >= folderContents.size()) { fileSelected = folderContents.size()-1; }
			}

			if(fileSelected-fileScroll < 0){
				fileScroll -= 1;
			}
			if(fileSelected-fileScroll >= contentHeight-1){
				fileScroll += 1;
			}
		}

		if(key == CU::keyCode::k_enter){
			if(fileSelectSelected){
				// If it's a directory, enter it
				if(folderContents[fileSelected].is_directory()){
	//				CU::debugWrite("Entering:"+std::string(folderContents[fileSelected].path()));
					loadDirectory(std::string(folderContents[fileSelected].path()));
				}else{
					// Open the file if we can
					if(access_type == CU::FileAccess::READ){
						if(access(folderContents[fileSelected].path().c_str(), R_OK) != 0){
						//if( (file_status.permissions() & std::filesystem::perms::group_read) == std::filesystem::perms::none){
							ErrorMsgBox("File not accessible!");
						}else{
							dialogOpen = false;
							fpath = loadFile(folderContents[fileSelected].path());
						}
					}
					else if(access_type == CU::FileAccess::WRITE){
						WriteFileName = folderContents[fileSelected].path().filename();
					}
				}
			}else{
				if(access_type == CU::FileAccess::WRITE){
//						if( (folder_status.permissions() & std::filesystem::perms::group_write) == std::filesystem::perms::none){
//							ErrorMsgBox("Cannot write to directory");
//						}else{
					// Make sure the user wants to overwrite the file if same name
					bool canWriteFile = true;
					for(int i = 0; i < folderContents.size(); i++){
						if(WriteFileName == folderContents[i].path().filename()){
							// Uh oh!
							canWriteFile = AreYouSure("Overwrite that file?");
						}
					}
					if(canWriteFile){
						// Write the file
						dialogOpen = false;
						CU::FileMode fmode = CU::FileMode::READ_WRITE;
						CU::ErrorCode fecode = fileList[fileTabSelected].save(std::string(folderPath.c_str()) + "/" + WriteFileName,fmode);
						if(fecode==CU::ErrorCode::OPEN){
							ErrorMsgBox("Failed to open!"); 
							fileList.pop_back();
						}else if(fecode==CU::ErrorCode::WRITE){
							ErrorMsgBox("Writing Error!"); 
							fileList.pop_back();
						}else if(fecode==CU::ErrorCode::LARGE){
							ErrorMsgBox("File too large!"); 
							fileList.pop_back();
						}else {
							fpath = folderContents[fileTabSelected].path();
						}
					}
				}
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

std::string CUEditor::loadFile(std::string load_path, bool showError){
	std::string fpath = "";
	fileList.emplace_back(CU::File());
	CU::FileMode fmode = CU::FileMode::READ_ONLY;
	if(access(load_path.c_str(), W_OK) == 0){
	//if( (file_status.permissions() & std::filesystem::perms::group_write) != std::filesystem::perms::none){
		fmode = CU::FileMode::READ_WRITE;
	}
	CU::ErrorCode fecode = fileList.back().open(load_path,fmode);
	if(fecode==CU::ErrorCode::OPEN){
		if(showError)
			ErrorMsgBox("Failed to open!"); 
		fileList.pop_back();
	}else if(fecode==CU::ErrorCode::READ){
		if(showError)
			ErrorMsgBox("Reading Error!"); 
		fileList.pop_back();
	}else if(fecode==CU::ErrorCode::LARGE){
		if(showError)
			ErrorMsgBox("File too large!"); 
		fileList.pop_back();
	}else {
		fpath = load_path;
		fileTabSelected = fileList.size()-1;
		CU::fileInfo finfo;
		fileInfo.push_back(finfo);
	}
	return fpath;
}

std::string CUEditor::openFile(){
	return openFileDialog("Open File",CU::FileAccess::READ);
};

std::string CUEditor::saveFile(bool overwrite){
	if(!fileList.size()) { return ""; }
	if(overwrite){
		// If we already saved, just overwrite it!
		if(fileList[fileTabSelected].hasBeenSaved){
			CU::FileMode fmode = CU::FileMode::READ_WRITE;
			CU::ErrorCode fecode = fileList[fileTabSelected].save(fileList[fileTabSelected].getPath(),fmode);
			if(fecode==CU::ErrorCode::OPEN){
				ErrorMsgBox("Failed to open!"); 
				fileList.pop_back();
			}else if(fecode==CU::ErrorCode::WRITE){
				ErrorMsgBox("Writing Error!"); 
				fileList.pop_back();
			}else if(fecode==CU::ErrorCode::LARGE){
				ErrorMsgBox("File too large!"); 
				fileList.pop_back();
			}
			return fileList[fileTabSelected].getPath();
		}
	}
	return openFileDialog("Save File",CU::FileAccess::WRITE);
};

void CUEditor::reloadCurrentFile(){
	if(!fileList.size()) { return ; }
	// If we didn't open it as a file, WE CAN'T RELOAD IT!
	if(!fileList[fileTabSelected].hasBeenSaved){
		return;
	}
	// If we haven't saved, ask!
	if(fileList[fileTabSelected].modified){
		// Ask if the user wants to save the file
		if(!AreYouSure("Reload without saving?")){
			// If so, Save the file
			if(saveFile(true).length() == 0){
				return;
			}
		}
	}

	CU::FileMode fmode = CU::FileMode::READ_WRITE;
	CU::ErrorCode fecode = fileList[fileTabSelected].open(fileList[fileTabSelected].getPath(),fmode);
	if(fecode==CU::ErrorCode::OPEN){
		ErrorMsgBox("Failed to open!"); 
		fileList.pop_back();
	}else if(fecode==CU::ErrorCode::READ){
		ErrorMsgBox("Reading Error!"); 
		fileList.pop_back();
	}else if(fecode==CU::ErrorCode::LARGE){
		ErrorMsgBox("File too large!"); 
		fileList.pop_back();
	}
	return;

};

void CUEditor::loadArgFiles(int argc, char *argv[]){
	for(int i = 1; i < argc; i++){
		if(std::strcmp(argv[i],"/no-color")==0){
			// Force no color support
		}
		if(std::strcmp(argv[i],"/color")==0){
			// Force color support

		}
	}
};

bool CUEditor::closeCurrentFile(){
	// Check if the file is modified
	if(!fileList[fileTabSelected].hasBeenSaved || fileList[fileTabSelected].modified){
		// Ask if the user wants to save the file
		if(!AreYouSure("Close without saving?")){
			// If so, Save the file
			if(saveFile(true).length() == 0){
				return false;
			}
		}
	}

	fileList.erase(fileList.begin()+fileTabSelected);
	fileInfo.erase(fileInfo.begin()+fileTabSelected);
	fileTabSelected += 1;
	if(fileTabSelected >= fileList.size()){
		fileTabSelected = fileList.size()-1;
	}
	if(fileTabSelected < 0){
		fileTabSelected = 0;
	}
	if(fileList.size() == 0){
		// Close the editor
		EditorSelected = false;
		EditorOpen = false;
	}
	return true;
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

bool CUEditor::AreYouSure(std::string warning){
	int menuWidth = 32;
	int menuHeight = 8;
	int menuX = (videoDriver.getWidth()>>1) - (menuWidth>>1);
	int menuY = (videoDriver.getHeight()>>1) - (menuHeight>>1)+1;

	bool returnValue = false;

	bool userAck = false;

	bool yesNoSelect = false;
	while(!userAck){

		CU::keyCode key = videoDriver.getkey();

		if(key == CU::keyCode::k_escape){
			userAck = true;
			returnValue = false;
		}
		if(key == CU::keyCode::l_y){
			userAck = true;
			returnValue = true;
		}
		if(key == CU::keyCode::l_n){
			userAck = true;
			returnValue = false;
		}
		if(key == CU::keyCode::s_left){
			yesNoSelect = false;
		}
		if(key == CU::keyCode::s_right){
			yesNoSelect = true;
		}
		if(key == CU::keyCode::k_tab){
			yesNoSelect = !yesNoSelect;
		}
		if(key == CU::keyCode::k_enter){
			userAck = true;
			returnValue = yesNoSelect;
		}

		// Draw a window

		// Draw the background
		videoDriver.drawBar(menuX,menuY,menuWidth,menuHeight, ' ', settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a feild
		videoDriver.drawBox(menuX,menuY,menuWidth,menuHeight, CU::BlockType::SINGLE, settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a title bar
		videoDriver.drawBar(menuX,menuY,menuWidth, 1, ' ', settings.menu_bar_fg_color, settings.menu_bar_bg_color);
		videoDriver.writeStr("Are you sure?", menuX + (menuWidth>>1)-5, menuY);
		std::string wmsg = "Are you sure you want to";
		videoDriver.writeStr(wmsg, menuX + (menuWidth>>1)-(wmsg.length()>>1), menuY+(menuHeight>>1));
		videoDriver.writeStr(warning, menuX + (menuWidth>>1)-(warning.length()>>1), menuY+(menuHeight>>1)+1);

		if(yesNoSelect){
			videoDriver.writeStr(" NO", menuX + 2, menuY+menuHeight-2);
			videoDriver.writeStr("[YES]", menuX + (menuWidth)-6, menuY+menuHeight-2);
		}else{
			videoDriver.writeStr("[NO]", menuX + 2, menuY+menuHeight-2);
			videoDriver.writeStr(" YES", menuX + (menuWidth)-6, menuY+menuHeight-2);
		}
		videoDriver.clearHalt();

		videoDriver.flush();
		//videoDriver.updateDriver();
	}
	return returnValue;
};


std::string CUEditor::getUserString(std::string msg,int maxLength){
	int menuWidth = 32;
	int menuHeight = 8;
	int menuX = (videoDriver.getWidth()>>1) - (menuWidth>>1);
	int menuY = (videoDriver.getHeight()>>1) - (menuHeight>>1)+1;

	std::string outputString = "";

	bool userAck = false;

	while(!userAck){

		CU::keyCode key = videoDriver.getkey();

		if(key == CU::keyCode::k_escape){
			userAck = true;
		}
		if(key == CU::keyCode::k_enter){
			userAck = true;
		}
		if( (maxLength>0 && outputString.length() < maxLength) || maxLength == -1){
			// Read characters
			if( ((int)key >= (int)CU::keyCode::k_space) && ((int)key <= (int)CU::keyCode::k_grave) ){
				outputString.push_back((char)key);
			}
			if(key == CU::keyCode::k_backspace){
				outputString.pop_back();
			}
		}

		// Draw a window

		// Draw the background
		videoDriver.drawBar(menuX,menuY,menuWidth,menuHeight, ' ', settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a feild
		videoDriver.drawBox(menuX,menuY,menuWidth,menuHeight, CU::BlockType::SINGLE, settings.sub_menu_fg_color, settings.sub_menu_bg_color);
		// Draw a title bar
		videoDriver.drawBar(menuX,menuY,menuWidth, 1, ' ', settings.menu_bar_fg_color, settings.menu_bar_bg_color);
		// Draw a text feild
		videoDriver.drawBox(menuX+2,menuY+3,menuWidth-4,3, CU::BlockType::DOUBLE, settings.sub_menu_fg_color, settings.sub_menu_bg_color);

		videoDriver.writeStr("Input Feild", menuX + (menuWidth>>1)-5, menuY);
		videoDriver.writeStr(msg, menuX + (menuWidth>>1)-(msg.length()>>1), menuY+2);

		videoDriver.writeStr(outputString, menuX + 3, menuY+4);

		videoDriver.clearHalt();

		videoDriver.flush();
		//videoDriver.updateDriver();
	}
	return outputString;
};

void CUEditor::createFile(std::string name){
	fileList.emplace_back(CU::File());
	std::string fname = "";
	int fileCount = 0;
	for(int i = 0; i < fileList.size();  i ++){
		if(fileList[i].getName().compare(0,8,name)==0){
			fileCount += 1;
		}
	}
	if(fileCount){
		fname = name+"_"+std::to_string(fileCount);
	}else{
		fname = name;
	}
	fileList.back().openNew(fname,CU::FileMode::READ_WRITE);
	CU::fileInfo finfo;
	fileInfo.push_back(finfo);
	fileTabSelected = fileList.size()-1;
};

void CUEditor::fixCursor(){
	int winWidth = videoDriver.getWidth()-2;
	int winHeight = videoDriver.getHeight()-6;

	fileInfo[fileTabSelected].cursorOffset = 0;

	std::vector<char> &filedata = fileList[fileTabSelected].getData();

	// Offset scrollY number of line breaks
	int lineBreakCount = 0;
	int fileOffset = filedata.size();
	bool vertEOF = true;
	for(int i = 0; i < filedata.size(); i++){
		if(lineBreakCount == fileInfo[fileTabSelected].cursorY){
			fileOffset = i;
			vertEOF = false;
			break;
		}
		if(filedata[i] == 10){ lineBreakCount++; }
	}
	
	fileInfo[fileTabSelected].cursorOffset = fileOffset;

	// Make cursorX stop at end of current line
	int lineEnd = filedata.size();
	bool lineEOF = false;
	for(int i = fileOffset; i < filedata.size(); i++){
		if(filedata[i] == 10){ 
			lineEnd = i - fileOffset; 
			break; 
		}
	}
	if(lineEnd == filedata.size()){
		lineEnd -= fileOffset;
		if(lineEnd < 0) { lineEnd = 0; }
		lineEOF = true;
	}
	if(fileInfo[fileTabSelected].cursorX >= CU::FileLineMax){
		fileInfo[fileTabSelected].cursorX = lineEnd;
	}
	fileInfo[fileTabSelected].cursorOffset += fileInfo[fileTabSelected].cursorX;

	// TODO:
	// Handle when cursor is past window width
	// winWidth-(maxLineNumWidth+3)

	if(fileInfo[fileTabSelected].cursorMovedDir>=2){
		if(fileInfo[fileTabSelected].cursorX > lineEnd){
			fileInfo[fileTabSelected].cursorX = lineEnd;
		}
	}else{
		if(fileInfo[fileTabSelected].cursorX > lineEnd){
			if(lineEOF){
				fileInfo[fileTabSelected].cursorX = lineEnd;
			}else{
				fileInfo[fileTabSelected].cursorX = 0;
				fileInfo[fileTabSelected].cursorY += 1;
			}
		}
	}
	if(vertEOF){
		if(fileInfo[fileTabSelected].cursorY>0){
			fileInfo[fileTabSelected].cursorY = lineBreakCount;
		}
	}
	if(fileInfo[fileTabSelected].cursorX < 0){
		if(fileInfo[fileTabSelected].cursorY > 0){
			// Make cursorX stop at end of previous line
			fileInfo[fileTabSelected].cursorY -= 1;

			// Offset scrollY number of line breaks
			lineBreakCount = 0;
			fileOffset = filedata.size();
			vertEOF = true;
			for(int i = 0; i < filedata.size(); i++){
				if(lineBreakCount == fileInfo[fileTabSelected].cursorY){
					fileOffset = i;
					vertEOF = false;
					break;
				}
				if(filedata[i] == 10){ lineBreakCount++; }
			}

			fileInfo[fileTabSelected].cursorOffset = fileOffset;

			int lineEnd = filedata.size();
			bool lineEOF = false;
			for(int i = fileOffset; i < filedata.size(); i++){
				if(filedata[i] == 10){ 
					lineEnd = i - fileOffset; 
					break; 
				}
			}
			if(lineEnd == filedata.size()){
				lineEnd -= fileOffset;
				lineEOF = true;
			}

			if(fileInfo[fileTabSelected].cursorX >= CU::FileLineMax){
				fileInfo[fileTabSelected].cursorX = lineEnd;
			}

			fileInfo[fileTabSelected].cursorOffset += fileInfo[fileTabSelected].cursorX;

			fileInfo[fileTabSelected].cursorX = lineEnd;
		}else{
			fileInfo[fileTabSelected].cursorX = 0;
		}
	}
/*	if(fileInfo[fileTabSelected].cursorX > 100){
		fileInfo[fileTabSelected].cursorX = 100;
	}*/
	while(fileInfo[fileTabSelected].cursorY < fileInfo[fileTabSelected].scrollY){
		fileInfo[fileTabSelected].scrollY -= 1;
		if(fileInfo[fileTabSelected].scrollY < 0){
			fileInfo[fileTabSelected].scrollY = 0;
			fileInfo[fileTabSelected].cursorY = 0;
			break;
		}
	}
	if(fileInfo[fileTabSelected].cursorY < 0){
		fileInfo[fileTabSelected].cursorY = 0;
		fileInfo[fileTabSelected].cursorX = 0;
	}

	while(fileInfo[fileTabSelected].cursorY > (winHeight+fileInfo[fileTabSelected].scrollY)){
		fileInfo[fileTabSelected].scrollY += 1;
	}

//	if(fileInfo[fileTabSelected].scrollY > 100){
//		fileInfo[fileTabSelected].scrollY = 100;
//	}
};

void CUEditor::doEditor(CU::keyCode key){

	// Get the character keys
	if((int)key >= (int)CU::keyCode::k_space && (int)key <= (int)CU::keyCode::k_grave){
		// Write that key to the file
		std::vector<char> &filedata = fileList[fileTabSelected].getData();
		filedata.insert(filedata.begin()+fileInfo[fileTabSelected].cursorOffset,(char)key);
		// Move the cursor
		fileInfo[fileTabSelected].cursorX += 1;
		fileInfo[fileTabSelected].cursorMovedDir = 1;
		fileList[fileTabSelected].modified = true;
	}
	if(key == CU::keyCode::k_enter){
		// Add a newline to the file
		std::vector<char> &filedata = fileList[fileTabSelected].getData();
		filedata.insert(filedata.begin()+fileInfo[fileTabSelected].cursorOffset,(char)10);
		// Move the cursor
		fileInfo[fileTabSelected].cursorX += 1;
		fileInfo[fileTabSelected].cursorMovedDir = 1;
		fileList[fileTabSelected].modified = true;
	}
	if(key == CU::keyCode::k_backspace){
		// Remove a char from the file
		std::vector<char> &filedata = fileList[fileTabSelected].getData();

		fileInfo[fileTabSelected].cursorX -= 1;
		fileInfo[fileTabSelected].cursorMovedDir = 0;
		if(fileInfo[fileTabSelected].cursorX < 0){
			fileInfo[fileTabSelected].cursorY -= 1;
			fileInfo[fileTabSelected].cursorX = CU::FileLineMax;
			fileInfo[fileTabSelected].cursorMovedDir = 2;
			fixCursor();

			if(filedata.size()&&fileInfo[fileTabSelected].cursorOffset<filedata.size()){
				filedata.erase(filedata.begin()+fileInfo[fileTabSelected].cursorOffset);
			}
			fileList[fileTabSelected].modified = true;
		}else{
			fileInfo[fileTabSelected].cursorOffset -= 1;
			if(filedata.size()&&fileInfo[fileTabSelected].cursorOffset >= 0 && fileInfo[fileTabSelected].cursorOffset < filedata.size()){
				filedata.erase(filedata.begin()+fileInfo[fileTabSelected].cursorOffset);
			}
			fileInfo[fileTabSelected].cursorOffset += 1;
			fileList[fileTabSelected].modified = true;
		}
	}
	if(key == CU::keyCode::s_delete){
		// Remove a char from the file
		std::vector<char> &filedata = fileList[fileTabSelected].getData();
		if(filedata.size()&&fileInfo[fileTabSelected].cursorOffset<filedata.size()){
			filedata.erase(filedata.begin()+fileInfo[fileTabSelected].cursorOffset);
		}
		fileList[fileTabSelected].modified = true;
	}

	if(key == CU::keyCode::s_left){
		fileInfo[fileTabSelected].cursorX -= 1;
		fileInfo[fileTabSelected].cursorMovedDir = 0;
	}
	if(key == CU::keyCode::s_right){
		fileInfo[fileTabSelected].cursorX += 1;
		fileInfo[fileTabSelected].cursorMovedDir = 1;
	}
	if(key == CU::keyCode::s_up){
		fileInfo[fileTabSelected].cursorY -= 1;
		fileInfo[fileTabSelected].cursorMovedDir = 2;
	}
	if(key == CU::keyCode::s_down){
		fileInfo[fileTabSelected].cursorY += 1;
		fileInfo[fileTabSelected].cursorMovedDir = 3;
	}

	if(key == CU::keyCode::s_pg_up){
		fileInfo[fileTabSelected].cursorY -= 32;
		fileInfo[fileTabSelected].cursorMovedDir = 2;
	}
	if(key == CU::keyCode::s_pg_down){
		fileInfo[fileTabSelected].cursorY += 32;
		fileInfo[fileTabSelected].cursorMovedDir = 3;
	}

	if((int)key & (int)CU::keyCode::c_ctrl){
		if((int)key & (int)CU::keyCode::s_home){
			fileInfo[fileTabSelected].cursorY = 0;
			fileInfo[fileTabSelected].cursorX = 0;
			fileInfo[fileTabSelected].cursorMovedDir = 0;
		}
		if((int)key & (int)CU::keyCode::s_end){
			fileInfo[fileTabSelected].cursorY = 999999;
			fileInfo[fileTabSelected].cursorMovedDir = 0;
		}
	}


	// Fix the cursor
	fixCursor();

	// Handle interupts
	handleInt();
};

void CUEditor::drawEditor(){
	int winX = 1;
	int winY = 2;
	int winWidth = videoDriver.getWidth()-2;
	int winHeight = videoDriver.getHeight()-3;

	// Draw the background
	videoDriver.drawBar(winX,winY,winWidth,winHeight, ' ', settings.editor_fg_color, settings.editor_bg_color);
	// Draw a field
	videoDriver.drawBox(winX,winY,winWidth,winHeight, CU::BlockType::SINGLE, settings.editor_fg_color, settings.editor_bg_color);
	// Draw a title bar
	videoDriver.drawBar(winX,winY,winWidth, 1, ' ', settings.menu_bar_fg_color, settings.menu_bar_bg_color);

	// TODO:
	// Make this display arrows when too many tabs are open
	// Handle tab scrolling

	int fileCount = 0;
	int tabWidth = 20;
	int max_files_on_line = videoDriver.getWidth() / tabWidth;
	for(int e = 0; e < fileList.size() && e < max_files_on_line; e++){
		char LBChar = (e==fileTabSelected)?'<':'[';
		char RBChar = (e==fileTabSelected)?'>':']';
		for(int i = 0; i < fileList.size();  i ++){
			if(e == i) continue;
			if(fileList[e].getName() == fileList[i].getName()){
				fileCount += 1;
				break;
			}
		}
		std::string title_str;
		title_str = fileList[e].getName();

		title_str = CU::fileizeString(title_str, (tabWidth-8),4);
		if(fileList[e].modified){
			title_str += " *";
		}
		if(fileCount){
			title_str += "("+std::to_string(fileCount)+")";
		}
		videoDriver.writeStr(LBChar+title_str+RBChar, winX + (e * tabWidth)+(tabWidth>>1)-(title_str.length()>>1), winY);
		videoDriver.writeStr((e==fileTabSelected)?"[":"|",winX+(e*tabWidth),winY);
		videoDriver.writeStr((e==fileTabSelected)?"]":"|",winX+(e*tabWidth)+tabWidth-1,winY);
	}

	// Draw the file in an editor

	// Draw every line
	int lineCount = 0;
	int lineBroke = -1;

	int maxLineNumWidth = 6;

	unsigned int fileOffset = 0;
	int eoffound = -1;
	int eofdone = 0;

	std::vector<char> &filedata = fileList[fileTabSelected].getData();

	// Offset scrollY number of line breaks
	int lineBreakCount = 0;

	fileOffset = -1;
	for(int i = 0; i < filedata.size(); i++){
		if(lineBreakCount == fileInfo[fileTabSelected].scrollY){
			if(fileOffset == -1){
				fileOffset = i;
			}
		}
		if(filedata[i] == 13){ i++; }
		if(filedata[i] == 10){ lineBreakCount++; }
	}
	if(fileOffset==-1){
		if(lineBreakCount <= fileInfo[fileTabSelected].scrollY){
			// We scrolled too far then
			fileOffset = filedata.size();
		}
	}	

	for(int line_idx = 0; line_idx < winHeight-2; line_idx ++){
		// Figure out the text to print on that line
		std::string lineString = "";
		if(eoffound==-1){
			if(lineBroke == 2){
				lineBroke = -1;
			}
			int i = 0;
			for(i = 0; i < winWidth-(maxLineNumWidth+3); i++){
				if(fileOffset+i >= filedata.size()){
					eoffound = lineCount;
					eofdone = 1;
					break;
				}
				if(filedata.at(fileOffset+i) == 10){
					if(lineBroke == 0)
						lineBroke = 2;
					if(lineBroke == 1)
						lineBroke = 2;
					eofdone = 0;
					i++;
					break;
				}
				if(filedata.at(fileOffset+i) < 32 || filedata.at(fileOffset+i) >= 127){
					lineString.push_back('?');
				}else{
					lineString.push_back(filedata.at(fileOffset+i));
				}
			}
			if(lineBroke == 0){
				lineBroke = 1;
			}
			if(i == winWidth-(maxLineNumWidth+3)){
				if(lineBroke == -1){
					lineBroke = 0;
				}
			}
			fileOffset += i;
		}

		std::string lineNumStr = CU::to_stringc(lineCount+fileInfo[fileTabSelected].scrollY,'.',maxLineNumWidth);
		if(lineString.length()==0 && eoffound >= 0 && eoffound < lineCount){//} && (eofdone || (eoffound > 0 && eoffound <= lineCount))){
			lineNumStr = std::string(maxLineNumWidth,'-');
			lineBroke = -1;
		}else if(lineBroke>0){
			lineNumStr = std::string(maxLineNumWidth,'.');
			lineCount -= 1;
		}
		lineCount += 1;
		
		videoDriver.writeStr(lineNumStr,winX+1,winY+1+line_idx,settings.editor_line_fg_color, settings.editor_line_bg_color);
		videoDriver.writeStr(lineString,winX+maxLineNumWidth+2,winY+1+line_idx,settings.editor_fg_color, settings.editor_bg_color);
		videoDriver.setCurPos(winX+maxLineNumWidth+1,winY+1+line_idx);
		videoDriver.writeBChar(CU::BlockChar::DVBAR);
	}
	if(cursorBlink){
		videoDriver.setCurPos(fileInfo[fileTabSelected].cursorX+winX+maxLineNumWidth+2-fileInfo[fileTabSelected].scrollX,fileInfo[fileTabSelected].cursorY+winY+1-fileInfo[fileTabSelected].scrollY);
		videoDriver.writeBChar(CU::BlockChar::VBAR,settings.editor_fg_color, settings.editor_bg_color);
	}
};

void CUEditor::handleInt(){
	switch((CUBreakType)videoDriver.halted()){
		case CUBreakType::SAVE_EXIT:
			// Save the project and exit
			shutdown();
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

