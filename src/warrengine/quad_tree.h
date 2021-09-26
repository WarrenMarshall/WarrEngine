
namespace war
{

struct Quad_Tree final
{
	// ----------------------------------------------------------------------------

	struct Node final
	{
		Rect bounds = {};

		std::vector<Node> children;

		void debug_draw() const;
	};

	// ----------------------------------------------------------------------------

	Node head;

	void set_bounds( const Rect& bounds );
	void debug_draw() const;
};

}
