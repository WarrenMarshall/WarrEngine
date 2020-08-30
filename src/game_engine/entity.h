#pragma once
// ----------------------------------------------------------------------------

// applies delta movements to an entity, stored as "per second" vectors

struct w_force
{
	w_force() = delete;
	w_force( w_vec2 dir, float strength );

	void init_to_zero();

	w_vec2 dir;			// normalized
	float strength;
};

struct w_entity : i_lifecycle, i_transform
{
	std::vector<std::unique_ptr<w_entity_component>> components;

	struct
	{
		std::vector<ec_sprite*> sprite;
		std::vector<ec_emitter*> emitter;
		std::vector<ec_sound*> sound;
	} ec;

	std::vector<std::unique_ptr<w_force>> forces;
	struct
	{
		w_vec2 forces;
	} cache;

	void set_life_cycle( e_lifecycle lifecycle ) override;
	virtual bool can_be_deleted();

	virtual void update();
	virtual void draw();
	virtual void post_spawn();

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_unique<T>( this ) );
		T* new_component = static_cast<T*>( components.back().get() );
		switch( new_component->type )
		{
			case component_type::sprite:
				ec.sprite.push_back( (ec_sprite*)new_component );
				break;
			case component_type::emitter:
				ec.emitter.push_back( (ec_emitter*)new_component );
				break;
			case component_type::sound:
				ec.sound.push_back( (ec_sound*)new_component );
				break;
		}

		return new_component;
	}
};

// ----------------------------------------------------------------------------

// a cozy is an entity that lives for X ms and then marks itself as dying
// so it'll get automatically cleaned up.
//
// this is a handy way to spawn temp effects like explosions or muzzle flashes.

struct w_entity_cozy : w_entity
{
	float life_remaining = 0.0f;

	w_entity_cozy();

	void update() override;
};
