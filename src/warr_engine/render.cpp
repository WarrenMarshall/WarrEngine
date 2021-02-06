
#include "master_pch.h"
#include "master_header.h"

// used to manage the potential allocations for the render_states vector
constexpr size_t max_render_states = 15;

void w_render::init()
{
	batch_quads = std::make_unique<w_render_batch>( render_prim::quad );
	batch_triangles = std::make_unique<w_render_batch>( render_prim::triangle );
	batch_lines = std::make_unique<w_render_batch>( render_prim::line );
	batch_points = std::make_unique<w_render_batch>( render_prim::point );

	// MODEL MATRIX (getting stuff into worldspace from model space)
	//
	// push an identity matrix onto the stack. this will always live as the
	// first element in the stack and should be the only matrix remaining when
	// the frame finishes rendering.

	OPENGL->push_identity();

	// initialize render state stacks

	render_states.reserve( max_render_states );
	rs_reset();

	// generate the sample points for drawing a circle. these verts sit on a
	// unit circle and are scaled to match the radius requested for each circle
	// we draw.

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
	rs_push()->color = color;
	return this;
}

w_render* w_render::push_rgba( const w_color& color )
{
	auto rs = rs_push();
	rs->color = color;

	return this;
}

w_render* w_render::push_rgba( const w_color& color, const float alpha )
{
	auto rs = rs_push();
	rs->color = color;
	rs->color.a = alpha;

	return this;
}

w_render* w_render::push_alpha( const float alpha )
{
	rs_push()->color.a = alpha;

	return this;
}

w_render* w_render::push_glow( const float glow )
{
	rs_push()->glow = glow;
	return this;
}

w_render* w_render::push_scale( const w_vec2& scale )
{
	rs_push()->scale = scale;

	return this;
}

w_render* w_render::push_scale( const float scale )
{
	rs_push()->scale = w_vec2( scale, scale );

	return this;
}

w_render* w_render::push_uv_tiling( const w_vec2& uv_tiling )
{
	rs_push()->uv_tiling = uv_tiling;

	return this;
}

w_render* w_render::push_uv_tiling( const float uv_tiling )
{
	rs_push()->uv_tiling = w_vec2( uv_tiling, uv_tiling );

	return this;
}

w_render* w_render::push_angle( const float angle )
{
	rs_push()->angle = angle;

	return this;
}

w_render* w_render::push_align( const e_align& align )
{
	rs_push()->align = align;

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

w_render* w_render::push_snap_to_pixel( bool snap_to_pixel )
{
	rs_push()->snap_to_pixel = snap_to_pixel;
	return this;
}

w_render* w_render::push_render_state( w_render_state& rs )
{
	rs_push( rs );

	return this;
}

w_render* w_render::push_render_state( w_render_state_opt& rso )
{
	auto rs = rs_push();
	rso.populate( rs );

	return this;
}

w_render* w_render::pop()
{
	rs_pop();

	return this;
}

w_render* w_render::end()
{
	rs_reset();
	return this;
}

w_render* w_render::push()
{
	rs_push();

	return this;
}

w_color w_render::get_palette_color_from_idx( int idx )
{
	if( !palette )
	{
		return w_color::white;
	}

	return palette->get_color_from_idx( idx );
}

bool w_render::batches_are_empty()
{
	return
		(
			batch_quads->is_empty()
			&& batch_triangles->is_empty()
			&& batch_lines->is_empty()
			&& batch_points->is_empty()
		);
}

void w_render::draw_and_reset_all_batches()
{
	RENDER->batch_quads->draw_and_reset();
	RENDER->batch_triangles->draw_and_reset();
	RENDER->batch_lines->draw_and_reset();
	RENDER->batch_points->draw_and_reset();
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

w_render_state* w_render::rs_top()
{
	return &( render_states.back() );
}

w_render_state* w_render::rs_push()
{
	// duplicates the render_state at the top of the stack
	auto rs = *rs_top();
	render_states.emplace_back( std::move( rs ) );

	assert( render_states.size() < max_render_states );

	return rs_top();
}

// places a copy of the provided w_render_state block on top of the stack.
// allows for easy mass setting of render state vars.

w_render_state* w_render::rs_push( w_render_state& rs )
{
	render_states.emplace_back( rs );

	assert( render_states.size() < max_render_states );

	return rs_top();
}

w_render_state* w_render::rs_pop()
{
	assert( render_states.size() > 1 );

	render_states.pop_back();

	return rs_top();
}

void w_render::rs_reset()
{
	render_states.clear();

	w_render_state rs;

	rs.color = w_color::white;
	rs.glow = 0.0f;
	rs.scale = w_vec2( 1.0f, 1.0f );
	rs.angle = 0.0f;
	rs.align = align::left;
	rs.snap_to_pixel = true;

	render_states.emplace_back( std::move( rs ) );
}

w_render* w_render::draw_mesh( a_mesh* mesh, const w_vec2& dst )
{
	// copy the color/alpha into each vertex on the mesh before rendering.

	auto rs = rs_top();

	for(auto & render_vert : mesh->render_verts)
	{
		render_vert.r = rs->color.r;
		render_vert.g = rs->color.g;
		render_vert.b = rs->color.b;
		render_vert.a = rs->color.a;

		render_vert.e = rs->glow;
	}

	OPENGL
		->push()
		->rotate( rs->angle )
		->scale( rs->scale.x, rs->scale.y );

	for( auto x = 0 ; x < mesh->render_verts.size() ; x +=3 )
	{
		batch_triangles->add_primitive(
			mesh->tex,
			mesh->render_verts[ x ],
			mesh->render_verts[ x + 1 ],
			mesh->render_verts[ x + 2 ]
		);
	}

	OPENGL->pop();

	return this;
}

// draws a texture as a sprite onto the screen.
//
// this offsets along left and up by half the texture size, which centers the
// quad being drawn at 0,0,0.

w_render* w_render::draw_sprite( const a_texture* texture, const w_vec2& dst )
{
	float w = texture->rc.w;
	float h = texture->rc.h;

	auto rs = rs_top();

	w *= rs->scale.x;
	h *= rs->scale.y;

	float hw = w / 2.0f;
	float hh = h / 2.0f;

	w_render_vertex v0( w_vec2( -hw, hh ), w_vec2( texture->uv00.u * rs->uv_tiling.u, texture->uv00.v * rs->uv_tiling.v ), rs->color, rs->glow );
	w_render_vertex v1( w_vec2( hw, hh ), w_vec2( texture->uv11.u * rs->uv_tiling.u, texture->uv00.v * rs->uv_tiling.v ), rs->color, rs->glow );
	w_render_vertex v2( w_vec2( hw, -hh ), w_vec2( texture->uv11.u * rs->uv_tiling.u, texture->uv11.v * rs->uv_tiling.v ), rs->color, rs->glow );
	w_render_vertex v3( w_vec2( -hw, -hh ), w_vec2( texture->uv00.u * rs->uv_tiling.u, texture->uv11.v * rs->uv_tiling.v ), rs->color, rs->glow );

	OPENGL
		->push()
		->translate( { dst.x, dst.y } )
		->rotate( rs->angle );

	batch_quads->add_primitive( texture, v0, v1, v2, v3 );

	OPENGL->pop();

	return this;
}

// draws a textured quad

w_render* w_render::draw( const a_texture* texture, const w_rect& dst )
{
	float w = dst.w ? dst.w : texture->rc.w;
	float h = dst.h ? dst.h : texture->rc.h;

	auto rs = rs_top();

	w *= rs->scale.x;
	h *= rs->scale.y;

	w_render_vertex v0( w_vec2( 0.0f, h ), w_vec2( texture->uv00.u * rs->uv_tiling.u, texture->uv00.v * rs->uv_tiling.v ), rs->color, rs->glow );
	w_render_vertex v1( w_vec2( w, h ), w_vec2( texture->uv11.u * rs->uv_tiling.u, texture->uv00.v * rs->uv_tiling.v ), rs->color, rs->glow );
	w_render_vertex v2( w_vec2( w, 0.0f ), w_vec2( texture->uv11.u * rs->uv_tiling.u, texture->uv11.v * rs->uv_tiling.v ), rs->color, rs->glow );
	w_render_vertex v3( w_vec2( 0.0f, 0.0f ), w_vec2( texture->uv00.u * rs->uv_tiling.u, texture->uv11.v * rs->uv_tiling.v ), rs->color, rs->glow );

	OPENGL->push()->translate( { dst.x, dst.y } );
	batch_quads->add_primitive( texture, v0, v1, v2, v3 );
	OPENGL->pop();

	return this;
}

// draws a string from a bitmap font, char by char, as textured quads

w_render* w_render::draw_string( const std::string_view text, const w_pos& pos )
{
	// not specifying a font means you want to use the default font
	return draw_string( engine->pixel_font, text, pos );
}

w_render* w_render::draw_string( a_font* font, const std::string_view text, const w_pos& pos )
{
	auto rs = rs_top();

	// ----------------------------------------------------------------------------

	w_vec2 alignment_pos_adjustment( 0.0f, 0.0f );

	if( rs->align & align::hcenter )
	{
		w_vec2 extents = font->get_string_extents( text ) * rs->scale.x;
		alignment_pos_adjustment.x -= extents.x / 2.0f;
	}

	if( rs->align & align::right )
	{
		w_vec2 extents = font->get_string_extents( text ) * rs->scale.x;
		alignment_pos_adjustment.x -= extents.x;
	}

	if( rs->align & align::vcenter )
	{
		alignment_pos_adjustment.y -= ( font->font_def->max_height * rs->scale.y ) / 2.0f;
	}

	// ----------------------------------------------------------------------------

	float xpos = pos.x + alignment_pos_adjustment.x;
	float ypos = pos.y + alignment_pos_adjustment.y;

	for( const char iter : text )
	{
		w_font_char* fch = &( font->font_def->char_map[ static_cast<int>( iter ) ] );

		// small optimization to skip drawing completely blank characters
		if( fch->w > 0 )
		{
			draw(
				fch->glyph_texture.get(),
				w_rect( xpos + ( fch->xoffset * rs->scale.x ), ypos + ( fch->yoffset * rs->scale.y ) )
			);
		}

		xpos += ( fch->xadvance * rs->scale.x);
	}

	return this;
}

// called at the start of each frame to set up and clear the screen

void w_render::begin_frame()
{
}

// called at end of each frame to finalize and render all buffers

void w_render::end_frame()
{
	OPENGL->init_view_matrix_identity_ui();

	// stats

	stats.frame_times_ms += engine->time->delta_ms;
	stats.frame_count++;

	draw_stats();

	// the last draws need to be flushed

	batch_quads->draw_and_reset_internal();
	batch_triangles->draw_and_reset_internal();
	batch_lines->draw_and_reset_internal();
	batch_points->draw_and_reset_internal();

	// when the frame ends, there should be a single matrix left on the stack
	// (the identity matrix we created at renderer start up). If there is any
	// other number, then there is an uneven push/pop combo somewhere in the
	// code.

	assert( OPENGL->modelview_stack.size() == 1 );

#ifndef _FINALRELEASE
	single_frame_debugger = false;
#endif
}

// draws the X and Y axis in the viewport at the world origin
//
// positive directions are brighter than the negative ones

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


// draws useful stats at the top of the screen


w_render* w_render::draw_stats()
{
#if !defined(_FINALRELEASE)
	RENDER->begin()->push_depth( zdepth_stats );

	if( show_stats )
	{
		stat_lines.clear();
		stat_lines.emplace_back( fmt::format( "RENDER : {} FPS ({:.1f} ms) / FTS: {} FPS",
											  f_commas( stats.frame_count.value ),
											  stats.frame_times_ms.value,
											  fixed_time_step::frames_per_second ) );
		stat_lines.emplace_back( fmt::format( "DC:{} / Q:{} / T:{} / L:{} / P:{}",
											  f_commas( stats.draw_calls.value ),
											  f_commas( stats.quads.value ),
											  f_commas( stats.triangles.value ),
											  f_commas( stats.lines.value ) ,
											  f_commas( stats.points.value ) )
		);
		stat_lines.emplace_back( fmt::format( "Layers : {}", engine->layer_mgr->layer_stack.size() ) );
		stat_lines.emplace_back( fmt::format( "Entities : {}", f_commas( stats.entities.value ) ) );
		stat_lines.emplace_back( fmt::format( "Time Dilation: {:.2f}", engine->time->dilation ) );
		stat_lines.emplace_back( fmt::format( "Mouse: W:{:.0f}, {:.0f} / V:{:.0f}, {:.0f} / U:{:.0f}, {:.0f}",
											  engine->input->mouse_window_pos.x, engine->input->mouse_window_pos.y,
											  engine->input->mouse_vwindow_pos.x, engine->input->mouse_vwindow_pos.y,
											  engine->input->mouse_uiwindow_pos.x, engine->input->mouse_uiwindow_pos.y )
		);

		if( stats.stat_custom_string.length() )
		{
			stat_lines.emplace_back( stats.stat_custom_string );
			stats.stat_custom_string = "";
		}

		RENDER
			->begin()
			->push_rgba( w_color::pal( 0 ) )
			->push_alpha( 0.75f )
			->draw_filled_rectangle( w_rect( 0.0f, 0.0f, ui_canvas_w,
				engine->pixel_font->font_def->max_height * stat_lines.size() ) )
			->end();

		RENDER
			->begin()
			->push_depth_nudge()
			->push_rgb( w_color::white )
			->push_align( align::hcenter );

		auto ypos = 0.0f;
		for( const auto& iter : stat_lines )
		{
			RENDER->draw_string( iter, { ui_canvas_hw, ypos } );
			ypos += engine->pixel_font->font_def->max_height;
		}

		RENDER->end();
	}
	else
	{
		RENDER->begin()
			->push_align( align::right )
			->draw_string(
				fmt::format( "{} FPS ({:.2f} ms)", f_commas( stats.frame_count.value ), stats.frame_times_ms.value ),
				{ ui_canvas_w, 0.0f	} )
			->end();
	}
#endif

	RENDER->end();

	return this;
}

w_render* w_render::draw_filled_rectangle( const w_rect& dst )
{
	auto rs = rs_top();

	w_render_vertex v0(
		w_vec2( dst.x, dst.y ),
		w_uv( 0, 0 ),
		rs->color,
		rs->glow
	);
	w_render_vertex v1(
		w_vec2( dst.x + dst.w, dst.y ),
		w_uv( 1, 0 ),
		rs->color,
		rs->glow
	);
	w_render_vertex v2(
		w_vec2( dst.x + dst.w, dst.y + dst.h ),
		w_uv( 1, 1 ),
		rs->color,
		rs->glow
	);
	w_render_vertex v3(
		w_vec2( dst.x, dst.y + dst.h ),
		w_uv( 0, 1 ),
		rs->color,
		rs->glow
	);

	batch_quads->add_primitive( engine->tex_white, v0, v1, v2, v3 );

	return this;
}

w_render* w_render::draw_triangle( const w_vec2& v0, const w_vec2& v1, const w_vec2& v2 )
{
	draw_line( v0, v1 );
	draw_line( v1, v2 );
	draw_line( v2, v0 );

	return this;
}

w_render* w_render::draw_filled_triangle( const w_vec2& v0, const w_vec2& v1, const w_vec2& v2 )
{
	auto rs = rs_top();

	w_render_vertex rv0(
		w_vec2( v0.x, v0.y ),
		w_uv( 0, 0 ),
		rs->color,
		rs->glow
	);
	w_render_vertex rv1(
		w_vec2( v1.x, v1.y ),
		w_uv( 1, 0 ),
		rs->color,
		rs->glow
	);
	w_render_vertex rv2(
		w_vec2( v2.x, v2.y ),
		w_uv( 1, 1 ),
		rs->color,
		rs->glow
	);

	batch_triangles->add_primitive( engine->tex_white, rv0, rv1, rv2 );

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
	auto rs = rs_top();

	w_render_vertex v0( w_vec2::zero, w_uv( 0, 0 ), rs->color, rs->glow );
	w_render_vertex v1( w_vec2::zero, w_uv( 0, 0 ), rs->color, rs->glow );

	RENDER->push_snap_to_pixel( false );

	for( auto x = 0; x < circle_sample_points_max; ++x )
	{
		v0.x = origin.x + ( circle_sample_points[ x ].x * radius );
		v0.y = origin.y + ( circle_sample_points[ x ].y * radius );

		v1.x = origin.x + ( circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
		v1.y = origin.y + ( circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

		batch_lines->add_primitive( engine->tex_white, v0, v1 );
	}

	RENDER->pop();

	return this;
}

w_render* w_render::draw_filled_circle( const w_vec2& origin, float radius )
{
	auto rs = rs_top();

	w_render_vertex v0( origin, w_uv( 0, 0 ), rs->color, rs->glow );
	w_render_vertex v1 = v0;
	w_render_vertex v2 = v0;

	RENDER->push_snap_to_pixel( false );

	for( auto x = 0; x < circle_sample_points_max; ++x )
	{
		v1.x = origin.x + ( circle_sample_points[ x ].x * radius );
		v1.y = origin.y + ( circle_sample_points[ x ].y * radius );

		v2.x = origin.x + ( circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
		v2.y = origin.y + ( circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

		batch_triangles->add_primitive( engine->tex_white, v0, v1, v2 );
	}

	RENDER->pop();

	return this;
}

w_render* w_render::draw_line( const w_vec2& start, const w_vec2& end )
{
	auto rs = rs_top();

	w_render_vertex v0( start, w_uv( 0, 0 ), rs->color, rs->glow );
	w_render_vertex v1( end, w_uv( 0, 0 ), rs->color, rs->glow );

	RENDER->push_snap_to_pixel( false );

	batch_lines->add_primitive(
		engine->tex_white,
		v0,
		v1
	);

	RENDER->pop();

	return this;
}

w_render* w_render::draw_point( const w_vec2& pos )
{
	auto rs = rs_top();

	w_render_vertex v0( pos, w_uv( 0, 0 ), rs->color, rs->glow );

	RENDER->push_snap_to_pixel( false );

	batch_points->add_primitive(
		engine->tex_white,
		v0
	);

	RENDER->pop();

	return this;
}

w_render* w_render::draw_sliced( const a_9slice_def* slice_def, const w_rect& dst )
{
	a_texture* p_00 = slice_def->patches[ slicedef_patch::P_00 ];
	a_texture* p_10 = slice_def->patches[ slicedef_patch::P_10 ];
	a_texture* p_20 = slice_def->patches[ slicedef_patch::P_20 ];
	a_texture* p_01 = slice_def->patches[ slicedef_patch::P_01 ];
	a_texture* p_11 = slice_def->patches[ slicedef_patch::P_11 ];
	a_texture* p_21 = slice_def->patches[ slicedef_patch::P_21 ];
	a_texture* p_02 = slice_def->patches[ slicedef_patch::P_02 ];
	a_texture* p_12 = slice_def->patches[ slicedef_patch::P_12 ];
	a_texture* p_22 = slice_def->patches[ slicedef_patch::P_22 ];

	// nudge the rendering down by the height of the top row of textures. this
	// allows us to think of the top/left of this window as the actual graphical top/left.

	float xpos = dst.x;
	float ypos = dst.y;

	float inner_w = dst.w - p_00->rc.w - p_20->rc.w;
	float inner_h = dst.h - p_00->rc.h - p_02->rc.h;

	// top row

	draw( p_00, w_rect( xpos, ypos, p_00->rc.w, p_00->rc.h ) );

	xpos += p_00->rc.w;
	draw( p_10, w_rect( xpos, ypos, inner_w, p_10->rc.h ) );

	xpos += inner_w;
	draw( p_20, w_rect( xpos, ypos, p_20->rc.w, p_20->rc.h ) );

	// middle row

	xpos = dst.x;
	ypos += p_00->rc.h;
	draw( p_01, w_rect( xpos, ypos, p_01->rc.w, inner_h ) );

	xpos += p_01->rc.w;
	draw( p_11, w_rect( xpos, ypos, inner_w, inner_h ) );

	xpos += inner_w;
	draw( p_21, w_rect( xpos, ypos, p_21->rc.w, inner_h ) );

	// bottom row

	xpos = dst.x;
	ypos += inner_h;
	draw( p_02, w_rect( xpos, ypos, p_02->rc.w, p_02->rc.h ) );

	xpos += p_02->rc.w;
	draw( p_12, w_rect( xpos, ypos, inner_w, p_12->rc.h ) );

	xpos += inner_w;
	draw( p_22, w_rect( xpos, ypos, p_22->rc.w, p_22->rc.h ) );

	return this;
}

// call this function to figure out a new value based on the frame interpolation percentage.

float w_render::calc_interpolated_per_sec_value( float current_value, float step_per_second ) const
{
	return current_value + ( ( step_per_second * fixed_time_step::per_second_scaler ) * RENDER->frame_interpolate_pct );
}

// binds a texture to a specific texture slot

void w_render::bind_texture( int slot, a_src_texture* tex )
{
	glBindTextureUnit( slot, tex->gl_id );
}
