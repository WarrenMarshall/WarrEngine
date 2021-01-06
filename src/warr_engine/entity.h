#pragma once

// ----------------------------------------------------------------------------

struct w_entity : i_life_cycle
{
	w_entity();
	~w_entity();

	std::string tag;

	// entity components
	std::vector<std::unique_ptr<w_entity_component>> components;

	// which collision layer(s) this entity is a part of
	bitflags collision_layer = 0;

	// which collision layer(s) this entity will collide WITH
	bitflags collides_with = 0;

	w_entity* set_tag( const char* tag );
	w_entity* set_collision( bitflags layer, bitflags collides_with );

	virtual void ilc_set( e_life_cycle life_cycle ) override;
	[[nodiscard]] virtual bool can_be_deleted();

	void update_from_physics();
	virtual void update();
	virtual void update_components();
	virtual void draw();

	// sets the entities position and also updates all physics components
	void set_position_deep( const w_vec2& pos, bool reset_velocity );
	void set_angle_deep( float angle );

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_unique<T>( this ) );
		auto new_component = static_cast<T*>( components.back().get() );

		return new_component;
	}

	// returns the first component it finds that matches the type bit mask.
	template<typename T>
	[[nodiscard]] T* get_component()
	{
		for( auto& ec : components )
		{
			if( dynamic_cast<T*>( ec.get() ) )
			{
				return static_cast<T*>( ec.get() );
			}
		}

		return nullptr;
	}

	// fills a vector with all the components it finds that match the type bit mask.

	template<typename T>
	void get_components( std::vector<T*>& ecs )
	{
		for( auto& ec : this->components )
		{
			if( dynamic_cast<T*>( ec.get() ) )
			{
				ecs.emplace_back( static_cast<T*>( ec.get() ) );
			}
		}
	}

	template<typename BaseT, typename T>
	void get_components( std::vector<BaseT*>& ecs )
	{
		for( auto& ec : this->components )
		{
			if( dynamic_cast<T*>( ec.get() ) )
			{
				ecs.emplace_back( static_cast<BaseT*>( ec.get() ) );
			}
		}
	}

	[[nodiscard]] ec_b2d_body* phys_get_primary_body();
	void phys_set_friction( float friction );
	void phys_set_restitution( float restitution );
	void phys_set_density( float density );

	virtual void phys_begin_contact( w_pending_collision& coll, w_entity* other );
	virtual void phys_end_contact( w_pending_collision& coll, w_entity* other );

	w_transform* get_tform();
};

// ----------------------------------------------------------------------------

// this is a handy way to spawn temp effects like explosions or muzzle flashes.
//
// NOTE : a transient entity will only live as long as it's components do.
// therefore, you must make sure that all components added to it will
// die out on their own.
//
// so adding an emitter component to an instance of this class
// which spits out particles forever means that instance will never be
// die or be deleted. give that emitter a "one shot" flag or a lifetime timer.

struct w_entity_transient : w_entity
{
	virtual void update() override;
};

// ----------------------------------------------------------------------------

// a camera ony affects drawing entities and regular draw calls. it does
// not affect UI drawing in any way. this is important because events like
// camera shakes will shake the world but leave the UI stable.

struct e_camera : w_entity
{
	// #component - this should all get moved into a "ec_follow_target" component
	struct
	{
		w_entity* target = nullptr;
		e_follow_flags flags = follow_flags::xy_axis;

		// this is a multiplier against the default speed.
		// So 2.0 is twice as fast, 0.5f is twice as slow.
		float strength = 1.0f;

		// positions are snapped to the grid when set_position is called on an entity,
		// so this is a scratch pad where we can keep the position of the following
		// entity in floating point form - aka not snapped to the grid. this means we
		// can smoothly interpolate towards the target each frame while still snapping
		// the entity itself to the grid for accurate rendering.
		w_vec2 pos = w_vec2::zero;

		std::optional<w_vec2> limits_x = std::nullopt;
		std::optional<w_vec2> limits_y = std::nullopt;
	} follow;

	void set_follow_target( w_entity* entity_to_follow, e_follow_flags flags, float strength );
	void set_follow_limits_x( w_vec2 limits );
	void set_follow_limits_y( w_vec2 limits );
	virtual void update() override;
};
