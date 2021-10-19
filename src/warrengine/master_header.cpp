
#include "master_pch.h"
#include "master_header.h"

namespace war
{

float_t viewport_w = 320;
float_t viewport_h = 240;
float_t ui_w = 320;
float_t ui_h = 240;
float_t final_pixel_w = 320;
float_t final_pixel_h = 240;

std::unique_ptr<Log_File> g_logfile = nullptr;
std::unique_ptr<Game> g_base_game = nullptr;
std::unique_ptr<Engine> g_engine = nullptr;
std::unique_ptr<UI_Mgr> g_ui = nullptr;

[[nodiscard]] float_t lerp( float_t a, float_t b, float_t lerp_factor )
{
	float_t result = ( ( 1.f - lerp_factor ) * a ) + ( lerp_factor * b );
	return result;
}

[[nodiscard]] Vec2 lerp( Vec2 a, Vec2 b, float_t lerp_factor )
{
	Vec2 result = Vec2::zero;

	result.x = ( ( 1.f - lerp_factor ) * a.x ) + ( lerp_factor * b.x );
	result.y = ( ( 1.f - lerp_factor ) * a.y ) + ( lerp_factor * b.y );

	return result;
}

}

// yoinked from:
// http://stevendebock.blogspot.com/2013/07/nvidia-optimus.html
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
