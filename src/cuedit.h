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
	int cursorOffset = 0;
}fileInfo;

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
	void drawEditor();
	void shutdown();

	void createFile();

	std::string openFile();
	void ErrorMsgBox(std::string error);

};


