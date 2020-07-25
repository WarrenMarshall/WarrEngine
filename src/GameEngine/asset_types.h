#pragma once

#include "particle_emitter.h"

// ----------------------------------------------------------------------------

struct a_texture : i_asset
{
	unsigned int id = 0;
	float w = 0.0f;
	float h = 0.0f;

	/*
		this is automatically created when the texture is precached. it's
		a convenience so you don't have to declare a matching a_subtexture for
		every a_texture in the asset_def files.

		call "get_subtexture" on your a_texture to get to it.
	*/
	a_subtexture* subtex = nullptr;

	~a_texture();

	void clean_up_internals() override;
	bool create_internals( bool is_hot_reloading ) override;

	void bind();
	void unbind();

	virtual a_subtexture* get_subtexture( float offset = 0.0f );

	void draw( e_render_pass render_pass ) override;
};

// ----------------------------------------------------------------------------
/*
	a lightweight class that defines a renderable subsection of an a_texture
*/

struct a_subtexture : i_asset
{
	// the texture asset we draw from
	a_texture* tex;

	// the rectangle of the area we want to extract from the source texture
	w_rect rc_src = w_rect( 0, 0, -1, -1 );

	// the width/height of this image, as per the dimensions passed to the ctor
	w_sz sz;

	// the uv coordinates that define this image within the texture
	w_uv uv00 = w_uv( 0, 0 );
	w_uv uv11 = w_uv( 1, 1 );

	a_subtexture( const std::string& tex_name );
	a_subtexture( const std::string& tex_name, const w_rect& rc );

	void bind();
	void unbind();
};

// ----------------------------------------------------------------------------

struct a_gradient : a_texture
{
	e_align alignment = e_align::horizontal;
	std::vector<w_color> colors;

	void clean_up_internals() override;
	bool create_internals( bool is_hot_reloading ) override;
};

// ----------------------------------------------------------------------------

struct a_anim_texture : a_texture
{
	std::vector<a_subtexture*> frames;
	std::unique_ptr<w_tween> frame_tween = nullptr;

	e_tween_type tween_type = e_tween_type::loop;
	int frames_per_second = 1;

	a_anim_texture() = delete;
	a_anim_texture( e_tween_type tween_type, int frames_per_second );

	void clean_up_internals() override;
	bool create_internals( bool is_hot_reloading ) override;
	void add_frame( a_subtexture* tex );
	void randomize();

	void update() override;
	a_subtexture* get_subtexture( float offset ) override;
};

// ----------------------------------------------------------------------------

struct a_emitter_params : i_asset, i_speaker
{
	std::string name;

	// texture to draw for this particle
	a_texture* tex = nullptr;

	// where to spawn new particles
	std::unique_ptr<w_particle_spawner> particle_spawner = std::make_unique<w_particle_spawner>();
	std::unique_ptr<w_timeline> t_scale = std::make_unique<w_timeline>( e_timeline_type::float_type );

	w_range r_velocity_spawn = w_range( 50, 50 );	// initial velocity
	w_range r_scale_spawn = w_range( 1, 1 );		// scale - spawn value and timeline
	w_range r_lifespan = w_range( 500, 1000 );		// lifetime in milliseconds
	w_range r_spin_per_sec = w_range( 0, 0 );		// ranges for controlling angle values
	w_range r_spin_spawn = w_range( 0, 0 );
	w_range r_dir_var = w_range( 0, 0 );			// variance for the spawn direction

	// timelines for interpolating color and alpha
	std::unique_ptr<w_timeline> t_color = std::make_unique<w_timeline>( e_timeline_type::color_type );
	std::unique_ptr<w_timeline> t_alpha = std::make_unique<w_timeline>( e_timeline_type::float_type );

	float s_max_spawn_per_sec = 0.0f;	// how many particles to spawn from this emitter, per second
	float a_dir = 0.0f;					// the base direction that particles start moving in when they spawn
	
	bool b_needs_warm_up = true;		// should this emitter "warm up" when first created?	
	bool b_one_shot = false;			// emitter releases "s_spawn_per_sec" particles and stops.

	a_emitter_params();

	bool create_internals( bool is_hot_reloading ) override;
};

// ----------------------------------------------------------------------------

struct w_font_char
{
	float w = 0.0f;
	float h = 0.0f;
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	float xadvance = 0.0f;
	std::unique_ptr<a_subtexture> img = nullptr;
};

// ----------------------------------------------------------------------------

constexpr int max_font_chars = 128;

struct a_font_def : i_asset
{
	// the font texture this font definition is pulling from
	std::string texture_name;
	
	// this height value is the largest one found in the font. using this is
	// guaranteed to enclose any line of text.
	int max_height = 0;

	// using an array here to maximize look ups later on. char values
	// become indices into this array.
	std::array<w_font_char, max_font_chars> char_map;

	bool create_internals( bool is_hot_reloading ) override;
};

// ----------------------------------------------------------------------------

struct a_font : i_asset
{
	a_font_def* font_def = nullptr;

	w_vec2 get_string_extents( const std::string& text );
};

// ----------------------------------------------------------------------------

struct a_cursor : i_asset
{
	w_vec2 hotspot_offset = w_vec2( 0, 0 );
	a_subtexture* img = nullptr;
};

// ----------------------------------------------------------------------------

struct a_9slice_def : i_asset
{
	std::array<a_subtexture*, 9> patches = {};
};

// ----------------------------------------------------------------------------

struct a_sound : i_asset
{
	int snd = -1;
	int channel = -1;

	~a_sound();

	void play();
	void clean_up_internals() override;
	bool create_internals( bool is_hot_reloading ) override;
};

// ----------------------------------------------------------------------------

struct a_music : i_asset
{
	int mus = -1;
	int channel = -1;

	~a_music();

	void play();
	void stop();

	void clean_up_internals() override;
	bool create_internals( bool is_hot_reloading ) override;
};

// ----------------------------------------------------------------------------
