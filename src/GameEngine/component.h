#pragma once

/*
	base class for all components
*/

struct w_component : i_lifecycle, i_transform
{
	e_component_type type = e_component_type::invalid;
	i_transform* parent_entity = nullptr;

	w_component();

	virtual bool is_fully_dead();
	virtual void draw() {}
	virtual void update_fts();
	virtual void post_spawn() {}
};

// ----------------------------------------------------------------------------

struct c_sprite : w_component
{
	a_image* img = nullptr;
	bool flip_x = false, flip_y = false;

	c_sprite();

	w_component* init( const std::string& img_name );
	w_component* init( const std::string& image_name, const w_rect& rc );
	virtual void draw();
};

// ----------------------------------------------------------------------------

struct c_emitter : w_component
{
	std::unique_ptr<w_particle_emitter> emitter = nullptr;

	c_emitter();

	w_component* init( i_transform* parent_entity, const std::string& params_name );
	virtual void set_life_cycle( e_lifecycle lifecycle );
	virtual bool is_fully_dead();
	virtual void draw();
	virtual void update_fts();
	virtual void post_spawn();
};

// ----------------------------------------------------------------------------

struct c_sound : w_component
{
	a_sound* snd = nullptr;

	c_sound();

	w_component* init( const std::string& snd_name );
	virtual void draw();
};
