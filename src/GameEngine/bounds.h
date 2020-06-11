#pragma once

// ----------------------------------------------------------------------------
// bounds are stored as if they are at the world origin. they don't take
// the entities world position into account.
// ----------------------------------------------------------------------------

struct w_bounds : i_transform
{
	w_bounds();

	virtual bool contains_pt( w_vec3 world_space_pos_to_check );
	virtual bool is_intersecting( w_bounds other );
	virtual void draw_bounds();
};

// ----------------------------------------------------------------------------

struct w_bounds_circle : w_bounds
{
	w_bounds_circle( int radius );

	int radius = 0;

	virtual bool contains_pt( w_vec3 world_space_pos_to_check );
	virtual bool is_intersecting( w_bounds other );
	virtual void draw_bounds();
};

// ----------------------------------------------------------------------------

struct w_bounds_bbox : w_bounds
{
	w_bounds_bbox();

	w_bbox box;
	virtual void draw_bounds();
};
