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

	_NODISCARD w_vec2 transform_pos( const w_vec2& vtx );
	_NODISCARD w_vec2 inverse_transform_pos( const w_vec2& vtx );
	_NODISCARD w_vec2 transform_dir( const w_vec2& dir );
	_NODISCARD w_vec2 inverse_transform_vec( const w_vec2& dir );

	w_matrix matrix;
	w_matrix matrix_dir;
};
