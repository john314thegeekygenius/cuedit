/*

	CU Edit

	Written by: Jonathan Clevenger

	10/3/2022
*/
#pragma once

namespace CU {

enum class EXEFileTypes {
	Broken = -2,
	Unknown = -1,
	Directory = 0,
	Symbolic = 1,
	Normal = 2,
	EXE = 3,
	ELF = 4,
};

typedef struct fileInfo {
	int cursorX = 0;
	int cursorY = 0;
	int scrollX = 0;
	int scrollY = 0;
	int cursorMovedDir = 0; // 0-left 1-right 2-up 3-down
	int cursorOffset = 0;
}fileInfo;

const int FileLineMax = 999999;

enum class FileAccess {
	READ = 0,
	WRITE = 1,
};

};

class CUEditor {
private:
	CU::Driver videoDriver;
	CUSettings settings;
	bool running = false;
	
	std::vector<CUMenu> menuList;
	std::vector<CU::File> fileList;
	std::vector<CU::fileInfo> fileInfo;
	int fileTabSelected = 0;
	std::vector<std::string> clipboard;
	std::string selected;

	bool cursorBlink = false;
	uint64_t cursorTime = 0;

	CU::Project project;

public:
	bool MainMenuTabsSelected = false;
	bool EditorSelected = false;
	bool TerminalSelected = false;
	bool EditorOpen = false;
	bool TerminalOpen = false;

public:
	void init();
	void loadSettings();
	void halt(int e);
	void run();
	void close();
	void handleInt();
	void drawGUI();
	void fixCursor();
	void doEditor(CU::keyCode key);
	void closeCurrentFile();
	void drawEditor();
	void shutdown();

	void createFile();

	std::string openFileDialog(std::string name, CU::FileAccess access_type);
	std::string openFile();
	std::string saveFile();
	void ErrorMsgBox(std::string error);

};


