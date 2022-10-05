/*

	CU-Edit - A text based C++ IDE

	Written by: Jonathan Clevenger

	Started: 10/3/2022

	Change log:
	*
	*
*/

#include "cuedit.h"


int main(int argc, char *argv[]){
	CUEditor editor;

	editor.run();
	editor.shutdown();
	return 0;
};

CUEditor::CUEditor(){
	running = true;
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
	videoDriver.drawBar(1,1,videoDriver.getWidth()-2, videoDriver.getHeight()-2, ' ', CU::Color::WHITE, CU::Color::BLUE);
};

void CUEditor::shutdown(){
};
