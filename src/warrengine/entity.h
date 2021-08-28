
namespace war
{

// ----------------------------------------------------------------------------

struct Entity_Simple_Force final
{
	Entity_Simple_Force() = default;
	Entity_Simple_Force( Vec2 normal, float strength );

	Vec2 normal = Vec2::zero;
	float strength = 0.f;
};

// ----------------------------------------------------------------------------

struct Entity_Simple_Collision final
{
	e_sc_type_t type = e_sc_type::dynamic;

	[[nodiscard]] bool is_dynamic()
	{
		return ( type == e_sc_type::dynamic );
	}
	[[nodiscard]] bool is_stationary()
	{
		return ( type == e_sc_type::stationary );
	}
	[[nodiscard]] bool is_kinematic()
	{
		return ( type == e_sc_type::kinematic );
	}

	float friction = 1.0f;
	float bounce_dampen = 0.75f;
	Range<float> max_velocity_x = { -5.0f, 5.0f };
	Range<float> max_velocity_y = { -5.0f, 5.0f };
	bool is_in_air : 1= false;
	bool is_affected_by_gravity : 1 = false;
	bool is_bouncy : 1 = false;
	bool bounce_needs_dampening : 1 = false;
};

// ----------------------------------------------------------------------------

struct Entity
{
	Life_Cycle_Mgr life_cycle;
	scene* parent_scene = nullptr;

#ifndef _FINAL_RELEASE
	// a handy string to throw info or a name in debug builds to make figuring
	// out which entity you're looking at in the debugger easier
	std::string debug_name;
#endif

	Entity();
	Entity( std::string debug_name );
	virtual ~Entity() = default;

	Entity_Simple_Collision simple;

	// forces and impulses

	Vec2 velocity = Vec2::zero;

	std::vector<Entity_Simple_Force> pending_forces;

	void add_force( const Entity_Simple_Force& force );
	void set_force( const Entity_Simple_Force& force );
	void add_impulse( const Entity_Simple_Force& force );

	void compile_velocity();

	// transforms
	Transform _tform;

	// this is a read-only pointer. to change the transform, use the
	// "transform_" functions below.
	[[nodiscard]] const Transform* get_transform();

	// direct transforms

	Transform* set_pos_angle_scale( const Vec2& pos, const float angle, const float scale );
	Transform* set_pos( const Vec2& pos );
	Transform* set_angle( const float angle );
	Transform* set_scale( const float scale );

	[[nodiscard]] Vec2 get_pos();
	[[nodiscard]] float get_angle();
	[[nodiscard]] float get_scale();

	// delta transforms

	Transform* add_delta_pos( const Vec2& delta );
	Transform* add_delta_angle( const float delta );
	Transform* add_delta_scale( const float delta );

	// ----------------------------------------------------------------------------

	void update_physics_components_to_match_transform();
	void update_transform_to_match_physics_components();

	virtual void set_life_cycle( e_life_cycle_t lc );

	void set_tag( hash tag );
	hash tag = hash_none;
	int pick_id = 0;
	bool is_selected = false;
	Render_State_Optional rs_opt;

	void make_pickable();

	std::vector<std::unique_ptr<Entity_Component>> components;

	[[nodiscard]] virtual bool can_be_deleted();

	void update_from_physics();
	virtual void pre_update();
	virtual void update();
	virtual void post_update();

	void apply_forces();

	virtual void pre_update_components();
	virtual void update_components();
	virtual void post_update_components();

	void remove_dead_components();

	virtual void draw();

	template<typename T> T* add_component()
	{
		components.push_back( std::make_unique<T>( this ) );
		auto new_component = static_cast<T*>( components.back().get() );

		return new_component;
	}

	// returns the first component it finds that matches the type bit mask.
	[[nodiscard]] Entity_Component* get_component( hash tag = hash_none ) const
	{
		return get_component<Entity_Component>( tag );
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
				ecs.push_back( static_cast<T*>( ec.get() ) );
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
				ecs.push_back( static_cast<B*>( ec.get() ) );
			}
		}
	}

	virtual void on_box2d_collision_begin( box2d_physics::Pending_Collision& coll, Entity* touched_by );
	virtual void on_box2d_collision_end( box2d_physics::Pending_Collision& coll, Entity* touched_by );

	virtual bool on_collided( simple_collision::Pending_Collision& coll );
	virtual bool on_touched( simple_collision::Pending_Collision& coll );

	void reflect_across( Vec2 normal );

	virtual void apply_movement_jump();
	virtual void apply_movement_walk( Vec2 delta, float speed );
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

struct Entity_Transient final : Entity
{
	virtual void update() override;
};

}
