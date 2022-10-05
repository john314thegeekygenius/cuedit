/*

	CU Edit Driver

	Written by: Jonathan Clevenger

	10/3/2022

*/


#include "cudriver.h"

CU::Driver::Driver(){
	// Make sure it's a valid terminal for color
	colorSupported = false;

	if(isatty(0)){
		// Yay!
		colorSupported = true;
	}
	
	// Update the variables
	updateDriver();

	// Hide the "pysical" cursor
	std::cout << "\x1B[?25l" << std::flush;
};

CU::Driver::~Driver(){
	// Shutdown the driver
	shutdownDriver();
};


void CU::Driver::shutdownDriver(){
	// Reset color stuff
	std::cout << "\033[0m" << std::flush;

	// Clear the screen
	std::cout << "\033[3J\033[2J\033[H"<< std::flush;

	// Show the "pysical" cursor
	std::cout << "\x1B[?25h" << std::flush;
};

void CU::Driver::updateDriver(){
	// Get the screen size
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	scrWidth = w.ws_col;
	scrHeight = w.ws_row;
	
	scrSize = scrWidth*scrHeight;

	// Clear the buffer
	scrBuffer.clear();
	
	// Set the buffer
	scrBuffer.reserve(scrWidth*scrHeight*2);
	
	// Set the cursor to a valid location
	setCurPos(0,0);
};

int CU::Driver::getWidth(){
	return scrWidth;
};

int CU::Driver::getHeight(){
	return scrHeight;
};

void CU::Driver::setCurPos(int x,int y){
	scrCurPos[0] = x;
	scrCurPos[1] = y;
	if(x < 0 ){
		scrCurPos[0] = 0;
	}
	if(x >= scrWidth ){
		scrCurPos[0] = scrWidth-1;
	}
	if(y < 0 ){
		scrCurPos[1] = 0;
	}
	if(y >= scrHeight ){
		scrCurPos[1] = scrHeight-1;
	}
};

//void CU::Driver::hideCursor(){};

//void CU::Driver::showCursor(){};

void CU::Driver::setBGColor(CU::Color c){
	scrBuffer[scrSize+(scrCurPos[1]*scrWidth) + scrCurPos[0]] &= 0xF0;
	scrBuffer[scrSize+(scrCurPos[1]*scrWidth) + scrCurPos[0]] |= (char)c;
};

void CU::Driver::setFGColor(CU::Color c){
	scrBuffer[scrSize+(scrCurPos[1]*scrWidth) + scrCurPos[0]] &= 0x0F;
	scrBuffer[scrSize+(scrCurPos[1]*scrWidth) + scrCurPos[0]] |= (char)c<<8;
};

void CU::Driver::clear(){
	if(scrBuffer.size()){
		std::fill_n(scrBuffer.begin(), scrSize, 0);
		std::fill_n(scrBuffer.begin()+scrSize, scrSize, 0);
	//	std::fill(scrBuffer.begin(), scrBuffer.end(), 0);
	//	std::fill(scrBuffer.begin()+scrSize, scrBuffer.end(), 7);
	}
};

void CU::Driver::flush(){
	// set the cursor position
	std::cout << "\x1b[3J\033[H";
	
	// output the buffer to the screen
	for(int y = 0; y < scrHeight; y++){
		for(int x = 0; x < scrWidth; x++){
			if(colorSupported){
				int bg = 40+(+scrBuffer[scrSize+(y*scrWidth) + x]&0x0F);
				int fg = 30+(+scrBuffer[scrSize+(y*scrWidth) + x]>>8);
				if(bg > 47){
					bg -= 8; // There is no bright backgrounds?
				}
				std::cout << "\x1B[" << bg << "m";
				if(fg > 37){
					fg += 52;
				}
				std::cout << "\x1B[" << fg <<"m";
			}

			int C = scrBuffer[(y*scrWidth) + x];
			if(C & 0x8000){
				std::cout << CU::UNIBlockChars[C&0xFF];
			}else{
				std::cout << (char)(C?C:' ');
			}
		}
	}

};

void CU::Driver::writeBChar(CU::BlockChar c){
	scrBuffer[(scrCurPos[1]*scrWidth) + scrCurPos[0]] = (int)c;	
};

void CU::Driver::drawBox(int x,int y,int w,int h,CU::BlockType t, CU::Color fg, CU::Color bg){

};

void CU::Driver::drawBar(int x,int y,int w,int h, int floodchar, CU::Color fg, CU::Color bg){
	for(int i = y; i < y+h; i++){
		for(int e = x; e < x+w; e++){
			scrBuffer[(i*scrWidth) + e] = (int)floodchar;	
			scrBuffer[scrSize+(i*scrWidth) + e] = (((int)fg)<<8) | (int)bg;
		}
	}
};

void CU::Driver::writeStr(std::string s, int x,int y){
};

int CU::Driver::kbhit(void) {
	// From: https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed
    static bool initflag = false;
    static const int STDIN = 0;

    if (!initflag) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initflag = true;
    }

    int nbbytes;
    ioctl(STDIN, FIONREAD, &nbbytes);  // 0 is STDIN
    return nbbytes;
}

char CU::Driver::getch() {
	// From: https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
			perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
			perror ("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror ("tcsetattr ~ICANON");
	return (buf);
};

