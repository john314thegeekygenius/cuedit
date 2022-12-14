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

enum class FileType {
	BINARY = 0,
	TEXT = 1,
	C = 2,
	CPP = 3,
};

typedef struct fileInfo {
	int cursorX = 0;
	int cursorY = 0;
	int scrollX = 0;
	int scrollY = 0;
	int cursorMovedDir = 0; // 0-left 1-right 2-up 3-down
	int cursorOffset = 0;
	FileType type;
}fileInfo;

const int FileLineMax = 999999;

enum class FileAccess {
	READ = 0,
	WRITE = 1,
};

enum class ParseType {
	TEXT = 0,
	TYPE = 1,
	FUNCTION = 2,
	VALUE = 3,
	DEFINED = 4,
	CONST = 5,
	CLASS = 6,
	STRING = 7,
	ARITHMATIC = 8,
};

typedef struct ParseStringType {
	std::string str;
	ParseType ptype;
}ParseStringType;

};

class CUEditor {
private:
	CU::Driver videoDriver;
	CUSettings settings;
	bool running = false;
	bool shuttingDown = false;
	
	std::vector<CUMenu> menuList;
	std::vector<CU::File> fileList;
	std::vector<CU::fileInfo> fileInfo;
	int fileTabSelected = 0;
	std::vector<std::string> clipboard;
	std::string selected;
	std::string currentFileName;

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
	void init(int argc, char *argv[]);
	void loadSettings();
	void halt(int e);
	void run();
	void close();
	void handleInt();
	void drawGUI();
	void fixCursor();
	void doEditor(CU::keyCode key);
	bool closeCurrentFile();
	void drawEditor();
	void shutdown();

	void createFile(std::string name = "Untitled");
	void reloadCurrentFile();
	void loadArgFiles(int argc, char *argv[]);

	void generateIntelliFile(std::vector<CU::ParseStringType> &parsed_strings, std::vector<char> &s_data);
	void writeParsedString(CU::ParseStringType parsed_string, int x, int y);

	std::string openFileDialog(std::string name, CU::FileAccess access_type);
	std::string openFile();
	std::string loadFile(std::string load_path,bool showError = true);
	std::string saveFile(bool overwrite);
	void ErrorMsgBox(std::string error);
	bool AreYouSure(std::string warning);
	std::string getUserString(std::string msg, int maxLength = -1);

};


