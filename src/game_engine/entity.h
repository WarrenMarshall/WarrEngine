#pragma once

// ----------------------------------------------------------------------------

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
	// components

	std::vector<std::unique_ptr<w_entity_component>> components;

	struct
	{
		std::vector<ec_sprite*> sprites;
		std::vector<ec_emitter*> emitters;
		std::vector<ec_sound*> sounds;
		std::vector<ec_collider*> colliders;
	} ec;

	// physics

	// all the forces currently acting on the entity
	std::vector<std::unique_ptr<w_force>> forces;

	struct
	{
		w_vec2 forces;
		w_vec2 ending_pos = w_vec2::zero;
	} physics_cache;

	void set_life_cycle( e_lifecycle lifecycle ) override;
	virtual bool can_be_deleted();

	virtual void pre_update();
	virtual void update();
	virtual void post_update();
	virtual void draw();
	virtual void post_spawn();
	virtual void collided_with( w_entity* entity_hit, c2Manifold& manifold ) {}

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_unique<T>( this ) );
		T* new_component = static_cast<T*>( components.back().get() );

		if( new_component->type == component_type::sprite )
			ec.sprites.push_back( (ec_sprite*) new_component );
		else if( new_component->type == component_type::emitter )
			ec.emitters.push_back( (ec_emitter*) new_component );
		else if( new_component->type == component_type::sound )
			ec.sounds.push_back( (ec_sound*) new_component );
		else if( new_component->type == component_type::collider )
			ec.colliders.push_back( (ec_collider*) new_component );

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
