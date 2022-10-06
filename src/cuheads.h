/*

	CU Edit

	Written by: Jonathan Clevenger

	10/3/2022
*/
#pragma once


#define CU_SETTINGS_FILE_NAME "cuedit.settings"
#define CU_EDIT_MIN_WIDTH 40

#include "cudriver.h"

typedef struct CUSettings {
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
	
} CUSettings;

#include "cumenu.h"

#include "cuedit.h"

