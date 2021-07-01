
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void glfw_error_callback( [[maybe_unused]] int error_code, const char* description )
{
	log_fatal( "\"{}\"", description );
}

void framebuffer_size_callback( [[maybe_unused]] GLFWwindow* window, int width, int height )
{
	// This chunk of code:
	//
	// - figure out how wide and high the viewport should be to fill the real window
	// - figures out how far to offset it on either X or Y to center it within the window
	//
	// This allows us to work with a set size screen (viewport) and not have to worry about
	// the user changing the window to some weird size.

	g_engine->window.viewport_pos_sz = rect( 0.f, 0.f, (float)( width ), (float)( height ) );

	float w_ratio = width / viewport_w;
	float h_ratio = height / viewport_h;

	if( w_ratio > h_ratio )
	{
		g_engine->window.viewport_pos_sz.w = viewport_w * h_ratio;
		g_engine->window.viewport_pos_sz.x = ( width - g_engine->window.viewport_pos_sz.w ) / 2.f;
	}
	else
	{
		g_engine->window.viewport_pos_sz.h = viewport_h * w_ratio;
		g_engine->window.viewport_pos_sz.y = ( height - g_engine->window.viewport_pos_sz.h ) / 2.f;
	}
}

void focus_change_callback( [[maybe_unused]] GLFWwindow* window, int focused )
{
	focused ? g_engine->resume() : g_engine->pause();
}

// ----------------------------------------------------------------------------

rect os_window::compute_max_window_size_for_desktop()
{
	float desktop_w = vidmode->width - viewport_hw;
	float desktop_h = vidmode->height - viewport_hh;

	// figure out a maximal size for the window to be to fill the screen neatly and the
	// window to be positioned in the center of the screen.

	auto wdiv = (int)( std::floorf( desktop_w / (float)( viewport_w ) ) );
	auto hdiv = (int)( std::floorf( desktop_h / (float)( viewport_h ) ) );
	int div = glm::min( wdiv, hdiv );

	rect window_pos( 0.f, 0.f, viewport_w * div, viewport_h * div );
	window_pos.x = ( vidmode->width - window_pos.w ) / 2;
	window_pos.y = ( vidmode->height - window_pos.h ) / 2;

	viewport_pos_sz = rect( 0.f, 0.f, window_pos.w, window_pos.h );

	return window_pos;
}

bool os_window::init()
{
	glfwInit();

	glfwSetErrorCallback( glfw_error_callback );

	log( "GLFW Version : {}", glfwGetVersionString() );

	primary_monitor = glfwGetPrimaryMonitor();
	vidmode = glfwGetVideoMode( primary_monitor );

	const char* monitor_name = glfwGetMonitorName( primary_monitor );
	log( "Primary monitor [{} - {}x{}]", monitor_name, vidmode->width, vidmode->height );

	rect window_pos = compute_max_window_size_for_desktop();

	//glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	//glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	//glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	glfw_window = glfwCreateWindow(
		(int)( window_pos.w ), (int)( window_pos.h ),
		"WarrEngine", nullptr, nullptr
	);

	if( !glfw_window )
	{
		log_fatal( "Failed to create window" );
	}

	glfwSetWindowPos( glfw_window,
		(int)( window_pos.x ),
		(int)( window_pos.y ) );

	glfwSetFramebufferSizeCallback( glfw_window, framebuffer_size_callback );
	glfwSetWindowFocusCallback( glfw_window, focus_change_callback );

	glfwMakeContextCurrent( glfw_window );

	glfwSetInputMode( glfw_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE );
	glfwSetInputMode( glfw_window, GLFW_STICKY_KEYS, GLFW_TRUE );

	return true;
}

void os_window::deinit()
{
	glfwDestroyWindow( glfw_window );
}

void os_window::toggle_fullscreen()
{
	toggle_bool( is_fullscreen );

	if( is_fullscreen )
	{
		// when toggling to fullscreen, save the current window state so it
		// can be restored later.
		glfwGetWindowPos( glfw_window, &save_windowed_state.x, &save_windowed_state.y );
		glfwGetWindowSize( glfw_window, &save_windowed_state.w, &save_windowed_state.h );

		glfwSetWindowMonitor( glfw_window, primary_monitor,
			0, 0,
			vidmode->width, vidmode->height, vidmode->refreshRate );
	}
	else
	{
		// dropping back
		glfwSetWindowMonitor( glfw_window, nullptr,
			save_windowed_state.x, save_windowed_state.y,
			save_windowed_state.w, save_windowed_state.h, 0 );
	}
}

void os_window::set_title( std::string_view title )
{
	glfwSetWindowTitle( glfw_window, title.data() );
}

void os_window::set_mouse_mode( e_mouse_mode mouse_mode )
{
	this->mouse_mode = mouse_mode;
	refresh_mouse_mode();
}

void os_window::refresh_mouse_mode()
{
	switch( mouse_mode )
	{
		case mouse_mode::os:
		{
			glfwSetInputMode( glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
			break;
		}

		case mouse_mode::custom:
		{
			glfwSetInputMode( glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
			break;
		}

		case mouse_mode::locked:
		{
			glfwSetInputMode( glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
			break;
		}

		default:
		{
			// unknown mouse mode
			assert( false );
		}
	}
}

}
