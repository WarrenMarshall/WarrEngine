#pragma once

struct w_transform
{
	// position
	w_vec2 pos = w_vec2( 0, 0 );

	// rotation (around z axis)
	float angle = 0.0f;

	// scale
	float scale = 1.0f;

	w_transform();

	// set transforms directly

	w_transform* set( w_vec2 pos, float angle, float scale );
	w_transform* set_position( w_vec2 pos );
	w_transform* set_angle( float angle );
	w_transform* set_scale( float scale );

	// mutate transforms

	w_transform* add_position_delta( w_vec2 delta );
	w_transform* add_angle_delta( float delta );
	w_transform* add_scale_delta( float delta );

	void rebuild_matrix();

	w_matrix matrix;
};
