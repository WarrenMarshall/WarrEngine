#pragma once

// ----------------------------------------------------------------------------

struct w_entity : i_life_cycle
{
	hash tag;
	float pick_id = 0.0f;
	bool is_selected;
	w_render_state_opt rs_opt;

	w_entity();
	~w_entity() = default;

	void make_pickable();

	// entity components
	std::vector<std::unique_ptr<w_entity_component>> components;

	virtual void ilc_set( e_life_cycle life_cycle ) override;
	_NODISCARD virtual bool can_be_deleted();

	void update_from_physics();
	virtual void update();
	virtual void update_components();
	virtual void draw();

	// sets the entities position and also updates all physics components
	void set_pos_deep( const w_vec2& pos, bool reset_velocity );
	void set_angle_deep( float angle );

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_unique<T>( this ) );
		auto new_component = static_cast<T*>( components.back().get() );

		return new_component;
	}

	// returns the first component it finds that matches the type bit mask.
	_NODISCARD w_entity_component* get_component( hash tag = 0 )
	{
		return get_component<w_entity_component>( tag );
	}

	template<typename T>
	_NODISCARD T* get_component( hash tag = 0 )
	{
		for( auto& ec : components )
		{
			if( dynamic_cast<T*>( ec.get() ) && ( tag == 0 || ec->tag == tag ) )
			{
				return static_cast<T*>( ec.get() );
			}
		}

		return nullptr;
	}

	// fills a vector with all the components it finds that match the class "T"

	template<typename T>
	void get_components( std::vector<T*>& ecs, hash tag = 0 )
	{
		// make sure the vector can handle at least 10 items - most get_* calls
		// will result in fewer than that so this saves unnecessary
		// reallocations
		ecs.reserve( 10 );

		for( auto& ec : this->components )
		{
			if( dynamic_cast<T*>( ec.get() ) && ( tag == 0 || ec->tag == tag ) )
			{
				ecs.emplace_back( static_cast<T*>( ec.get() ) );
			}
		}
	}

	// fills a vector with all the components it finds that match the class "T"
	// and have a base class of "B"

	template<typename B, typename T>
	void get_components( std::vector<B*>& ecs )
	{
		// make sure the vector can handle at least 10 items - most get_* calls
		// will result in fewer than that so this saves unnecessary
		// reallocations
		ecs.reserve( 10 );

		for( auto& ec : this->components )
		{
			if( dynamic_cast<T*>( ec.get() ) )
			{
				ecs.emplace_back( static_cast<B*>( ec.get() ) );
			}
		}
	}

	virtual void on_collision_begin( w_pending_collision& coll, w_entity* other );
	virtual void on_collision_end( w_pending_collision& coll, w_entity* other );

	w_transform* get_transform();
};

// ----------------------------------------------------------------------------
// this is a handy way to spawn temp effects like explosions or muzzle flashes.
//
// a transient entity will only live as long as it's components do. therefore,
// you must make sure that all components added to it will die out on their own.
//
// so adding an emitter component to an instance of this class which spits out
// particles forever means that instance will never be die or be deleted. give
// that emitter a "one shot" flag or a lifetime timer.

struct w_entity_transient : w_entity
{
	virtual void update() override;
};

// ----------------------------------------------------------------------------
// a camera ony affects drawing entities and regular draw calls. it does not
// affect UI drawing in any way. this is important because events like camera
// shakes will shake the world but leave the UI stable.

struct e_camera : w_entity
{
	e_camera();
};
