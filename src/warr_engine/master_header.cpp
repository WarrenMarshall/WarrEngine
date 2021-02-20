
#include "master_pch.h"
#include "master_header.h"

float v_window_w = 320;
float v_window_h = 240;
float ui_window_w = 640;
float ui_window_h = 480;

std::unique_ptr<w_logfile> logfile = nullptr;
std::unique_ptr<w_game> base_game = nullptr;
std::unique_ptr<w_engine> engine = nullptr;

// yoinked from : http://stevendebock.blogspot.com/2013/07/nvidia-optimus.html
// also : https://www.reddit.com/r/gamedev/comments/bk7xbe/psa_for_anyone_developing_a_gameengine_in_c/
//
// this line fixes the problem where laptops would select their integrated
// graphics cards instead of the nvidia/AMD one.
//
// it's certainly better than making people go into their nvidia
// control panel and changing their GPU selection setting.

extern "C"
{
	// nvidia
	__declspec( dllexport ) DWORD NvOptimusEnablement = 0x00000001;

	// AMD
	__declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}
