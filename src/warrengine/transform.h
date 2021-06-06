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
	w_transform* set_pos( w_vec2 pos );
	w_transform* set_angle( float angle );
	w_transform* set_scale( float scale );

	// mutate transforms

	w_transform* add_pos_delta( w_vec2 delta );
	w_transform* add_angle_delta( float delta );
	w_transform* add_scale_delta( float delta );

	void rebuild_matrix();

	[[nodiscard]] w_vec2 transform( const w_vec2& vtx );
	[[nodiscard]] w_vec2 inverse_transform( const w_vec2& vtx );

	w_matrix matrix;
	w_matrix matrix_dir;
};
