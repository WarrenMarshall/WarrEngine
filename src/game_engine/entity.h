#pragma once

struct w_entity : i_life_cycle, i_transform
{
	const char* tag = nullptr;

	// entity components
	std::vector<std::unique_ptr<w_entity_component>> components;

	// the layer that this entity lives on
	w_layer* parent_layer = nullptr;

	// which collision layer(s) this entity is a part of
	bitflags collision_layer = 0;

	// which collision layer(s) this entity will collide WITH
	bitflags collides_with = 0;

	void set_collision( bitflags layer, bitflags collides_with );

	// generic flag to indicate that the component should draw
	// stuff to indicate it's internal state.
	bool draw_debug_info = false;

	virtual void set_life_cycle( e_life_cycle life_cycle ) override;
	_NODISCARD virtual bool can_be_deleted();

	virtual void update_from_physics();
	virtual void update();
	virtual void update_components();
	virtual void draw();

	// sets the entities position and also updates all physics components
	void set_position_deep( w_vec2 pos, bool reset_velocity );
	void set_angle_deep( float angle );

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_unique<T>( this ) );
		T* new_component = static_cast<T*>( components.back().get() );

		return new_component;
	}

	// returns the first component it finds that matches the type bit mask.
	template<typename T>
	_NODISCARD T* get_component( e_component_type type_mask )
	{
		for( auto& ec : components )
		{
			if( ec->type & type_mask )
			{
				return static_cast<T*>( ec.get() );
			}
		}

		return nullptr;
	}

	// fills a vector with all the components it finds that match the type bit mask.
	template<typename T>
	_NODISCARD std::basic_string<T*> get_components( e_component_type type_mask )
	{
		std::basic_string<T*> ecs;

		for( auto& ec : this->components )
		{
			if( ec->type & type_mask )
			{
				ecs += static_cast<T*>( ec.get() );
			}
		}

		return ecs;
	}

	void remove_component( w_entity_component* ec );

	_NODISCARD ec_b2d_body* phys_get_primary_body();
	void phys_set_friction( float friction );
	void phys_set_restitution( float restitution );
	void phys_set_density( float density );
};

// ----------------------------------------------------------------------------

// this is a handy way to spawn temp effects like explosions or muzzle flashes.
//
// NOTE : an fx entity will only live as long as it's components do.
// therefore, you must make sure that all components added to it will
// die out on their own.
//
// so adding an emitter that spits out particles forever means that
// this entity will never be deleted. give that emitter a "one shot"
// flag or a lifetime timer.

struct w_entity_fx : w_entity
{
	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct w_camera : w_entity
{
	struct
	{
		w_entity* target = nullptr;
		e_follow_flags flags = follow_flags::xy_axis;
		float strength = 1.0f;
		std::optional<w_vec2> limits_x = std::nullopt;
		std::optional<w_vec2> limits_y = std::nullopt;
	} follow;

	void set_follow_target( w_entity* entity_to_follow, e_follow_flags flags, float strength );
	void set_follow_limits_x( w_vec2 limits );
	void set_follow_limits_y( w_vec2 limits );
	virtual void update() override;
};
