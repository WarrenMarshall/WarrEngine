
namespace war
{

// ----------------------------------------------------------------------------

struct Physics_Force final
{
	Physics_Force() = default;
	Physics_Force( Vec2 normal, f32 strength );

	Vec2 normal = Vec2::zero;
	f32 strength = 0.f;
};

// ----------------------------------------------------------------------------

struct Entity
{
	struct
	{
		// if true, this entity will be given a selection ID and be drawn into the picking frame buffer
		bool pickable : 1 = false;

		// if true, this entity is included in the scene's quad tree spatial map.
		// this defaults to false for performance reasons. entities must specifically opt-in.
		bool include_in_quad_tree : 1 = false;
	} flags;

	Life_Cycle life_cycle;

	hash tag = hash_none;
	i32 pick_id = 0;
	Render_State_Optional rs_opt;

	Scene* parent_scene = nullptr;

	// the entities that are stuck to this one. meaning that when this entity
	// moves, the entities in this list move as well.
	std::set<Entity*> sticky_set;

	std::set<Collision_Body_Component*> sensors_this_frame;
	std::set<Collision_Body_Component*> sensors_last_frame;

	struct
	{
		e_physics_body_type type = e_physics_body_type::dynamic;

		[[nodiscard]] bool is_dynamic() { return ( type == e_physics_body_type::dynamic ); }
		[[nodiscard]] bool is_stationary() { return ( type == e_physics_body_type::stationary ); }
		[[nodiscard]] bool is_kinematic() { return ( type == e_physics_body_type::kinematic ); }

		struct
		{
			Range max_velocity_x = { -5.0f, 5.0f };
			Range max_velocity_y = { -5.0f, 5.0f };
		} settings;

		struct
		{
			bool is_in_air : 1 = true;
			bool is_affected_by_gravity : 1 = false;
			bool is_bouncy : 1 = false;
		} flags;

	} collision;

	// forces and impulses

	Vec2 velocity = Vec2::zero;
	std::vector<Physics_Force> pending_forces;

	// transforms
	Transform _tform;

	std::list<std::unique_ptr<Entity_Component>> components;

	// the nodes in the parent scenes spatial map that this entity is touching
	std::set<Quad_Tree::Node*> nodes;

	// updated once per frame, this is the world space bounding box of all
	// the entities collision primitives
	Rect collision_ws_aabb;

	// a handy string to throw info or a name in debug builds to make figuring
	// out which entity you're looking at in the debugger easier
	std::string debug_name;

	Entity() = default;
	virtual ~Entity() = default;

	void add_force( const Physics_Force& force );
	void add_impulse( const Physics_Force& force );
	void add_impulse( f32 strength );

	void compile_velocity();

	// this is a read-only pointer. to change the transform, use the
	// "transform_" functions below.
	[[nodiscard]] const Transform* get_transform();

	// direct transforms

	Transform* set_pos_angle_scale( const Vec2& pos, const f32 angle, const f32 scale );
	Transform* set_pos( const Vec2& pos );
	Transform* set_angle( const f32 angle );
	Transform* set_scale( const f32 scale );

	[[nodiscard]] Vec2 get_pos();
	[[nodiscard]] f32 get_angle();
	[[nodiscard]] f32 get_scale();

	// delta transforms

	void add_delta_pos( const Vec2& delta );
	void add_delta_angle( const f32 delta );
	void add_delta_scale( const f32 delta );

	void update_box2d_components_to_match_transform();
	void update_transform_to_match_box2d_components();

	virtual void set_life_cycle( e_life_cycle lc );

	void set_tag( hash tag );

	void set_pickable();

	[[nodiscard]] virtual bool can_be_deleted();

	virtual void pre_update();
	virtual void update();
	virtual void post_update();

	void apply_forces();

	virtual void pre_update_components();
	virtual void update_components();
	virtual void post_update_components();

	void remove_dead_components();

	virtual void draw();

	template<typename T, typename ...Params> T* add_component( Params&&... params )
	{
		components.push_back( std::make_unique<T>( this, std::forward<Params>( params )... ) );
		auto new_component = (T*)( components.back().get() );

		return new_component;
	}

	// quickly check to see if an entity has at least one of the specified
	// components attached

	[[nodiscard]] bool has_component( e_component_type type, hash tag = hash_none ) const
	{
		for( auto& ec : this->components )
		{
			if( ec.get()->component_type == type and ( tag == hash_none or ec->tag == tag ) )
			{
				return true;
			}
		}

		return false;
	}

	// fills a vector with all the components that match the type

	[[nodiscard]] std::vector<Entity_Component*> get_components( e_component_type type, hash tag = hash_none ) const
	{
		std::vector<Entity_Component*> ecs;
		ecs.reserve( this->components.size() );

		for( auto& ec : this->components )
		{
			if( ec.get()->component_type == type and ( tag == hash_none or ec->tag == tag ) )
			{
				ecs.push_back( ec.get() );
			}
		}

		return ecs;
	}

	// returns the first component that matches the type

	[[nodiscard]] Entity_Component* get_component( e_component_type type, hash tag = hash_none ) const
	{
		for( auto& ec : this->components )
		{
			if( ec.get()->component_type == type and ( tag == hash_none or ec->tag == tag ) )
			{
				return ec.get();
			}
		}

		return nullptr;
	}

	virtual void on_box2d_collision_begin( box2d::Pending_Collision& coll, Entity* touched_by );
	virtual void on_box2d_collision_end( box2d::Pending_Collision& coll, Entity* touched_by );

	virtual bool on_collided( collision::Pending_Collision& coll );

	bool add_sensor_to_touch_list( Collision_Body_Component* sensor );

	virtual bool on_touching_begin( Collision_Body_Component* sensor );
	virtual bool on_touching( Collision_Body_Component* sensor );
	virtual bool on_touching_end( Collision_Body_Component* sensor );

	void reflect_across( Vec2 normal );

	virtual void apply_movement_jump();
	virtual void apply_movement_walk( Vec2 delta, f32 speed );

	Rect compute_ws_aabb() const;

private:
	Box2D_Physics_Body_Component* find_primary_box2d_body() const;
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

struct Entity_Transient : Entity
{
	virtual void update() override;
};

}
