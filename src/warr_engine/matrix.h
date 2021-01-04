#pragma once

struct w_matrix
{
	glm::mat4 m = glm::mat4(1);

	w_matrix() = default;

	w_matrix* set_identity();
	w_matrix* add_transform( const w_transform& transform );
	w_matrix* add_transform( const w_matrix& matrix );
	w_matrix* add_transform( const w_vec2& pos, const float angle, const float _scale );
	w_matrix* translate( const w_vec2& v );
	w_matrix* scale( float v );
	w_matrix* scale( float x, float y );
	w_matrix* rotate( float v );
	[[nodiscard]] w_vec2 transform_vec2( const w_vec2& vtx );
};
