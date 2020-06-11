#pragma once

/*
	blending func cheat sheet:

	Alpha Blend:	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	Add:			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	Multiply:		glBlendFunc( GL_DST_COLOR, GL_ZERO );

*/

// ----------------------------------------------------------------------------

struct w_opengl
{
	std::stack<glm::mat4> modelview_stack;
	glm::mat4 mtx_modelview;

	void init();
	void deinit();

	void push_matrix();
	void push_identity_matrix();
	void pop_matrix();
	glm::mat4* top_matrix();
	void add_transform( const i_transform& t );
	void translate( w_vec3 v );
	void scale( float v );
	void rotate( float v );

	void clear_texture_bind();

	void set_blend( e_opengl_blend blend );
	void refresh_primitive_sizes();
};
