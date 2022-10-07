/*

	CU Edit

	Written by: Jonathan Clevenger

	10/6/2022
*/
#pragma once

enum class CUBreakType {
	COPY = 2,
	UNDO = 20,
	SAVE_EXIT = 3,
//	EXIT = 3,
};

typedef struct CUSettings {
	// Color settings
	// -----------------
	// Background
	CU::Color background_color;
	std::string background_pattern;
	// Foreground
	CU::Color foreground_color;
	// Header
	CU::Color head_bg_color;
	CU::Color head_fg_color;
	// Menu
	CU::Color menu_bg_color;
	CU::Color menu_fg_color;

	CU::Color menu_bar_bg_color;
	CU::Color menu_bar_fg_color;


	// Application settings
	// -----------------
	CUBreakType handleBreak;

	void init(){
		background_color = CU::Color::BLACK;
		background_pattern = " ";
		foreground_color = CU::Color::L_WHITE;
		head_bg_color = CU::Color::BLACK;
		head_fg_color = CU::Color::L_WHITE;
		menu_bg_color = CU::Color::BLACK;
		menu_fg_color = CU::Color::L_WHITE;
		menu_bar_bg_color = CU::Color::BLACK;
		menu_bar_fg_color = CU::Color::L_WHITE;
		handleBreak = CUBreakType::COPY;
	};

	void copy(CUSettings &set){
		// Background
		background_color = set.background_color;
		background_pattern = set.background_pattern;
		// Foreground
		foreground_color = set.foreground_color;
		// Header
		head_bg_color = set.head_bg_color;
		head_fg_color = set.head_fg_color;
		// Menu
		menu_bar_bg_color = set.menu_bar_bg_color;
		menu_bar_fg_color = set.menu_bar_fg_color;

		menu_bg_color = set.menu_bg_color;
		menu_fg_color = set.menu_fg_color;

		handleBreak = set.handleBreak;
	};
} CUSettings;
