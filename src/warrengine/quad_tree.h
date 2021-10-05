
namespace war
{

struct Quad_Tree final
{
	// ----------------------------------------------------------------------------

	struct Node final
	{
		// where this node lies in world space
		Rect bounds = {};

		// the entities that are inside this node
		std::set<Entity*> entities = {};

		Node() = default;
		Node( const Rect& rc );

		void debug_draw() const;
	};

	// ----------------------------------------------------------------------------

	// how many entities can be inside a node before it gets subdivided
	//
	// NOTE : it's best to keep this at 3 or higher. going too low creates a lot
	// of needless subdividing of the spatial grid. you hit diminishing returns
	// pretty quickly.

	int32_t max_entities_per_node = 3;

	// the smallest area that a node can be before we can't split it anymore.
	// this prevents nodes from getting super tiny. this is computed out as
	// (min_node_area.x * min_node_area.y).
	//
	// use different values for rectangular nodes or just a single value for
	// squares.
	//
	// NOTE : this is best set so that you have a reasonable number of
	// subdivides per frame. doing too many leads to diminishing returns pretty
	// quickly.

	Vec2 min_node_area = 64;

	Scene* parent_scene = nullptr;
	std::list<std::unique_ptr<Quad_Tree::Node>> nodes;
	Rect bounds = { -750, -750, 1'500, 1'500 };

	void init( const Rect& bounds );
	void reset();
	void debug_draw() const;

	[[nodiscard]] std::set<Quad_Tree::Node*> get_nodes_entity_is_touching( Entity* e ) const;
	[[nodiscard]] std::set<Quad_Tree::Node*> get_nodes_circle_is_touching( const Vec2& pos, float_t radius ) const;
	[[nodiscard]] std::set<Quad_Tree::Node*> get_nodes_rect_is_touching( const Rect& rc_aabb ) const;

	[[nodiscard]] std::set<Entity*> find_potentially_colliding_entities( Entity* e ) const;
	[[nodiscard]] std::set<Entity*> find_potentially_colliding_entities( const Vec2& pos, float_t radius ) const;
	[[nodiscard]] std::set<Entity*> find_potentially_colliding_entities( const Rect& rc_aabb ) const;

	void pre_update();
	void update();
	void post_update();

protected:
	void insert_entity( Entity* e );
	void subdivide_nodes_as_necessary();
};

}
