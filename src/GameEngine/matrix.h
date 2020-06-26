#pragma once

struct w_matrix
{
	glm::mat4 m;

	static w_matrix make_identity();

	w_matrix* add_transform( const i_transform& t );
	w_matrix* add_transform( const w_vec3& pos, const float& angle, const float& _scale );
	w_matrix* translate( const w_vec3& v );
	w_matrix* scale( float v );
	w_matrix* rotate( float v );
};
