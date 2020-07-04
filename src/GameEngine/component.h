#pragma once

/*
	base class for all components
*/

struct w_component : i_lifecycle, i_transform
{
	e_component_type type = e_component_type::invalid;
	i_transform* parent_entity = nullptr;

	// used for offsetting whatever the component needs to
	float anim_offset = 0.0f;

	w_component();

	virtual bool is_fully_dead();
	virtual void draw() {}
	virtual void update();
	virtual void post_spawn() {}
};

// ----------------------------------------------------------------------------

struct c_sprite : w_component
{
	a_subtexture* img = nullptr;
	bool flip_x = false, flip_y = false;

	c_sprite();

	w_component* init( const std::string& subtexture_name );
	w_component* init( const std::string& subtexture_name, const w_rect& rc );
	void draw() override;
};

// ----------------------------------------------------------------------------

struct c_emitter : w_component
{
	std::unique_ptr<w_particle_emitter> emitter = nullptr;

	c_emitter();

	w_component* init( i_transform* parent_entity, const std::string& params_name );
	void set_life_cycle( e_lifecycle lifecycle ) override;
	bool is_fully_dead() override;
	void draw() override;
	void update() override;
	void post_spawn() override;
};

// ----------------------------------------------------------------------------

struct c_sound : w_component
{
	a_sound* snd = nullptr;

	c_sound();

	w_component* init( const std::string& snd_name );
	void draw() override;
};
