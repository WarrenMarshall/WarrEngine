#pragma once

/*
	blending func cheat sheet:

	Alpha Blend:	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	Add:			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	Multiply:		glBlendFunc( GL_DST_COLOR, GL_ZERO );

*/

struct w_opengl
{
	std::stack<w_matrix> modelview_stack;

	void init();
	void deinit();

	w_matrix* push( bool identity );
	w_matrix* top();
	void pop();

	void clear_texture_bind();

	void set_blend( e_opengl_blend blend );
	void refresh_primitive_sizes();
};
