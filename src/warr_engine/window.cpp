
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void glfw_error_callback( int error_code, const char* description )
{
	log_error( "\"{}\"", description );
}

void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
	// This chunk of code:
	//
	// - figure out how wide and high the viewport should be to fill the real window
	// - figures out how far to offset it on either X or Y to center it within the window
	//
	// This allows us to work with a set size screen (v_window) and not have to worry about
	// the user changing the window to some weird size.

	engine->window->viewport_pos_sz = w_rect( 0.0f, 0.0f, static_cast<float>( width ), static_cast<float>( height ) );

	float w_ratio = width / v_window_w;
	float h_ratio = height / v_window_h;

	if( w_ratio > h_ratio )
	{
		engine->window->viewport_pos_sz.w = v_window_w * h_ratio;
		engine->window->viewport_pos_sz.x = ( width - engine->window->viewport_pos_sz.w ) / 2.0f;
	}
	else
	{
		engine->window->viewport_pos_sz.h = v_window_h * w_ratio;
		engine->window->viewport_pos_sz.y = ( height - engine->window->viewport_pos_sz.h ) / 2.0f;
	}

	glViewport(
		static_cast<int>( engine->window->viewport_pos_sz.x ), static_cast<int>( engine->window->viewport_pos_sz.y ),
		static_cast<int>( engine->window->viewport_pos_sz.w ), static_cast<int>( engine->window->viewport_pos_sz.h )
	);
}

void focus_change_callback( GLFWwindow* window, int focused )
{
	engine->set_pause( ( focused == 0 ) );
}

// ----------------------------------------------------------------------------

w_rect w_window::compute_max_window_size_for_desktop()
{
	float desktop_w = vidmode->width - v_window_hw;
	float desktop_h = vidmode->height - v_window_hh;

	// figure out a maximal size for the window to be to fill the screen neatly and the
	// window to be positioned in the center of the screen.
	auto wdiv = static_cast<int>( std::floorf( desktop_w / static_cast<float>( v_window_w ) ) );
	auto hdiv = static_cast<int>( std::floorf( desktop_h / static_cast<float>( v_window_h ) ) );
	int div = glm::min( wdiv, hdiv );

	w_rect window_pos( 0.0f, 0.0f, v_window_w * div, v_window_h * div );
	window_pos.x = ( vidmode->width - window_pos.w ) / 2;
	window_pos.y = ( vidmode->height - window_pos.h ) / 2;

	viewport_pos_sz = w_rect( 0, 0, window_pos.w, window_pos.h );

	return window_pos;
}

bool w_window::init()
{
	// Initialize GLFW
	if( !glfwInit() )
	{
		log_error( "GLFW failed to init" );
	}

	glfwSetErrorCallback( glfw_error_callback );

	log( "GLFW Version : {}", glfwGetVersionString() );

	// Create a windowed mode window and its OpenGL context

	primary_monitor = glfwGetPrimaryMonitor();
	vidmode = glfwGetVideoMode( primary_monitor );

	const char* monitor_name = glfwGetMonitorName( primary_monitor );
	log( "Primary monitor [{} - {}x{}]", monitor_name, vidmode->width, vidmode->height );

	w_rect window_pos = compute_max_window_size_for_desktop();

	window = glfwCreateWindow(
		static_cast<int>( window_pos.w ), static_cast<int>( window_pos.h ),
		"WarrEngine", nullptr, nullptr
	);

	if( !window )
	{
		log_error( "Failed to create window" );
	}

	glfwSetWindowPos( window,
					  static_cast<int>( window_pos.x ),
					  static_cast<int>( window_pos.y ) );

	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
	glfwSetWindowFocusCallback( window, focus_change_callback );

	// Make the window's context current
	glfwMakeContextCurrent( window );

	glfwSetInputMode( window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE );
	glfwSetInputMode( window, GLFW_STICKY_KEYS, GLFW_TRUE );

	return true;
}

void w_window::deinit()
{
	glfwDestroyWindow( window );
}

void w_window::toggle_fullscreen()
{
	is_fullscreen = !is_fullscreen;

	if( is_fullscreen )
	{
		// when toggling to fullscreen, save the current window state so it
		// can be restored later.
		glfwGetWindowPos( window, &save_windowed_state.x, &save_windowed_state.y );
		glfwGetWindowSize( window, &save_windowed_state.w, &save_windowed_state.h );

		glfwSetWindowMonitor( window, primary_monitor,
			0, 0,
			vidmode->width, vidmode->height, vidmode->refreshRate );
	}
	else
	{
		// dropping back
		glfwSetWindowMonitor( window, nullptr,
			save_windowed_state.x, save_windowed_state.y,
			save_windowed_state.w, save_windowed_state.h, 0 );
	}
}

void w_window::set_title( const std::string_view title )
{
	glfwSetWindowTitle( window, title.data() );
}

void w_window::set_mouse_mode( e_mouse_mode mouse_mode )
{
	this->mouse_mode = mouse_mode;
	refresh_mouse_mode();
}

void w_window::refresh_mouse_mode()
{
	switch( mouse_mode )
	{
		case mouse_mode::os:
		{
			glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
		}
		break;

		case mouse_mode::custom:
		{
			glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
		}
		break;

		case mouse_mode::locked:
		{
			glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
		}
		break;

		default:
		{
			// unknown mouse mode
			assert( false );
		}
		break;
	}
}
