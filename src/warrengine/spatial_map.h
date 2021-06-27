
namespace war
{

// ----------------------------------------------------------------------------

struct spatial_map_cell
{
	spatial_map_cell( rect rc_ws );

	rect rc_ws;
};

// ----------------------------------------------------------------------------

struct spatial_map
{
	std::vector<spatial_map_cell> cells;
	size_t w = 0;
	size_t h = 0;

	void init( rect rc_ws, size_t w, size_t h );
	void draw();
};

}
