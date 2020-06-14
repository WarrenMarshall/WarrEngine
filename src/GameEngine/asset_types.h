#pragma once

#include "particle_emitter.h"

// ----------------------------------------------------------------------------

struct a_texture : i_asset
{
	unsigned int id = 0;
	float w = 0.0f;
	float h = 0.0f;

	~a_texture();

	virtual void clean_up_internals();
	virtual bool create_internals( bool is_hot_reloading );
	void bind();
	void unbind();
	virtual a_texture* get_texture();
	virtual void update() {}
	virtual void update_fts() {}
	virtual void draw( e_render_pass render_pass );
};

// ----------------------------------------------------------------------------

struct a_gradient : a_texture
{
	e_align alignment = e_align::horizontal;
	std::vector<w_color> colors;

	virtual void clean_up_internals();
	virtual bool create_internals( bool is_hot_reloading );
};

// ----------------------------------------------------------------------------

struct a_anim_texture : a_texture
{
	std::vector<a_texture*> frames;
	std::unique_ptr<w_tween> frame_tween = nullptr;

	a_anim_texture();

	virtual void clean_up_internals();
	virtual bool create_internals( bool is_hot_reloading );
	void add_frame( a_texture* tex );
	void set_speed( float indices_per_sec );
	void randomize();
	virtual void update();
	virtual void update_fts();
	virtual a_texture* get_texture();
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

	virtual bool create_internals( bool is_hot_reloading );
};

// ----------------------------------------------------------------------------

constexpr int max_font_chars = 128;

struct a_font_def : i_asset
{
	// this height value is the largest one found in the font. using this is
	// guaranteed to enclose any line of text.
	float max_height = 0.0f;

	// using an array here to maximize look ups later on. char values
	// become indices into this array.
	std::array<w_font_char, max_font_chars> char_map;
	//Pw_font_char char_map[max_font_chars];

	virtual bool create_internals( bool is_hot_reloading );
};

// ----------------------------------------------------------------------------

struct a_font : i_asset
{
	a_texture* tex = nullptr;
	a_font_def* font_def = nullptr;

	w_vec2 get_string_extents( const std::string& text );
};

// ----------------------------------------------------------------------------

struct a_atlas_def : i_asset
{
	// using an array here to maximize look ups later on. char values
	// become indices into this array.
	std::vector<w_atlas_tile> tile_map;

	virtual bool create_internals( bool is_hot_reloading );
};

#if 0
struct a_atlas : i_asset
{
	a_texture* tex = nullptr;
	//a_font_def* font_def = nullptr;
};
#endif

// ----------------------------------------------------------------------------

struct a_9slice_def : i_asset
{
	std::array<w_rect, 9> patches;

	virtual bool create_internals( bool is_hot_reloading );
};

// ----------------------------------------------------------------------------

struct a_sound : i_asset
{
	int snd = -1;
	int channel = -1;

	~a_sound();

	void play();
	virtual void clean_up_internals();
	virtual bool create_internals( bool is_hot_reloading );
};

// ----------------------------------------------------------------------------

struct a_music : i_asset
{
	int mus = -1;
	int channel = -1;

	~a_music();

	void play();
	void stop();

	virtual void clean_up_internals();
	virtual bool create_internals( bool is_hot_reloading );
};

// ----------------------------------------------------------------------------
