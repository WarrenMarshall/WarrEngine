
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

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

	engine->opengl->refresh_primitive_sizes();

	glViewport(
		static_cast<int>( engine->window->viewport_pos_sz.x ), static_cast<int>( engine->window->viewport_pos_sz.y ),
		static_cast<int>( engine->window->viewport_pos_sz.w ), static_cast<int>( engine->window->viewport_pos_sz.h )
	);
}	

// ----------------------------------------------------------------------------

bool w_window::init( const std::string& title )
{
	this->base_title = title;

	// Initialize GLFW
	if( !glfwInit() )
	{
		log_error( "%s : GLFW failed to init", __FUNCTION__ );
	}

	// Create a windowed mode window and its OpenGL context

	primary_monitor = glfwGetPrimaryMonitor();
	vidmode = glfwGetVideoMode( primary_monitor );

	const char* monitor_name = glfwGetMonitorName( primary_monitor );
	log_msg( "%s : Using monitor [%s - %dx%d]", __FUNCTION__, monitor_name, vidmode->width, vidmode->height );

	float desktop_w = vidmode->width - v_window_hw;
	float desktop_h = vidmode->height - v_window_hh;

	// figure out a maximal size for the window to be to fill the screen neatly and the
	// window to be positioned in the center of the screen.
	int wdiv = static_cast<int>( std::floorf( desktop_w / static_cast<float>( v_window_w ) ) );
	int hdiv = static_cast<int>( std::floorf( desktop_h / static_cast<float>( v_window_h ) ) );
	int div = w_min<int>( wdiv, hdiv );

	w_rect window_pos( 0.0f, 0.0f, v_window_w * div, v_window_h * div );
	window_pos.x = ( vidmode->width - window_pos.w ) / 2;
	window_pos.y = ( vidmode->height - window_pos.h ) / 2;

	window = glfwCreateWindow(
		static_cast<int>( window_pos.w ), static_cast<int>( window_pos.h ),
		this->base_title.c_str(), NULL, NULL
	);

	if( !window )
	{
		log_error( "%s : failed to create window", __FUNCTION__ );
		glfwTerminate();
	}

	glfwSetWindowPos( window, static_cast<int>( window_pos.x ), static_cast<int>( window_pos.y ) );

	// whenever the frame buffer changes size, we need to know so we can adjust
	// the opengl viewport
	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

	viewport_pos_sz = w_rect( 0, 0, window_pos.w, window_pos.h );

	// Make the window's context current
	glfwMakeContextCurrent( window );

	// 0 = no v-sync
	// 1 = v-sync on
	glfwSwapInterval( 0 );

	engine->opengl->refresh_primitive_sizes();

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
		glfwGetWindowPos( window, &save_windowed_state.x, &save_windowed_state.y );
		glfwGetWindowSize( window, &save_windowed_state.w, &save_windowed_state.h );

		glfwSetWindowMonitor( window, primary_monitor,
			0, 0,
			vidmode->width, vidmode->height, vidmode->refreshRate );
	}
	else
	{
		glfwSetWindowMonitor( window, NULL,
			save_windowed_state.x, save_windowed_state.y,
			save_windowed_state.w, save_windowed_state.h, 0 );
	}
}

void w_window::set_title( const std::string& title )
{
	glfwSetWindowTitle( window, title.c_str() );
}
