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

	struct
	{
		w_vec2 forces;
		w_vec2 ending_pos = w_vec2::zero;
		float falling_speed = 0.0f;
	} physics_cache;

	virtual void set_transform( const w_vec2& pos, const float angle, const float scale ) override;
	void set_pos( const w_vec2& pos );

	virtual void set_life_cycle( e_life_cycle life_cycle ) override;
	virtual bool can_be_deleted();

	virtual void update_physics();
	virtual void update();
	virtual void update_components();
	virtual void draw();
	virtual void post_spawn();
	virtual void collided_with( ec_collider* collider, w_entity* entity_hit, c2Manifold& manifold ) {}

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_unique<T>( this ) );
		T* new_component = static_cast<T*>( components.back().get() );

		return new_component;
	}

	// returns the first component it finds of the specified type.
	template<typename T>
	T* get_component()
	{
		for( auto& ec : components )
		{
			if( typeid( *ec.get() ) == typeid( T ) )
			{
				return static_cast<T*>( ec.get() );
			}
		}

		return nullptr;
	}

	// fills a vector with all the components it finds of the specified type.
	template<typename T>
	std::basic_string<T*> get_components()
	{
		std::basic_string<T*> ecs;

		for( auto& ec : this->components )
		{
			if( typeid( *ec.get() ) == typeid( T ) )
			{
				ecs += static_cast<T*>( ec.get() );
			}
		}

		return ecs;
	}

	void remove_component( w_entity_component* ec );
};

// ----------------------------------------------------------------------------

// this is a handy way to spawn temp effects like explosions or muzzle flashes.
//
// an fx entity will live as long as it's components do.
// therefore, you must make sure that all components added to it will
// die out on their own.
//
// so adding an emitter that spits out particles
// forever means that this entity will never be deleted. give that
// emitter a "b_one_shot" flag or a lifetime timer.

struct w_entity_fx : w_entity
{
	virtual void update() override;
};
