
#include "master_pch.h"
#include "master_header.h"

namespace war
{

float viewport_w = 320;
float viewport_h = 240;
float ui_w = 640;
float ui_h = 480;

std::unique_ptr<log_file> g_logfile = nullptr;
std::unique_ptr<game> g_base_game = nullptr;
std::unique_ptr<engine> g_engine = nullptr;
std::unique_ptr<ui_mgr> g_ui = nullptr;

float lerp( float a, float b, float lerp_factor )
{
	float result = ( ( 1.f - lerp_factor ) * a ) + ( lerp_factor * b );
	return result;
}

vec2 lerp( vec2 a, vec2 b, float lerp_factor )
{
	vec2 result = vec2::zero;

	result.x = ( ( 1.f - lerp_factor ) * a.x ) + ( lerp_factor * b.x );
	result.y = ( ( 1.f - lerp_factor ) * a.y ) + ( lerp_factor * b.y );

	return result;
}

}

// yoinked from: http://stevendebock.blogspot.com/2013/07/nvidia-optimus.html
// https://www.reddit.com/r/gamedev/comments/bk7xbe/psa_for_anyone_developing_a_gameengine_in_c/
//
// these lines fix the problem where laptops will sometimes select their
// integrated graphics cards instead of the nvidia/AMD one. this feels weird and
// fragile and hacky but it's certainly better than making people go into their
// nvidia control panel and changing their GPU selection setting.

extern "C"
{
	// nvidia
	__declspec( dllexport ) DWORD NvOptimusEnablement = 0x00000001;

	// AMD
	__declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}
