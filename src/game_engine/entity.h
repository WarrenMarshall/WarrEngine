#pragma once

struct w_entity : i_life_cycle, i_transform
{
	float air_control_damping = 0.25f;

	// entity components
	std::vector<std::unique_ptr<w_entity_component>> components;

	// the layer that this entity lives on
	w_layer* parent_layer = nullptr;

	// which collision layer(s) this entity is a part of
	e_collision_layer collision_layer = 0;

	// which collision layer(s) this entity will collide WITH
	e_collision_layer collides_with = 0;

	void set_collision( e_collision_layer layer, e_collision_layer collides_with );

	// generic flag to indicate that the component should draw
	// stuff to indicate it's internal state.
	bool draw_debug_info = false;

	virtual void set_life_cycle( e_life_cycle life_cycle ) override;
	virtual bool can_be_deleted();

	virtual void update_physics();
	virtual void update();
	virtual void update_components();
	virtual void draw();

	virtual void set_position( w_vec2 pos ) override;

	void teleport( w_vec2 pos, bool reset_velocity );

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_unique<T>( this ) );
		T* new_component = static_cast<T*>( components.back().get() );

		return new_component;
	}

	// returns the first component it finds that matches the type bit mask.
	template<typename T>
	T* get_component( e_component_type type_mask )
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
	std::basic_string<T*> get_components( e_component_type type_mask )
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
};

// ----------------------------------------------------------------------------

// this is a handy way to spawn temp effects like explosions or muzzle flashes.
//
// NOTE : an fx entity will only live as long as it's components do.
// therefore, you must make sure that all components added to it will
// die out on their own.
//
// so adding an emitter that spits out particles forever means that\
// this entity will never be deleted. give that emitter a "b_one_shot"
// flag or a lifetime timer.

struct w_entity_fx : w_entity
{
	virtual void update() override;
};
