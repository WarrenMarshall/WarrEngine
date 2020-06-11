
#include "master_pch.h"
#include "master_header.h"

void w_accum_value::init()
{
	value = accumulator = 0.0f;
}

void w_accum_value::update_value( int steps )
{
	value = accumulator / static_cast<float>( steps );
	accumulator = 0.0f;
}

void w_accum_value::update_value()
{
	value = accumulator;
	accumulator = 0.0f;
}

void w_accum_value::accum( float value )
{
	accumulator += value;
}

void w_accum_value::inc()
{
	accumulator += 1.0f;
}

// ----------------------------------------------------------------------------

w_render_stats::w_render_stats()
{
	stat_timer = std::make_unique<w_timer>( 1000 );
	stat_timer->use_fts_update = false;
}

void w_render_stats::update()
{
	stat_timer->update();

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
		num_frames_rendered.update_value( steps );
	}
}

// ----------------------------------------------------------------------------

void w_render::init()
{
	// MODEL MATRIX (getting stuff into worldspace from model space)
	//
	// push an identity matrix onto the stack. this will always live
	// as the last element in the stack and should be the only
	// matrix remaining when the frame finishes rendering.
	engine->opengl->push_identity_matrix();

	float angle = 0;
	float angle_step = 360.0f / static_cast<float>( circle_sample_points_max );
	for( int x = 0; x < circle_sample_points_max; ++x )
	{
		circle_sample_points[x] = w_vec2::from_angle( angle );
		angle += angle_step;
	}
}

/*
	draws a texture as a sprite onto the screen.

	this offsets along left and up by half the texture size, which
	centers the quad being drawn at 0,0,0.
*/
void w_render::draw_sprite( a_texture* texture, const w_color& color )
{
	float hw = texture->get_texture()->w / 2.0f;
	float hh = texture->get_texture()->h / 2.0f;

	w_render_vert v0( w_vec3( -hw, hh, 0.0f ), w_vec2( 0, 1 ), color );
	w_render_vert v1( w_vec3( hw, hh, 0.0f ), w_vec2( 1, 1 ), color );
	w_render_vert v2( w_vec3( hw, -hh, 0.0f ), w_vec2( 1, 0 ), color );
	w_render_vert v3( w_vec3( -hw, -hh, 0.0f ), w_vec2( 0, 0 ), color );

	texture->get_texture()->render_buffer->add_quad( v0, v1, v2, v3 );
}

/*
	draws a texture onto a quad.

	rc_src - defines a sub region within the texture.  good for sprite sheets, ui graphics, and fonts.
*/
void w_render::draw( a_texture* texture, const w_color& color, w_vec2 sz, w_rect rc_src )
{
	a_texture* tex = texture->get_texture();

	float w = ( sz.w == -1 ) ? tex->w : sz.w;
	float h = ( sz.h == -1 ) ? tex->h : sz.h;

	w_render_vert v0( w_vec3( 0, h, 0 ), w_vec2( rc_src.u0, rc_src.v1 ), color );
	w_render_vert v1( w_vec3( w, h, 0 ), w_vec2( rc_src.u1, rc_src.v1 ), color );
	w_render_vert v2( w_vec3( w, 0, 0 ), w_vec2( rc_src.u1, rc_src.v0 ), color );
	w_render_vert v3( w_vec3( 0, 0, 0 ), w_vec2( rc_src.u0, rc_src.v0 ), color );

	tex->render_buffer->add_quad( v0, v1, v2, v3 );
}

/*
	draws a string from a bitmap font, char by char
*/
void w_render::draw_string( a_font* font, w_vec3 pos, const std::string& text, e_align align, w_color color )
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

	std::stack<w_color> color_stack;
	color_stack.push( color );

	w_font_char* fch;
	std::string str = text;
	std::string token;
	w_tokenizer tok( str, '}' );
	for( int x = 0 ; x < str.length() ; ++x )
	{
		fch = font->font_def->char_map[text[x]].get();

		if( text[x] == '\n' )
		{
			xpos = pos.x;
			ypos -= font->font_def->max_height;
		}
		else if( text[x] == '{' )
		{
			tok.idx = x + 1;
			token = tok.get_next_token();

			if( token.length() )
			{
				w_color clr = engine->find_color_from_symbol( token );
				color_stack.push( clr );
				x += static_cast<int>( token.length() ) + 1;
			}
			else
			{
				color_stack.pop();
				x++;
			}
		}
		else
		{
			// small optimization to skip drawing completely blank characters
			if( fch->w > 0 )
			{
				engine->opengl->push_matrix();
				engine->opengl->translate( w_vec3( xpos + fch->xoffset, ypos - fch->h - fch->yoffset, pos.z ) );
				draw(
					font->tex,
					color_stack.top(),
					w_vec2( fch->w, fch->h ),
					w_rect(
						fch->uv00.u,
						fch->uv00.v,
						fch->uv11.u,
						fch->uv11.v
					)
				);
				engine->opengl->pop_matrix();
			}

			xpos += fch->xadvance;
		}
	}
}

/*
	call at the start of each frame to set up and clear the screen
*/
void w_render::begin()
{
	glClearColor( .25f, .25f, .25f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// reset all render buffers

	for( auto& iter : engine->asset_cache->cache )
	{
		iter.second->clear_render_buffer();
	}

    engine->shader->bind();

	// set up baseline opengl matrices

	// PROJECTION MATRIX (getting stuff into screen space from camera space)

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho<float>(
		0, v_window_w, -v_window_h, 0,
		-1000.0f, 1000.0f );

	glUniformMatrix4fv( glGetUniformLocation( engine->shader->id, "P" ), 1, GL_FALSE, glm::value_ptr( projection ) );

	// VIEW MATRIX (getting stuff into camera space from worldspace)

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate( view, glm::vec3( v_window_hw, -v_window_hh, 0.0f ) );
	glUniformMatrix4fv( glGetUniformLocation( engine->shader->id, "V" ), 1, GL_FALSE, glm::value_ptr( view ) );
}

/*
	call at end of frame to finalize frame and present it
*/
void w_render::end()
{
	if( show_stats )
	{
		draw_stats();
	}

	// draw all render buffers

	for( auto& iter : engine->asset_cache->cache )
	{
		iter.second->draw( e_render_pass::opaque );
	}

	for( auto& iter : engine->asset_cache->cache )
	{
		glDepthMask( GL_FALSE );
		iter.second->draw( e_render_pass::transparent );
	}
	glDepthMask( GL_TRUE );

	// update stats

	stats.update();

	// update the window title to show the FPS and milliseconds per frame
	std::string new_title = s_format( "%s - %0.f FPS", engine->window->base_title.c_str(), stats.frame_count.value );
	engine->window->set_title( new_title );

	stats.frame_times_ms.accum( engine->time->delta_ms );
	stats.frame_count.inc();

	// when the frame ends and we do the final matrix pop, there should be
	// a single matrix left on the stack (the identity matrix we created
	// at the renderer start up). If there are any other number, then
	// there is an uneven push/pop combo somewhere in the code.

	assert( engine->opengl->modelview_stack.size() == 1 );

	engine->opengl->clear_texture_bind();
	stats.num_frames_rendered.inc();
}

/*
	draws the X and Y axis in the viewport at the world origin

	positive directions are brighter than the negative ones
*/

void w_render::draw_world_axis()
{
	SCOPED_VAR( rs_color( w_color(1.0f,0.0f,0.0f) ) );
	draw_line( w_vec3( 0, 0, 0 ), w_vec3( 5000, 0, 500 ) );
	
	SCOPED_VAR( rs_color( w_color(0.5f,0.0f,0.0f) ) );
	draw_line( w_vec3( 0, 0, 0 ), w_vec3( -5000, 0, 500 ) );
	
	SCOPED_VAR( rs_color( w_color(0.0f,1.0f,0.0f) ) );
	draw_line( w_vec3( 0, 0, 0 ), w_vec3( 0, 5000, 500 ) );
	
	SCOPED_VAR( rs_color( w_color(0.0f,0.5f,0.0f) ) );
	draw_line( w_vec3( 0, 0, 0 ), w_vec3( 0, -5000, 500 ) );
}

/*
	draws useful stats at the top of the screen

	it does this at the world origin, so make sure the camera
	transform is reset before calling this
*/

constexpr int stats_draw_reserve = 10;
void w_render::draw_stats()
{
	std::vector<std::string> stat_lines;
	stat_lines.reserve( stats_draw_reserve );

	stat_lines.emplace_back( s_format( "%0.f FPS", stats.frame_count.value ) );
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
		stat_lines.push_back( stats.stat_custom_string );
		stats.stat_custom_string = "";
	}

	assert( stat_lines.size() < stats_draw_reserve );

	auto font = engine->get_asset<a_font>( "ui_simple_font" );
	draw_filled_rectangle( w_vec2( -v_window_hw, v_window_hh ), w_vec2( v_window_hw, v_window_hh - ( font->font_def->max_height * stat_lines.size() ) ), 5.0f, w_color( .25f, .25f, .25f, 0.75f ) );

	float y = v_window_hh;
	for( auto& iter : stat_lines)
	{
		draw_string( font, w_vec3( 0, y, 1000.0f ), iter.c_str(), e_align::hcenter );
		y -= font->font_def->max_height;
	}
}

void w_render::draw_filled_rectangle( w_vec2 start, w_vec2 end, float z, w_color color )
{
	w_render_vert v0(
		w_vec3( start.x, start.y, z ),
		w_uv( 0, 0 ),
		color
	);
	w_render_vert v1(
		w_vec3( end.x, start.y, z ),
		w_uv( 1, 0 ),
		color
	);
	w_render_vert v2(
		w_vec3( end.x, end.y, z ),
		w_uv( 1, 1 ),
		color
	);
	w_render_vert v3(
		w_vec3( start.x, end.y, z ),
		w_uv( 0, 1 ),
		color
	);

	engine->white_solid->render_buffer->add_quad( v0, v1, v2, v3 );
}

// draws an empty rectangle

void w_render::draw_rectangle( w_rect rc_dst )
{
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
}

// draws a circle with line segments

void w_render::draw_circle( w_vec3 origin, float radius )
{
	w_render_vert v0( origin, w_uv( 0, 0 ), current_color );
	w_render_vert v1( origin, w_uv( 0, 0 ), current_color );

	for( int x = 0; x < circle_sample_points_max; ++x )
	{
		v0.x = circle_sample_points[x].x * radius;
		v0.y = circle_sample_points[x].y * radius;

		v1.x = circle_sample_points[( x + 1 ) % circle_sample_points_max].x * radius;
		v1.y = circle_sample_points[( x + 1 ) % circle_sample_points_max].y * radius;

		engine->white_wire->render_buffer->add_line( v0, v1 );
	}
}

// draws a line

void w_render::draw_line( w_vec3 start, w_vec3 end )
{
	w_render_vert v0( start, w_uv( 0, 0 ), current_color );
	w_render_vert v1( end, w_uv( 0, 0 ), current_color );

	engine->white_wire->render_buffer->add_line( v0, v1 );
}

void w_render::draw_sliced_texture( a_texture* texture, const std::string& patch_name, w_rect rc_dst, float z, w_color color )
{
	assert( false );// not implemented yet
	a_9slice_def* slice_def = engine->get_asset<a_9slice_def>( patch_name.c_str() );
	if( slice_def == nullptr )
	{
		assert( false );
		return;
	}

	float xpos;
	float ypos;
	float dst_w;
	float dst_h;

	SCOPED_VAR( rs_color( color ) );

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
}

void w_render::set_clear_color( w_color clear_color )
{
	current_clear_color = clear_color;
}
