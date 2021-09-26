
namespace war
{

struct Spatial_Map final
{
	// ----------------------------------------------------------------------------

	struct Node final
	{
		// where this node lies in world space
		Rect bounds = {};

		// the entities that are inside this node
		std::set<Entity*> entities;

		Node() = default;
		Node( const Rect& rc );

		void debug_draw() const;
	};

	// ----------------------------------------------------------------------------

	Scene* parent_scene = nullptr;
	std::vector<Spatial_Map::Node> nodes;

	void init( const Rect& bounds, int32_t subd_count );
	void debug_draw() const;
	std::vector<Spatial_Map::Node*> get_nodes_entity_is_touching( Entity* e );

	void pre_update();
	void update();
	void post_update();

protected:
	void insert_entity( Entity* e );
};

}
