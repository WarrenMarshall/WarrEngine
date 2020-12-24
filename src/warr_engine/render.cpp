
#include "master_pch.h"
#include "master_header.h"

void w_render::init()
{
	batch_quads = std::make_unique<w_render_batch>( render_prim::quad );
	batch_quads->bind();
	batch_triangles = std::make_unique<w_render_batch>( render_prim::triangle );
	batch_triangles->bind();
	batch_lines = std::make_unique<w_render_batch>( render_prim::line );
	batch_lines->bind();

	// MODEL MATRIX (getting stuff into worldspace from model space)
	//
	// push an identity matrix onto the stack. this will always live
	// as the first element in the stack and should be the only
	// matrix remaining when the frame finishes rendering.
	MATRIX->push_identity();

	// initialize render state stacks
	clear_render_states();

	// generate the sample points for drawing a circle. these verts sit
	// on a unit circle and are scaled to match the radius requested for
	// each circle we draw.
	float angle = 0;
	float angle_step = 360.0f / static_cast<float>( circle_sample_points_max );
	for( auto& circle_sample_point : circle_sample_points )
	{
		circle_sample_point = w_vec2::dir_from_angle( angle );
		angle += angle_step;
	}
}

w_render* w_render::begin()
{
	return this;
}

w_render* w_render::push_rgb( const w_color& color )
{
	rs_color_stack.emplace_back( color );

	return this;
}

w_render* w_render::replace_rgb( const w_color& color )
{
	rs_color_stack[ rs_color_stack.size() - 1 ] = color;
	return this;
}

w_render* w_render::pop_rgb()
{
	rs_color_stack.pop_back();

	return this;
}

w_render* w_render::push_rgba( const w_color& color )
{
	push_rgb( color );
	push_alpha( color.a );

	return this;
}

w_render* w_render::push_rgba( const w_color& color, const float alpha )
{
	push_rgb( color );
	push_alpha( alpha );

	return this;
}

w_render* w_render::replace_rgba( const w_color& color )
{
	replace_rgb( color );
	replace_alpha( color.a );
	return this;

}

w_render* w_render::replace_rgba( const w_color& color, const float alpha )
{
	replace_rgb( color );
	replace_alpha( alpha );
	return this;
}

w_render* w_render::pop_rgba()
{
	pop_rgb();
	pop_alpha();

	return this;
}

w_render* w_render::push_alpha( const float alpha )
{
	rs_alpha_stack.emplace_back( alpha );

	return this;
}

w_render* w_render::replace_alpha( const float alpha )
{
	rs_alpha_stack[ rs_alpha_stack.size() - 1 ] = alpha;
	return this;
}

w_render* w_render::pop_alpha()
{
	rs_alpha_stack.pop_back();
	return this;
}

w_render* w_render::push_emissive( const float emissive )
{
	rs_emissive_stack.emplace_back( emissive );
	return this;
}

w_render* w_render::pop_emissive()
{
	rs_emissive_stack.pop_back();
	return this;
}

w_render* w_render::push_scale( const w_vec2& scale )
{
	rs_scale_stack.emplace_back( scale );

	return this;
}

w_render* w_render::push_scale( const float scale )
{
	rs_scale_stack.emplace_back( w_vec2( scale, scale ) );

	return this;
}

w_render* w_render::replace_scale( const w_vec2& scale )
{
	rs_scale_stack[ rs_scale_stack.size() - 1 ] = scale;
	return this;
}

w_render* w_render::replace_scale( const float scale )
{
	replace_scale( w_vec2( scale, scale ) );
	return this;
}

w_render* w_render::push_angle( const float angle )
{
	rs_angle_stack.emplace_back( angle );

	return this;
}

w_render* w_render::replace_angle( const float angle )
{
	rs_angle_stack[ rs_angle_stack.size() - 1 ] = angle;
	return this;
}

w_render* w_render::push_align( const e_align& align )
{
	rs_align_stack.emplace_back( align );

	return this;
}

w_render* w_render::replace_align( const e_align& align )
{
	rs_align_stack[ rs_align_stack.size() - 1 ] = align;
	return this;
}

w_render* w_render::push_depth( const float depth )
{
	rs_z_depth = depth;
	rs_z_depth_nudge_accum = 0.0f;

	return this;
}

// adds/subtracts a value from the current depth. makes it easier
// to, for example, nudge something in front of something else without
// having to know the current depth

w_render* w_render::push_depth_nudge( const float nudge )
{
	rs_z_depth += nudge;
	rs_z_depth_nudge_accum += nudge;

	return this;
}

w_render* w_render::end()
{
	clear_render_states();
	return this;
}

void w_render::clear_render_states()
{
	rs_color_stack.clear();
	rs_alpha_stack.clear();
	rs_emissive_stack.clear();
	rs_scale_stack.clear();
	rs_angle_stack.clear();
	rs_align_stack.clear();

	rs_snap_to_pixel = true;

	rs_color_stack = { w_color::white };
	rs_alpha_stack = { 1.0f };
	rs_emissive_stack = { 0.0f };
	rs_scale_stack = { w_vec2( 1.0f, 1.0f ) };
	rs_angle_stack = { 0.0f };
	rs_align_stack = { align::left };
}

w_color w_render::get_palette_color_from_idx( int idx )
{
	if( !palette )
	{
		return w_color::white;
	}

	return palette->get_color_from_idx( idx );
}

w_color w_render::pal_color_from_idx( int idx )
{
	// if there's no palette specifically in use, return a default color
	if( !current_palette )
	{
		return w_color::white;
	}

	// validate we are in range
	assert( idx < current_palette->colors.size() );

	return current_palette->colors[ idx ];
}

w_render* w_render::draw_mesh( a_mesh* mesh, const w_vec2& dst )
{
	w_vec2 rs_scale = rs_scale_stack.back();
	float rs_angle = rs_angle_stack.back();

	// copy the color/alpha into each vertex on the mesh before rendering.
	// #optimization : probably not the best way to do this, but fine for now.
	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();
	float rs_emissive = rs_emissive_stack.back();

	for(auto & render_vert : mesh->render_verts)
	{
		render_vert.r = rs_color.r;
		render_vert.g = rs_color.g;
		render_vert.b = rs_color.b;
		render_vert.a = rs_color.a;

		render_vert.e = rs_emissive;
	}

	MATRIX
		->push()
		->rotate( rs_angle )
		->scale( rs_scale.x, rs_scale.y );

	for( auto x = 0 ; x < mesh->render_verts.size() ; x +=3 )
	{
		batch_triangles->add_element(
			mesh->tex,
			mesh->render_verts[ x ],
			mesh->render_verts[ x + 1 ],
			mesh->render_verts[ x + 2 ]
		);
	}

	MATRIX->pop();

	return this;
}

/*
	draws a texture as a sprite onto the screen.

	this offsets along left and up by half the texture size, which
	centers the quad being drawn at 0,0,0.
*/
w_render* w_render::draw_sprite( a_texture* tex, const w_vec2& dst )
{
	return draw_sprite( tex->get_subtexture(), dst );
}

w_render* w_render::draw_sprite( const a_subtexture* subtex, const w_vec2& dst )
{
	float w = subtex->rc_tex.w;
	float h = subtex->rc_tex.h;

	w_vec2 rs_scale = rs_scale_stack.back();
	float rs_angle = rs_angle_stack.back();

	w *= rs_scale.x;
	h *= rs_scale.y;

	float hw = w / 2.0f;
	float hh = h / 2.0f;

	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();
	float rs_emissive = rs_emissive_stack.back();

	w_render_vertex v0( w_vec2( -hw, hh ), w_vec2( subtex->uv00.u, subtex->uv00.v ), rs_color, rs_emissive );
	w_render_vertex v1( w_vec2( hw, hh ), w_vec2( subtex->uv11.u, subtex->uv00.v ), rs_color, rs_emissive );
	w_render_vertex v2( w_vec2( hw, -hh ), w_vec2( subtex->uv11.u, subtex->uv11.v ), rs_color, rs_emissive );
	w_render_vertex v3( w_vec2( -hw, -hh ), w_vec2( subtex->uv00.u, subtex->uv11.v ), rs_color, rs_emissive );

	MATRIX
		->push()
		->translate( { dst.x, dst.y } )
		->rotate( rs_angle );

	batch_quads->add_element( subtex->tex, v0, v1, v2, v3 );

	MATRIX->pop();

	return this;
}

/*
	draws a texture onto a quad.
*/
w_render* w_render::draw( a_texture* tex, const w_rect& dst )
{
	return draw( tex->get_subtexture(), dst );
}

w_render* w_render::draw( const a_subtexture* subtex, const w_rect& dst )
{
	float w = dst.w ? dst.w : subtex->rc_tex.w;
	float h = dst.h ? dst.h : subtex->rc_tex.h;

	w_vec2 rs_scale = rs_scale_stack.back();

	w *= rs_scale.x;
	h *= rs_scale.y;

	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();
	float rs_emissive = rs_emissive_stack.back();

	w_render_vertex v0( w_vec2( 0.0f, h ), w_vec2( subtex->uv00.u, subtex->uv00.v ), rs_color, rs_emissive );
	w_render_vertex v1( w_vec2( w, h ), w_vec2( subtex->uv11.u, subtex->uv00.v ), rs_color, rs_emissive );
	w_render_vertex v2( w_vec2( w, 0.0f ), w_vec2( subtex->uv11.u, subtex->uv11.v ), rs_color, rs_emissive );
	w_render_vertex v3( w_vec2( 0.0f, 0.0f ), w_vec2( subtex->uv00.u, subtex->uv11.v ), rs_color, rs_emissive );

	MATRIX->push()->translate( { dst.x, dst.y } );
	batch_quads->add_element( subtex->tex, v0, v1, v2, v3 );
	MATRIX->pop();

	return this;
}

/*
	draws a string from a bitmap font, char by char
*/
w_render* w_render::draw_string( const std::string_view text, const w_rect& dst )
{
	// not specifying a font means you want to use the default font
	return draw_string( engine->pixel_font, text, dst );
}

w_render* w_render::draw_string( a_font* font, const std::string_view text, const w_rect& dst )
{
	w_vec2 rs_scale = rs_scale_stack.back();
	e_align rs_align = rs_align_stack.back();

	// ----------------------------------------------------------------------------

	w_vec2 alignment_pos_adjustment( 0.0f, 0.0f );

	if( rs_align & align::hcenter )
	{
		w_vec2 extents = font->get_string_extents( text ) * rs_scale.x;
		alignment_pos_adjustment.x -= extents.x / 2.0f;
	}

	if( rs_align & align::right )
	{
		w_vec2 extents = font->get_string_extents( text ) * rs_scale.x;
		alignment_pos_adjustment.x -= extents.x;
	}

	if( rs_align & align::vcenter )
	{
		alignment_pos_adjustment.y -= (font->font_def->max_height * rs_scale.y ) / 2.0f;
	}

	// ----------------------------------------------------------------------------

	MATRIX
		->push()
		->translate( { alignment_pos_adjustment.x, alignment_pos_adjustment.y } );

	float xpos = dst.x;
	float ypos = dst.y;

	for( const char iter : text )
	{
		w_font_char* fch = &( font->font_def->char_map[ static_cast<int>( iter ) ] );

		// small optimization to skip drawing completely blank characters
		if( fch->w > 0 )
		{
			draw(
				fch->subtex.get(),
				w_rect( xpos + ( fch->xoffset * rs_scale.x ), ypos + ( fch->yoffset * rs_scale.y ) )
			);
		}

		xpos += (fch->xadvance * rs_scale.x);
	}

	MATRIX->pop();

	return this;
}

/*
	call at the start of each frame to set up and clear the screen
*/
void w_render::begin_frame()
{
}

/*
	call at end of frame to finalize frame and render all buffers
*/
void w_render::end_frame()
{
	OPENGL->init_view_matrix_identity_ui();

	// accum stats
	stats.frame_times_ms.accum( engine->time->delta_ms );
	stats.frame_count.inc();

	// possibly draw stats on the screen
	draw_stats();

	// the last draws need to be flushed

	batch_quads->draw_and_reset();
	batch_triangles->draw_and_reset();
	batch_lines->draw_and_reset();

	// when the frame ends, there should be
	// a single matrix left on the stack (the identity matrix we created
	// at the renderer start up). If there are any other number, then
	// there is an uneven push/pop combo somewhere in the code.

	assert( OPENGL->modelview_stack.size() == 1 );

	// verify that state stacks are back where they started. if not,
	// it means there's a push/pop mismatch somewhere in the code.
	assert( rs_color_stack.size() == 1 );
	assert( rs_alpha_stack.size() == 1 );
	assert( rs_emissive_stack.size() == 1 );
	assert( rs_scale_stack.size() == 1 );
	assert( rs_align_stack.size() == 1 );
}

/*
	draws the X and Y axis in the viewport at the world origin

	positive directions are brighter than the negative ones
*/

w_render* w_render::draw_world_axis()
{
	push_rgb( w_color( 1.0f, 0.0f, 0.0f ) );
	draw_line( w_vec2::zero, w_vec2( 5000, 0 ) );

	push_rgb( w_color( 0.5f, 0.0f, 0.0f ) );
	draw_line( w_vec2::zero, w_vec2( -5000, 0 ) );

	push_rgb( w_color( 0.0f, 1.0f, 0.0f ) );
	draw_line( w_vec2::zero, w_vec2( 0, 5000 ) );

	push_rgb( w_color( 0.0f, 0.5f, 0.0f ) );
	draw_line( w_vec2::zero, w_vec2( 0, -5000 ) );

	return this;
}

/*
	draws useful stats at the top of the screen
*/

w_render* w_render::draw_stats()
{
#if !defined(_FINALRELEASE)
	RENDER->begin()->push_depth( zdepth_stats );

	if( show_stats )
	{
		stat_lines.clear();
		stat_lines.emplace_back( fmt::format( "RENDER : {} FPS ({:.1f} ms) / TICK : {} FPS",
											  stats.frame_count.value,
											  stats.frame_times_ms.value,
											  static_cast<int>( w_time::FTS_desired_frames_per_second ) ) );
		stat_lines.emplace_back( fmt::format( "DC: {}, V: {}, I: {}",
											  f_commas( stats.draw_calls.value ),
											  f_commas( stats.vertices.value ),
											  f_commas( stats.indices.value ) )
		);
		stat_lines.emplace_back( fmt::format( "Layers : {}", engine->layer_mgr->layer_stack.size() ) );
		stat_lines.emplace_back( fmt::format( "Entities : {}", f_commas( stats.entities.value ) ) );
		stat_lines.emplace_back( fmt::format( "Time Dilation: {:.2f}", engine->time->dilation ) );
		stat_lines.emplace_back( fmt::format( "Mouse: W:{:.0f}, {:.0f} / V:{:.0f}, {:.0f}", engine->input->mouse_window_pos.x, engine->input->mouse_window_pos.y, engine->input->mouse_vwindow_pos.x, engine->input->mouse_vwindow_pos.y ) );

		if( stats.stat_custom_string.length() )
		{
			stat_lines.emplace_back( stats.stat_custom_string );
			stats.stat_custom_string = "";
		}

		int font_max_height = engine->pixel_font->font_def->max_height;

		RENDER->begin()
			->push_rgba( w_color::pal( 0 ) )
			->push_alpha( 0.75f )
			->draw_filled_rectangle( w_rect( 0.0f, 0.0f, ui_canvas_w, static_cast<float>( font_max_height * stat_lines.size() ) ) )
			->pop_alpha()
			->push_depth_nudge()
			->push_rgb( w_color::white );

		RENDER->push_align( align::hcenter );
		float ypos = 0;
		for( const auto& iter : stat_lines )
		{
			RENDER->draw_string( iter, w_rect( ui_canvas_hw, ypos ) );
			ypos += font_max_height;
		}

		RENDER->end();
	}
	else
	{
		RENDER->begin()
			->push_align( align::right )
			->draw_string(
				fmt::format( "{} FPS ({:.2f} ms)", f_commas( stats.frame_count.value ), stats.frame_times_ms.value ),
				w_rect( ui_canvas_w, 0 ) )
			->end();
	}
#endif

	RENDER->end();

	return this;
}

w_render* w_render::draw_filled_rectangle( const w_rect& dst )
{
	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();
	float rs_emissive = rs_emissive_stack.back();

	w_render_vertex v0(
		w_vec2( dst.x, dst.y ),
		w_uv( 0, 0 ),
		rs_color,
		rs_emissive
	);
	w_render_vertex v1(
		w_vec2( dst.x + dst.w, dst.y ),
		w_uv( 1, 0 ),
		rs_color,
		rs_emissive
	);
	w_render_vertex v2(
		w_vec2( dst.x + dst.w, dst.y + dst.h ),
		w_uv( 1, 1 ),
		rs_color,
		rs_emissive
	);
	w_render_vertex v3(
		w_vec2( dst.x, dst.y + dst.h ),
		w_uv( 0, 1 ),
		rs_color,
		rs_emissive
	);

	batch_quads->add_element( engine->white_solid->tex, v0, v1, v2, v3 );

	return this;
}

// draws an empty rectangle

w_render* w_render::draw_rectangle( const w_rect& dst )
{
	w_bbox box;
	box.add( w_vec2(
		static_cast<float>( dst.x ),
		static_cast<float>( dst.y ) )
	);
	box.add( w_vec2(
		static_cast<float>( dst.x + dst.w ),
		static_cast<float>( dst.y + dst.h ) )
	);

	draw_line( w_vec2( box.min.x, box.min.y ), w_vec2( box.max.x, box.min.y ) );
	draw_line( w_vec2( box.max.x, box.min.y ), w_vec2( box.max.x, box.max.y ) );
	draw_line( w_vec2( box.max.x, box.max.y ), w_vec2( box.min.x, box.max.y ) );
	draw_line( w_vec2( box.min.x, box.max.y ), w_vec2( box.min.x, box.min.y ) );

	return this;
}

// draws a circle with line segments

w_render* w_render::draw_circle( const w_vec2& origin, float radius )
{
	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();
	float rs_emissive = rs_emissive_stack.back();

	w_render_vertex v0( w_vec2::zero, w_uv( 0, 0 ), rs_color, rs_emissive );
	w_render_vertex v1( w_vec2::zero, w_uv( 0, 0 ), rs_color, rs_emissive );

	rs_snap_to_pixel = false;

	for( auto x = 0; x < circle_sample_points_max; ++x )
	{
		v0.x = origin.x + ( circle_sample_points[ x ].x * radius );
		v0.y = origin.y + ( circle_sample_points[ x ].y * radius );

		v1.x = origin.x + ( circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
		v1.y = origin.y + ( circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

		batch_lines->add_element( engine->white_wire->tex, v0, v1 );
	}

	rs_snap_to_pixel = true;

	return this;
}

w_render* w_render::draw_line( const w_vec2& start, const w_vec2& end )
{
	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();
	float rs_emissive = rs_emissive_stack.back();

	w_render_vertex v0( start, w_uv( 0, 0 ), rs_color, rs_emissive );
	w_render_vertex v1( end, w_uv( 0, 0 ), rs_color, rs_emissive );

	rs_snap_to_pixel = false;

	batch_lines->add_element(
		engine->white_wire->tex,
		v0,
		v1
	);

	rs_snap_to_pixel = true;

	return this;
}

w_render* w_render::draw_point( const w_vec2& pos )
{
	return this;		// #batch

#if 0
	set_current_texture( engine->white_wire->tex );

	auto start = pos;
	auto end = start + w_vec2( 1.0f, 0.0f );

	rs_snap_to_pixel = false;

	draw_line( start, end );

	rs_snap_to_pixel = true;

	return this;
#endif
}

w_render* w_render::draw_sliced( const a_9slice_def* slice_def, const w_rect& dst )
{
	a_subtexture* p_00 = slice_def->patches[ slicedef_patch::P_00 ];
	a_subtexture* p_10 = slice_def->patches[ slicedef_patch::P_10 ];
	a_subtexture* p_20 = slice_def->patches[ slicedef_patch::P_20 ];
	a_subtexture* p_01 = slice_def->patches[ slicedef_patch::P_01 ];
	a_subtexture* p_11 = slice_def->patches[ slicedef_patch::P_11 ];
	a_subtexture* p_21 = slice_def->patches[ slicedef_patch::P_21 ];
	a_subtexture* p_02 = slice_def->patches[ slicedef_patch::P_02 ];
	a_subtexture* p_12 = slice_def->patches[ slicedef_patch::P_12 ];
	a_subtexture* p_22 = slice_def->patches[ slicedef_patch::P_22 ];

	// nudge the rendering down by the height of the top row of subtextures. this
	// allows us to think of the top/left of this window as the actual graphical top/left.

	float xpos = dst.x;
	float ypos = dst.y;

	float inner_w = dst.w - p_00->rc_tex.w - p_20->rc_tex.w;
	float inner_h = dst.h - p_00->rc_tex.h - p_02->rc_tex.h;

	// top row

	draw( p_00, w_rect( xpos, ypos, p_00->rc_tex.w, p_00->rc_tex.h ) );

	xpos += p_00->rc_tex.w;
	draw( p_10, w_rect( xpos, ypos, inner_w, p_10->rc_tex.h ) );

	xpos += inner_w;
	draw( p_20, w_rect( xpos, ypos, p_20->rc_tex.w, p_20->rc_tex.h ) );

	// middle row

	xpos = dst.x;
	ypos += p_00->rc_tex.h;
	draw( p_01, w_rect( xpos, ypos, p_01->rc_tex.w, inner_h ) );

	xpos += p_01->rc_tex.w;
	draw( p_11, w_rect( xpos, ypos, inner_w, inner_h ) );

	xpos += inner_w;
	draw( p_21, w_rect( xpos, ypos, p_21->rc_tex.w, inner_h ) );

	// bottom row

	xpos = dst.x;
	ypos += inner_h;
	draw( p_02, w_rect( xpos, ypos, p_02->rc_tex.w, p_02->rc_tex.h ) );

	xpos += p_02->rc_tex.w;
	draw( p_12, w_rect( xpos, ypos, inner_w, p_12->rc_tex.h ) );

	xpos += inner_w;
	draw( p_22, w_rect( xpos, ypos, p_22->rc_tex.w, p_22->rc_tex.h ) );

	return this;
}

// call this function to figure out a new value based on the frame interpolation percentage.

float w_render::calc_interpolated_per_sec_value( float current_value, float step_per_second ) const
{
	return current_value + ( ( step_per_second * w_time::FTS_step_value_s ) * RENDER->frame_interpolate_pct );
}

// binds a texture to a specific texture slot

void w_render::bind_texture( int slot, a_texture* tex )
{
	glBindTextureUnit( slot, tex->gl_id );
}
