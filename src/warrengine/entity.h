
namespace war
{

struct entity
{
	transform _tform;

	// this is a read-only pointer. to change the transform, use the
	// "transform_" functions below.
	[[nodiscard]] const transform* get_transform()
	{
		return &_tform;
	}

	// direct transforms

	transform* transform_set( const vec2& pos, const float angle, const float scale );
	transform* transform_set_pos( const vec2& pos );
	transform* transform_set_angle( const float angle );
	transform* transform_set_scale( const float scale );

	// delta transforms

	transform* transform_delta_pos( const vec2& delta );
	transform* transform_delta_angle( const float delta );
	transform* transform_delta_scale( const float delta );

	// ----------------------------------------------------------------------------

	void update_physics_components_to_match_transform();
	void update_transform_to_match_physics_components();

	virtual void set_life_cycle( e_life_cycle lc );
	const life_cycle_mgr* get_life_cycle();

	void set_tag( hash tag );
	hash tag = hash_none;
	int pick_id = 0;
	bool is_selected = false;
	render_state_optional rs_opt;

	void make_pickable();

	// entity components
	std::deque<std::unique_ptr<entity_component>> components;

	[[nodiscard]] virtual bool can_be_deleted();

	void update_from_physics();
	virtual void pre_update();
	virtual void update();
	virtual void post_update();
	virtual void update_components();
	virtual void draw();

	template<typename T> T* add_component()
	{
		components.emplace_back( std::make_unique<T>( this ) );
		auto new_component = static_cast<T*>( components.back().get() );

		return new_component;
	}

	// returns the first component it finds that matches the type bit mask.
	[[nodiscard]] entity_component* get_component( hash tag = hash_none ) const
	{
		return get_component<entity_component>( tag );
	}

	template<typename T>
	[[nodiscard]] T* get_component( hash tag = hash_none ) const
	{
		for( auto& ec : components )
		{
			if( dynamic_cast<T*>( ec.get() ) and ( tag == hash_none or ec->tag == tag ) )
			{
				return static_cast<T*>( ec.get() );
			}
		}

		return nullptr;
	}

	// quickly check to see if an entity has at least one of the specified
	// components attached
	template<typename T>
	[[nodiscard]] bool has_component( hash tag = hash_none ) const
	{
		return get_component<T>( tag ) != nullptr;
	}

	// fills a vector with all the components it finds that match the class "T"

	template<typename T>
	std::vector<T*> get_components( hash tag = hash_none ) const
	{
		// make sure the vector can handle at least 10 items - most get_* calls
		// will result in fewer than that so this saves unnecessary
		// reallocations
		std::vector<T*> ecs;
		ecs.reserve( 10 );

		for( auto& ec : this->components )
		{
			if( dynamic_cast<T*>( ec.get() ) and ( tag == hash_none or ec->tag == tag ) )
			{
				ecs.emplace_back( static_cast<T*>( ec.get() ) );
			}
		}

		return ecs;
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

	virtual void on_collision_begin( box2d_physics::pending_collision& coll, entity* other );
	virtual void on_collision_end( box2d_physics::pending_collision& coll, entity* other );

private:

	life_cycle_mgr life_cycle;
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

struct entity_transient : entity
{
	virtual void update() override;
};

}
