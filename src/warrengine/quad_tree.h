
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

	int32_t max_entities_per_node = 3;

	// the smallest area that a node can be before we can't split it anymore.
	// this prevents nodes from getting super tiny. this is computed out as
	// (min_node_area.x * min_node_area.y).
	//
	// use different values for rectangular nodes or just a single value for
	// squares.

	Vec2 min_node_area = 16;

	Scene* parent_scene = nullptr;
	std::list<std::unique_ptr<Quad_Tree::Node>> nodes;
	Rect bounds;

	void init( const Rect& bounds );
	void reset();
	void debug_draw() const;
	[[nodiscard]] std::vector<Quad_Tree::Node*> get_nodes_entity_is_touching( Entity* e ) const;
	[[nodiscard]] std::vector<Quad_Tree::Node*> get_nodes_circle_is_touching( const Vec2& pos, float_t radius ) const;
	[[nodiscard]] std::vector<Quad_Tree::Node*> get_nodes_rect_is_touching( const Rect& rc_aabb ) const;
	[[nodiscard]] std::set<Entity*> get_potential_entity_colliding_set( Entity* e ) const;
	[[nodiscard]] std::set<Entity*> get_potential_entity_colliding_set( const Vec2& pos, float_t radius ) const;
	[[nodiscard]] std::set<Entity*> get_potential_entity_colliding_set( const Rect& rc_aabb ) const;

	void pre_update();
	void update();
	void post_update();

protected:
	void insert_entity( Entity* e );
	void subdivide_nodes_as_necessary();
};

}
