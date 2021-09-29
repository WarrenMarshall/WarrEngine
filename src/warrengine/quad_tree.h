
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

	int32_t max_entities_per_node = 3;
	int32_t min_node_area = 16 * 16;

	Scene* parent_scene = nullptr;
	std::list<std::unique_ptr<Quad_Tree::Node>> nodes;
	Rect bounds;

	void init( const Rect& bounds );
	void reset();
	void debug_draw() const;
	std::vector<Quad_Tree::Node*> get_nodes_entity_is_touching( Entity* e );
	std::set<Entity*> get_potential_entity_colliding_set( Entity* e );

	void pre_update();
	void update();
	void post_update();

protected:
	void insert_entity( Entity* e );
	void subdivide_nodes_as_necessary();
};

}
