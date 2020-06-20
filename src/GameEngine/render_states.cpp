
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_render_state::w_render_state( bool auto_activation )
{
	this->auto_activation = auto_activation;
}

void w_render_state::reset()
{
	time_remaining_ms = static_cast<float>( time_interval_ms );
}

void w_render_state::update()
{
	time_remaining_ms -= engine->time->FTS_step_value_ms;
	time_remaining_ms = w_max<float>( time_remaining_ms, 0 );
}

// ----------------------------------------------------------------------------

rs_color::rs_color( float r, float g, float b, bool auto_activation )
	: w_render_state( auto_activation )
{
	this->new_color = w_color( r, g, b );
	if( auto_activation )
	{
		apply();
	}
}

rs_color::rs_color( w_color color, bool auto_activation )
	: w_render_state( auto_activation )
{
	this->new_color = color;
	if( auto_activation )
	{
		apply();
	}
}

rs_color::~rs_color()
{
	if( auto_activation )
	{
		remove();
	}
}

void rs_color::apply()
{
	this->saved_color = engine->render->current_color;
	engine->render->current_color = this->new_color;
}

void rs_color::remove()
{
	engine->render->current_color = this->saved_color;
}

// ----------------------------------------------------------------------------

rs_scale::rs_scale( float new_scale, bool auto_activation )
	: w_render_state( auto_activation )
{
	this->new_scale = new_scale;
	if( auto_activation )
	{
		apply();
	}
}

rs_scale::~rs_scale()
{
	if( auto_activation )
	{
		remove();
	}
}

void rs_scale::apply()
{
	saved_scale = engine->render->current_scale;
	engine->render->current_scale = new_scale;
}

void rs_scale::remove()
{
	engine->render->current_scale = saved_scale;
}

// ----------------------------------------------------------------------------

rs_angle::rs_angle( float new_angle, bool auto_activation )
	: w_render_state( auto_activation )
{
	this->new_angle = new_angle;
	if( auto_activation )
	{
		apply();
	}
}

rs_angle::~rs_angle()
{
	if( auto_activation )
	{
		remove();
	}
}

void rs_angle::apply()
{
	this->saved_angle = engine->render->current_angle;
	engine->render->current_angle = new_angle;
}

void rs_angle::remove()
{
	engine->render->current_angle = saved_angle;
}

// ----------------------------------------------------------------------------

rs_alpha::rs_alpha( float new_alpha, bool auto_activation )
	: w_render_state( auto_activation )
{
	this->new_alpha = new_alpha;

	if( auto_activation )
	{
		apply();
	}
}

rs_alpha::~rs_alpha()
{
	if( auto_activation )
	{
		remove();
	}
}

void rs_alpha::apply()
{
	this->saved_alpha = engine->render->current_alpha;
	engine->render->current_alpha = this->new_alpha;
}

void rs_alpha::remove()
{
	engine->render->current_alpha = this->saved_alpha;
}
