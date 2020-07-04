
#include "master_pch.h"
#include "master_header.h"

w_render_stats::w_render_stats()
{
	stat_timer = std::make_unique<w_timer>( 1000 );
}

void w_render_stats::update()
{
	if( stat_timer->is_elapsed() )
	{
		stat_timer->reset();

		frame_count.update_value();
		int steps = static_cast<int>( frame_count.value );

		frame_times_ms.update_value( steps );
		render_buffers.update_value( steps );
		render_vertices.update_value( steps );
		render_indices.update_value( steps );
		num_entities.update_value( steps );
		num_frames_rendered.update_value();
	}
	else
	{
		stat_timer->update();
	}
}

// ----------------------------------------------------------------------------

void w_render::init()
{
	// MODEL MATRIX (getting stuff into worldspace from model space)
	//
	// push an identity matrix onto the stack. this will always live
	// as the first element in the stack and should be the only
	// matrix remaining when the frame finishes rendering.
	MATRIX
		->push_identity();

	// initialize render state stacks
	rs_color_stack.push( W_COLOR_WHITE );
	rs_scale_stack.push( 1.0f );

	// generate the sample points for drawing a circle. these verts sit
	// on a unit circle and are scaled to match the radius requesed for
	// each circle draw.
	float angle = 0;
	float angle_step = 360.0f / static_cast<float>( circle_sample_points_max );
	for( int x = 0; x < circle_sample_points_max; ++x )
	{
		circle_sample_points[x] = w_vec2::from_angle( angle );
		angle += angle_step;
	}
}

w_render* w_render::begin()
{
	rs_color_count = 0;

	return this;
}

w_render* w_render::push_color( const w_color& color )
{
	rs_color_count++;
	rs_color_stack.push( color );

	return this;
}

w_render* w_render::push_scale( const float& scale )
{
	rs_scale_count++;
	rs_scale_stack.push( scale );

	return this;
}

void w_render::end()
{
	while( rs_color_count )
	{
		rs_color_stack.pop();
		rs_color_count--;
	}
	rs_color_count = 0;

	while( rs_scale_count )
	{
		rs_scale_stack.pop();
		rs_scale_count--;
	}
	rs_scale_count = 0;

}

/*
	draws a texture as a sprite onto the screen.

	this offsets along left and up by half the texture size, which
	centers the quad being drawn at 0,0,0.
*/
w_render* w_render::draw_sprite( a_texture* tex, const w_sz& sz )
{
	return draw_sprite( tex->get_subtexture(), sz );
}

w_render* w_render::draw_sprite( a_subtexture * subtex, const w_sz & sz )
{
	float w = ( sz.w == -1 ) ? subtex->sz.w : sz.w;
	float h = ( sz.h == -1 ) ? subtex->sz.h : sz.h;
	
	float rs_scale = rs_scale_stack.top();

	w *= rs_scale;
	h *= rs_scale;

	float hw = w / 2.0f;
	float hh = h / 2.0f;

	w_color rs_color = rs_color_stack.top();

	w_render_vert v0( w_vec3( -hw, hh, 0.0f ), w_vec2( subtex->uv00.u, subtex->uv11.v ), rs_color );
	w_render_vert v1( w_vec3( hw, hh, 0.0f ), w_vec2( subtex->uv11.u, subtex->uv11.v ), rs_color );
	w_render_vert v2( w_vec3( hw, -hh, 0.0f ), w_vec2( subtex->uv11.u, subtex->uv00.v ), rs_color );
	w_render_vert v3( w_vec3( -hw, -hh, 0.0f ), w_vec2( subtex->uv00.u, subtex->uv00.v ), rs_color );

	subtex->tex->render_buffer->add_quad( v0, v1, v2, v3 );

	return this;
}

/*
	draws a texture onto a quad.
*/
w_render* w_render::draw( a_texture* tex, const w_sz& sz )
{
	return draw( tex->get_subtexture(), sz );
}

w_render* w_render::draw( a_subtexture* subtex, const w_sz& sz )
{
	float w = ( sz.w == -1 ) ? subtex->sz.w : sz.w;
	float h = ( sz.h == -1 ) ? subtex->sz.h : sz.h;

	float rs_scale = rs_scale_stack.top();

	w *= rs_scale;
	h *= rs_scale;

	w_color rs_color = rs_color_stack.top();

	w_render_vert v0( w_vec3( 0, h, 0 ), w_vec2( subtex->uv00.u, subtex->uv11.v ), rs_color );
	w_render_vert v1( w_vec3( w, h, 0 ), w_vec2( subtex->uv11.u, subtex->uv11.v ), rs_color );
	w_render_vert v2( w_vec3( w, 0, 0 ), w_vec2( subtex->uv11.u, subtex->uv00.v ), rs_color );
	w_render_vert v3( w_vec3( 0, 0, 0 ), w_vec2( subtex->uv00.u, subtex->uv00.v ), rs_color );

	subtex->tex->render_buffer->add_quad( v0, v1, v2, v3 );

	return this;
}

/*
	draws a string from a bitmap font, char by char
*/
w_render* w_render::draw_string( a_font* font, w_vec3 pos, const std::string& text, e_align align )
{
	const char* rd_ptr = text.c_str();
	float xpos = pos.x;
	float ypos = pos.y;

	w_vec2 extents;

	if( (align & e_align::hcenter) > 0 )
	{
		extents = font->get_string_extents( text );
		xpos = pos.x - ( extents.x / 2.0f );
	}
	else if( (align & e_align::right) > 0 )
	{
		extents = font->get_string_extents( text );
		xpos = pos.x - extents.x;
	}

	if( (align & e_align::vcenter) > 0 )
	{
		ypos = pos.y + ( font->font_def->max_height / 2.0f );
	}

	w_font_char* fch;
	std::string str = text;
	std::string token;
	w_tokenizer tok( str, '}' );

	for( const auto& iter : str )
	{
		fch = &( font->font_def->char_map[ iter ] );

		if( iter == '\n' )
		{
			xpos = pos.x;
			ypos -= font->font_def->max_height;
		}
		else
		{
			// small optimization to skip drawing completely blank characters
			if( fch->w > 0 )
			{
				MATRIX
					->push()
					->translate( w_vec3( xpos + fch->xoffset, ypos - fch->h - fch->yoffset, pos.z ) );
				draw(
					fch->img.get(),
					w_vec2( fch->w, fch->h )
				);
				MATRIX
					->pop();
			}

			xpos += fch->xadvance;
		}
	}

	return this;
}

/*
	call at the start of each frame to set up and clear the screen
*/
void w_render::begin_frame( float frame_interpolate_pct )
{
	this->frame_interpolate_pct = frame_interpolate_pct;

	glClearColor( .25f, .25f, .25f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// reset all render buffers

	for( const auto& iter : engine->asset_cache->cache )
	{
		iter.second->clear_render_buffer();
	}

	engine->shader->bind();
}

/*
	call at end of frame to finalize frame and render all buffers
*/
void w_render::end_frame()
{
	//if( show_stats )
	{
		draw_stats();
	}

	// draw all render buffers

	for( const auto& iter : engine->asset_cache->cache )
	{
		iter.second->draw( e_render_pass::opaque );
	}

	for( const auto& iter : engine->asset_cache->cache )
	{
		glDepthMask( GL_FALSE );
		iter.second->draw( e_render_pass::transparent );
	}
	glDepthMask( GL_TRUE );

	// update stats
	stats.update();

	// accum stats
	stats.frame_times_ms.accum( static_cast<float>( engine->time->delta_ms ) );
	stats.frame_count.inc();

	// when the frame ends, there should be
	// a single matrix left on the stack (the identity matrix we created
	// at the renderer start up). If there are any other number, then
	// there is an uneven push/pop combo somewhere in the code.

	assert( OPENGL->modelview_stack.size() == 1 );

	// verify that state stacks are back where they started. if not,
	// it means there's a push/pop mismatch somewhere in the code.
	assert( rs_color_stack.size() == 1 );
	assert( rs_scale_stack.size() == 1 );

	// Swap buffers
	glfwSwapBuffers( engine->window->window );

	OPENGL->clear_texture_bind();
	stats.num_frames_rendered.inc();
}

/*
	draws the X and Y axis in the viewport at the world origin

	positive directions are brighter than the negative ones
*/

w_render* w_render::draw_world_axis()
{
	push_color( w_color( 1.0f, 0.0f, 0.0f ) );
	draw_line( w_vec3::zero, w_vec3( 5000, 0, 500 ) );

	push_color( w_color( 0.5f, 0.0f, 0.0f ) );
	draw_line( w_vec3::zero, w_vec3( -5000, 0, 500 ) );

	push_color( w_color( 0.0f, 1.0f, 0.0f ) );
	draw_line( w_vec3::zero, w_vec3( 0, 5000, 500 ) );

	push_color( w_color( 0.0f, 0.5f, 0.0f ) );
	draw_line( w_vec3::zero, w_vec3( 0, -5000, 500 ) );

	return this;
}

/*
	draws useful stats at the top of the screen

	it does this at the world origin, so make sure the camera
	transform is reset before calling this
*/

constexpr int stats_draw_reserve = 10;
w_render* w_render::draw_stats()
{
	auto font = engine->get_asset<a_font>( "ui_simple_font" );

	if( show_stats )
	{
		std::vector<std::string> stat_lines;
		stat_lines.reserve( stats_draw_reserve );

		stat_lines.emplace_back( s_format( "RENDER : %s FPS / UPDATE : %d FPS",
										   s_commas( (int) stats.num_frames_rendered.value, "%d" ).c_str(),
										   (int) w_time::FTS_desired_frames_per_second ) );
		stat_lines.emplace_back( s_format( "RB: %s, V: %s, I: %s",
										   s_commas( stats.render_buffers.value, "%0.f" ).c_str(),
										   s_commas( stats.render_vertices.value, "%0.f" ).c_str(),
										   s_commas( stats.render_indices.value, "%0.f" ).c_str() )
		);
		stat_lines.emplace_back( s_format( "Layers : %d", engine->layer_mgr->layer_stack.size() ) );
		stat_lines.emplace_back( s_format( "Entities : %s", s_commas( stats.num_entities.value, "%0.f" ).c_str() ) );
		stat_lines.emplace_back( s_format( "Time Dilation: %.2f", engine->time->dilation ) );

		if( stats.stat_custom_string.length() )
		{
			stat_lines.emplace_back( stats.stat_custom_string );
			stats.stat_custom_string = "";
		}

		assert( stat_lines.size() < stats_draw_reserve );

		RENDER->begin();
		{
			RENDER
				->push_color( w_color( .25f, .25f, .25f, 0.75f ) )
				->draw_filled_rectangle( w_vec2( -v_window_hw, v_window_hh ), w_vec2( v_window_hw, v_window_hh - ( font->font_def->max_height * stat_lines.size() ) ), 999.0f );

			RENDER->push_color( W_COLOR_WHITE );

			float y = v_window_hh;
			for( const auto& iter : stat_lines )
			{
				RENDER->draw_string( font, w_vec3( 0, y, 1000.0f ), iter.c_str(), e_align::hcenter );
				y -= font->font_def->max_height;
			}
		}
		RENDER->end();
	}
	else
	{
	#if !defined(FINALRELEASE)
		std::string fps_stats( s_format( "%s FPS", s_commas( (int)stats.num_frames_rendered.value, "%d" ).c_str() ) );

		RENDER->begin()
			->draw_string( font, w_vec3( v_window_hw, v_window_hh, 1000.0f ), fps_stats, e_align::right )
			->end();
	#endif
	}

	return this;
}

w_render* w_render::draw_filled_rectangle( w_vec2 start, w_vec2 end, float z )
{
	w_color rs_color = rs_color_stack.top();

	w_render_vert v0(
		w_vec3( start.x, start.y, z ),
		w_uv( 0, 0 ),
		rs_color
	);
	w_render_vert v1(
		w_vec3( end.x, start.y, z ),
		w_uv( 1, 0 ),
		rs_color
	);
	w_render_vert v2(
		w_vec3( end.x, end.y, z ),
		w_uv( 1, 1 ),
		rs_color
	);
	w_render_vert v3(
		w_vec3( start.x, end.y, z ),
		w_uv( 0, 1 ),
		rs_color
	);

	engine->white_solid->tex->render_buffer->add_quad( v0, v1, v2, v3 );

	return this;
}

// draws an empty rectangle

w_render* w_render::draw_rectangle( w_rect rc_dst )
{
	// warren
	assert( false );// not implemented yet
	w_bbox box;
	box.add( w_vec3(
		static_cast<float>( rc_dst.x ),
		static_cast<float>( rc_dst.y ),
		0.0f )
	);
	box.add( w_vec3(
		static_cast<float>( rc_dst.x + rc_dst.w ),
		static_cast<float>( rc_dst.y + rc_dst.h ),
		0.0f )
	);

	draw_line( w_vec3( box.min.x, box.min.y, 0.0f ), w_vec3( box.max.x, box.min.y, 0.0f ) );
	draw_line( w_vec3( box.max.x, box.min.y, 0.0f ), w_vec3( box.max.x, box.max.y, 0.0f ) );
	draw_line( w_vec3( box.max.x, box.max.y, 0.0f ), w_vec3( box.min.x, box.max.y, 0.0f ) );
	draw_line( w_vec3( box.min.x, box.max.y, 0.0f ), w_vec3( box.min.x, box.min.y, 0.0f ) );

	return this;
}

// draws a circle with line segments

w_render* w_render::draw_circle( w_vec3 origin, float radius )
{
	w_color rs_color = rs_color_stack.top();
	w_render_vert v0( origin, w_uv( 0, 0 ), rs_color );
	w_render_vert v1( origin, w_uv( 0, 0 ), rs_color );

	for( int x = 0; x < circle_sample_points_max; ++x )
	{
		v0.x = circle_sample_points[x].x * radius;
		v0.y = circle_sample_points[x].y * radius;

		v1.x = circle_sample_points[( x + 1 ) % circle_sample_points_max].x * radius;
		v1.y = circle_sample_points[( x + 1 ) % circle_sample_points_max].y * radius;

		engine->white_wire->tex->render_buffer->add_line( v0, v1 );
	}

	return this;
}

// draws a line

w_render* w_render::draw_line( w_vec3 start, w_vec3 end )
{
	w_color rs_color = rs_color_stack.top();
	w_render_vert v0( start, w_uv( 0, 0 ), rs_color );
	w_render_vert v1( end, w_uv( 0, 0 ), rs_color );

	engine->white_wire->tex->render_buffer->add_line( v0, v1 );

	return this;
}

w_render* w_render::draw_sliced( a_texture* tex, const std::string& patch_name, w_rect rc_dst, float z )
{
	return draw_sliced( tex->get_subtexture(), patch_name, rc_dst, z );
}

w_render* w_render::draw_sliced( a_subtexture* subtex, const std::string& patch_name, w_rect rc_dst, float z )
{
	// warren
	assert( false );// not implemented yet
	
	a_9slice_def* slice_def = engine->get_asset<a_9slice_def>( patch_name.c_str() );
	assert( slice_def );

	float xpos;
	float ypos;
	float dst_w;
	float dst_h;

	// top row

	xpos = rc_dst.x;
	ypos = rc_dst.y;

	dst_w = slice_def->patches[(int)e_patch::P_00].w;
	dst_h = slice_def->patches[(int)e_patch::P_00].h;
	//draw_sub_texture( texture->get_texture(), slice_def->patches[(int)e_patch::P_00], w_rect( xpos, ypos, dst_w, dst_h ), z );

	xpos += dst_w;
	dst_w = rc_dst.w - slice_def->patches[(int)e_patch::P_00].w - slice_def->patches[(int)e_patch::P_20].w;
	//draw_sub_texture( texture->get_texture(), slice_def->patches[(int)e_patch::P_10], w_rect( xpos, ypos, dst_w, dst_h ), z );

	xpos += dst_w;
	dst_w = slice_def->patches[(int)e_patch::P_20].w;
	//draw_sub_texture( texture->get_texture(), slice_def->patches[(int)e_patch::P_20], w_rect( xpos, ypos, dst_w, dst_h ), z );

	// middle row

	xpos = rc_dst.x;
	ypos += dst_h;

	dst_w = slice_def->patches[(int)e_patch::P_01].w;
	dst_h = rc_dst.h - slice_def->patches[(int)e_patch::P_00].h - slice_def->patches[(int)e_patch::P_02].h;
	//draw_sub_texture( texture->get_texture(), slice_def->patches[(int)e_patch::P_01], w_rect( xpos, ypos, dst_w, dst_h ), z );

	xpos += dst_w;
	dst_w = rc_dst.w - slice_def->patches[(int)e_patch::P_01].w - slice_def->patches[(int)e_patch::P_21].w;
	//draw_sub_texture( texture->get_texture(), slice_def->patches[(int)e_patch::P_11], w_rect( xpos, ypos, dst_w, dst_h ), z );

	xpos += dst_w;
	dst_w = slice_def->patches[(int)e_patch::P_21].w;
	//draw_sub_texture( texture->get_texture(), slice_def->patches[(int)e_patch::P_21], w_rect( xpos, ypos, dst_w, dst_h ), z );

	// bottom row

	xpos = rc_dst.x;
	ypos += dst_h;

	dst_w = slice_def->patches[(int)e_patch::P_02].w;
	dst_h = slice_def->patches[(int)e_patch::P_02].h;
	//draw_sub_texture( texture->get_texture(), slice_def->patches[(int)e_patch::P_02], w_rect( xpos, ypos, dst_w, dst_h ), z );

	xpos += dst_w;
	dst_w = rc_dst.w - slice_def->patches[(int)e_patch::P_02].w - slice_def->patches[(int)e_patch::P_22].w;
	//draw_sub_texture( texture->get_texture(), slice_def->patches[(int)e_patch::P_12], w_rect( xpos, ypos, dst_w, dst_h ), z );

	xpos += dst_w;
	dst_w = slice_def->patches[(int)e_patch::P_22].w;
	//draw_sub_texture( texture->get_texture(), slice_def->patches[(int)e_patch::P_22], w_rect( xpos, ypos, dst_w, dst_h ), z );

	return this;
}

void w_render::init_projection()
{
	// setting this stuff up one time as we only use a single shader and the camera never moves.
	//
	// as we get fancier, parts of this may have to move to the start of each rendering frame.

	engine->shader->bind();

	// PROJECTION MATRIX (getting stuff into screen space from camera space)

	glm::mat4 projection = glm::mat4( 1.0f );
	projection = glm::ortho<float>(
		0, v_window_w, -v_window_h, 0,
		-1000.0f, 1000.0f );

	glUniformMatrix4fv( glGetUniformLocation( engine->shader->id, "P" ), 1, GL_FALSE, glm::value_ptr( projection ) );

	// VIEW MATRIX (getting stuff into camera space from worldspace)

	glm::mat4 view = glm::mat4( 1.0f );
	view = glm::translate( view, glm::vec3( v_window_hw, -v_window_hh, 0.0f ) );
	glUniformMatrix4fv( glGetUniformLocation( engine->shader->id, "V" ), 1, GL_FALSE, glm::value_ptr( view ) );

	engine->shader->unbind();
}
