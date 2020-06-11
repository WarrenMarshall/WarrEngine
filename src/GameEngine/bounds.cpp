
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_bounds::w_bounds()
	: i_transform()
{
}

bool w_bounds::contains_pt( w_vec3 world_space_pos_to_check )
{
	return false;
}

bool w_bounds::is_intersecting( w_bounds other )
{
	return false;
}

void w_bounds::draw_bounds()
{

}

// ----------------------------------------------------------------------------

w_bounds_circle::w_bounds_circle( int radius )
	: w_bounds(), radius( radius )
{
}

bool w_bounds_circle::contains_pt( w_vec3 world_space_pos_to_check )
{
	//if( w_vec3::get_distance_between( parent->pos, world_space_pos_to_check ) < radius )
	//{
	//	return true;
	//}

	return false;
}

bool w_bounds_circle::is_intersecting( w_bounds other )
{
	return false;
}

void w_bounds_circle::draw_bounds()
{
	//SCOPED_VAR( W_COLOR_TEAL );
	//engine->render->draw_circle( parent->pos, radius );
}

// ----------------------------------------------------------------------------

w_bounds_bbox::w_bounds_bbox()
	: w_bounds()
{
}

void w_bounds_bbox::draw_bounds()
{
	//w_rect rc;

	//rc = box.to_w_rect();
	//rc.x += parent->pos.x;
	//rc.y += parent->pos.y;

	//SCOPED_VAR( W_COLOR_TEAL );
	//engine->render->draw_solid_rectangle( rc );
}
