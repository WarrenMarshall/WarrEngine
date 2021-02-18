#pragma once

struct w_matrix
{
	w_matrix() = default;

	w_matrix* set_identity();
	w_matrix* add_transform( const w_transform& transform );
	w_matrix* add_transform( const w_matrix& matrix );
	w_matrix* add_transform( const w_vec2& pos, const float angle, const float _scale );
	w_matrix* translate( const w_vec2& v );
	w_matrix* scale( const float v );
	w_matrix* scale( const float x, const float y );
	w_matrix* rotate( const float v );

	_NODISCARD w_vec2 transform( const w_vec2& v ) const;
	_NODISCARD w_vec2 inverse_transform( const w_vec2& v ) const;
private:
	glm::mat4 m = glm::mat4( 1 );

};
