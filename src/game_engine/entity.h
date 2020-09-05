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
#ifndef _FINAL_RELEASE
	bool debug_draw_collision = false;
#endif

	// components

	std::vector<std::unique_ptr<w_entity_component>> components;

	struct
	{
		std::vector<ec_sprite*> sprites;
		std::vector<ec_emitter*> emitters;
		std::vector<ec_sound*> sounds;
		std::vector<ec_collider*> colliders;
	} ec;

	// which collision layer this entity is a part of
	//
	// identifies what this entity IS to other colliders.
	e_collision_layer collision_layer = 0;

	// a bitmask of the collision_layer enum that identifies
	// which collision layers this will collide WITH
	//
	// this is only set for dynamic entities which will
	// instigate collisions. static entities can leave this
	// as zero.
	int collides_with = 0;

	// physics

	// all the forces currently acting on the entity
	std::vector<std::unique_ptr<w_force>> forces;

	struct
	{
		w_vec2 forces;
		w_vec2 ending_pos = w_vec2::zero;
	} physics_cache;

	virtual void set_transform( const w_vec2& pos, const float& angle, const float& scale ) override;
	void set_pos( const w_vec2& pos );

	void set_life_cycle( e_lifecycle lifecycle ) override;
	virtual bool can_be_deleted();

	virtual void pre_update();
	virtual void update();
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
	virtual void update() final;
};
