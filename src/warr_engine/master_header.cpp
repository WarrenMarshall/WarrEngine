
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "master_pch.h"
#include "master_header.h"

float v_window_w = 320;
float v_window_h = 240;
float ui_canvas_w = 640;
float ui_canvas_h = 480;

std::unique_ptr<w_logfile> logfile = nullptr;
std::unique_ptr<w_engine> engine = nullptr;
std::unique_ptr<w_game> base_game = nullptr;

// yoinked from : http://stevendebock.blogspot.com/2013/07/nvidia-optimus.html
//
// this line fixes the problem where laptops would select their integrated
// graphics cards instead of the nvidia one. i don't love this but it's
// simple and it works, so it's fine for now.
//
// it's certainly better than making people go into their nvidia
// control panel and changing their GPU selection setting.

extern "C" {
	_declspec( dllexport ) DWORD NvOptimusEnablement = 0x00000001;
}
