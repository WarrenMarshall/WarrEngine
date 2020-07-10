#pragma once

struct w_matrix
{
	glm::mat4 m = glm::mat4(1);

	w_matrix() = default;

	w_matrix* add_transform( const i_transform& t );
	w_matrix* add_transform( const w_vec2& pos, const float& angle, const float& _scale );
	w_matrix* translate( const w_vec2& v );
	w_matrix* scale( float v );
	w_matrix* rotate( float v );
};
