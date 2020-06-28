#pragma once

// ----------------------------------------------------------------------------

#define W_COLOR_WHITE		w_color( 1.0f, 1.0f, 1.0f )
#define W_COLOR_BLACK		w_color( 0.0f, 0.0f, 0.0f )
#define W_COLOR_RED			w_color( 1.0f, 0.0f, 0.0f )
#define W_COLOR_GREEN		w_color( 0.0f, 1.0f, 0.0f )
#define W_COLOR_BLUE		w_color( 0.0f, 0.0f, 1.0f )
#define W_COLOR_LIGHT_BLUE	w_color( 0.5f, 0.5f, 1.0f )
#define W_COLOR_ORANGE		w_color( 1.0f, 0.5f, 0.0f )
#define W_COLOR_YELLOW		w_color( 1.0f, 1.0f, 0.0f )
#define W_COLOR_TEAL		w_color( 0.3f, 0.8f, 1.0f )
#define W_COLOR_DARK_TEAL	w_color( 0.2f, 0.4f, 0.5f )
#define W_COLOR_DARK_GREY	w_color( 0.25f, 0.25f, 0.25f )
#define W_COLOR_GREY		w_color( 0.5f, 0.5f, 0.5f )
#define W_COLOR_LIGHT_GREY	w_color( 0.75f, 0.75f, 0.75f )
#define W_COLOR_LIGHT_GREEN	w_color( 0.5f, 1.0f, 0.5f )
#define W_COLOR_DARK_GREEN	w_color( 0.25f, 0.5f, 0.25f )
#define W_COLOR_MAGENTA		w_color( 0.96f, 0.32f, 0.65f )

struct w_color
{
	float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

	w_color() {}
	w_color( const w_color& other );
	w_color( float r, float g, float b, float a = 1.0f );
	w_color( std::string str );

	//std::string to_string();
	w_color from_string( std::string text );
};

// ----------------------------------------------------------------------------

/*
	a single stop on a timeline
*/

struct w_keyframe
{
	float pct_marker = 0.0f;
	float float_value = 0.0f;
	w_color color_value = W_COLOR_WHITE;;

	w_keyframe() {}
	w_keyframe( const w_keyframe& other );
	w_keyframe( float pct_marker, float value );
	w_keyframe( float pct_marker, w_color value );
};

/*
	represents successive steps through a 0-1 timeline
*/

struct w_timeline
{
	e_timeline_type type = e_timeline_type::invalid;
	std::vector<w_keyframe> keyframes;

	w_timeline( e_timeline_type type );
	w_timeline( const w_timeline& other );

	w_timeline* kf_clear();
	w_timeline* kf_add( w_keyframe keyframe );

	void get_value( float pct_on_timeline, float* value );
	void get_value( float pct_on_timeline, w_color* value );
};

// ----------------------------------------------------------------------------

struct w_range
{
	float min = 0.0f;
	float max = 1.0f;

	w_range() {}
	w_range( float min, float max );
	w_range( std::string str );

	float get_value();
};

// ----------------------------------------------------------------------------

struct w_rect
{
	union
	{
		struct
		{
			float x;
			float y;
			float w;
			float h;
		};
		struct
		{
			float u0;
			float v0;
			float u1;
			float v1;
		};
	};

	w_rect();
	w_rect( float x, float y );
	w_rect( float x, float y, float w, float h );
};

// ----------------------------------------------------------------------------

struct w_vec2
{
	union
	{
		struct
		{
			float x, y;
		};
		struct
		{
			float u, v;
		};
		struct
		{
			float w, h;
		};
	};

	w_vec2();
	w_vec2( float x, float y );
	w_vec2( std::string str );

	w_vec2 operator+( const w_vec2& v );
	w_vec2 operator-( const w_vec2& v );
	w_vec2 operator=( const w_vec2& v );

	static w_vec2 scale( w_vec2 a, float scale );
	static float get_size_squared( w_vec2 a );
	static float get_size( w_vec2 a );
	static float get_distance_between( w_vec2 a, w_vec2 b );
	static w_vec2 normalize( w_vec2 a );
	static w_vec2 from_angle( float angle );
};

struct w_vec3
{
	float x = 0.0f, y = 0.0f, z = 0.0f;

	w_vec3();
	w_vec3( const w_vec3& v );
	w_vec3( float x, float y, float z );
	w_vec3( std::string str );

	static const w_vec3 zero;
	
	w_vec3 operator+( const w_vec3& v );
	w_vec3 operator-( const w_vec3& v );
	w_vec3 operator=( const w_vec3& v );
	w_vec3 operator*( const float& f);

	static float get_size_squared( w_vec3 a );
	static float get_size( w_vec3 a );
	static float get_distance_between( w_vec3 a, w_vec3 b );
	static w_vec3 normalize( w_vec3 a );
	static w_vec3 from_angle( float angle );
};

// ----------------------------------------------------------------------------

struct w_bbox
{
	w_vec3 min, max;

	w_bbox();

	void add( w_vec3 vtx );
	void reset();
};
