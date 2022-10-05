/*

	CU Edit

	Written by: Jonathan Clevenger

	10/3/2022
*/

#include "cudriver.h"

class CUEditor {
private:
	CU::Driver videoDriver;
	bool running = false;
public:
	CUEditor();
	void run();
	void drawGUI();
	void shutdown();

};

