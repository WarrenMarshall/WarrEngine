
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
		auto steps = static_cast<int>( frame_count.value );

		frame_times_ms.update_value( steps );
		render_buffers.update_value( steps );
		render_vertices.update_value( steps );
		render_indices.update_value( steps );
		num_entities.update_value( steps );
	}
	else
	{
		stat_timer->update();
	}
}

// ----------------------------------------------------------------------------

void w_render::init()
{
	master_render_buffer = std::make_unique<w_render_buffer>();
	master_render_buffer->bind();

	// MODEL MATRIX (getting stuff into worldspace from model space)
	//
	// push an identity matrix onto the stack. this will always live
	// as the first element in the stack and should be the only
	// matrix remaining when the frame finishes rendering.
	MATRIX->push_identity();

	// initialize render state stacks
	rs_color_stack.clear();		rs_color_stack += w_color::white;
	rs_alpha_stack.clear();		rs_alpha_stack += 1.0f;
	rs_scale_stack.clear();		rs_scale_stack += 1.0f;
	rs_angle_stack.clear();		rs_angle_stack += 0.0f;
	rs_align_stack.clear();		rs_align_stack = align::left;
	zdepth = zdepth_background;
	zdepth_nudge_accum = 0.0f;

	// generate the sample points for drawing a circle. these verts sit
	// on a unit circle and are scaled to match the radius requesed for
	// each circle draw.
	float angle = 0;
	float angle_step = 360.0f / static_cast<float>( circle_sample_points_max );
	for( auto& circle_sample_point : circle_sample_points )
	{
		circle_sample_point = w_vec2::from_angle( angle );
		angle += angle_step;
	}
}

w_render* w_render::begin()
{
	return this;
}

w_render* w_render::push_rgb( const w_color& color )
{
	rs_color_stack += color;

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

w_render* w_render::pop_rgba()
{
	pop_rgb();
	pop_alpha();

	return this;
}

w_render* w_render::push_alpha( const float alpha )
{
	rs_alpha_stack += alpha;

	return this;
}

w_render* w_render::pop_alpha()
{
	rs_alpha_stack.pop_back();

	return this;
}

w_render* w_render::push_scale( const float scale )
{
	rs_scale_stack += scale;

	return this;
}

w_render* w_render::push_angle( const float angle )
{
	rs_angle_stack += angle;

	return this;
}

w_render* w_render::push_align( const e_align& align )
{
	rs_align_stack += align;

	return this;
}

w_render* w_render::push_depth( const float depth )
{
	zdepth = depth;
	zdepth_nudge_accum = 0.0f;

	return this;
}

// adds/subtracts a value from the current depth. makes it easier
// to, for example, nudge something in front of something else without
// having to know the current depth

w_render* w_render::push_depth_nudge( const float nudge )
{
	zdepth += nudge;
	zdepth_nudge_accum += nudge;

	return this;
}

void w_render::end()
{
	rs_color_stack.clear();		rs_color_stack += w_color::white;
	rs_alpha_stack.clear();		rs_alpha_stack += 1.0f;
	rs_scale_stack.clear();		rs_scale_stack += 1.0f;
	rs_angle_stack.clear();		rs_angle_stack += 0.0f;
	rs_align_stack.clear();		rs_align_stack += align::left;

	zdepth -= zdepth_nudge_accum;
	zdepth_nudge_accum = 0.0f;
}

void w_render::draw_master_buffer()
{
	master_render_buffer->draw( current_texture );
	master_render_buffer->clear();
}

void w_render::maybe_draw_master_buffer( a_texture* texture )
{
	if( current_texture == nullptr )
	{
		current_texture = texture;
	}
	else if( current_texture != texture )
	{
		draw_master_buffer();
		current_texture = texture;
	}
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
	maybe_draw_master_buffer( subtex->tex );

	float w = subtex->sz.w;
	float h = subtex->sz.h;

	float rs_scale = rs_scale_stack.back();
	float rs_angle = rs_angle_stack.back();

	w *= rs_scale;
	h *= rs_scale;

	float hw = w / 2.0f;
	float hh = h / 2.0f;

	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();

	w_render_vert v0( w_vec2( -hw, hh ), w_vec2( subtex->uv00.u, subtex->uv00.v ), rs_color );
	w_render_vert v1( w_vec2( hw, hh ), w_vec2( subtex->uv11.u, subtex->uv00.v ), rs_color );
	w_render_vert v2( w_vec2( hw, -hh ), w_vec2( subtex->uv11.u, subtex->uv11.v ), rs_color );
	w_render_vert v3( w_vec2( -hw, -hh ), w_vec2( subtex->uv00.u, subtex->uv11.v ), rs_color );

	MATRIX->push()->translate( { dst.x, dst.y } )->rotate( rs_angle );
	master_render_buffer->add_quad( v0, v1, v2, v3 );
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
	maybe_draw_master_buffer( subtex->tex );

	float w = dst.w ? dst.w : subtex->sz.w;
	float h = dst.h ? dst.h : subtex->sz.h;

	float rs_scale = rs_scale_stack.back();

	w *= rs_scale;
	h *= rs_scale;

	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();

	w_render_vert v0( w_vec2( 0.0f, h ), w_vec2( subtex->uv00.u, subtex->uv00.v ), rs_color );
	w_render_vert v1( w_vec2( w, h ), w_vec2( subtex->uv11.u, subtex->uv00.v ), rs_color );
	w_render_vert v2( w_vec2( w, 0.0f ), w_vec2( subtex->uv11.u, subtex->uv11.v ), rs_color );
	w_render_vert v3( w_vec2( 0.0f, 0.0f ), w_vec2( subtex->uv00.u, subtex->uv11.v ), rs_color );

	MATRIX->push()->translate( { dst.x, dst.y } );
	master_render_buffer->add_quad( v0, v1, v2, v3 );
	MATRIX->pop();

	return this;
}

/*
	draws a string from a bitmap font, char by char
*/
w_render* w_render::draw_string( a_font* font, const std::string_view text, const w_rect& dst )
{
	maybe_draw_master_buffer( font->font_def->texture );

	float rs_scale = rs_scale_stack.back();
	e_align rs_align = rs_align_stack.back();

	w_vec2 alignment_pos_adjustment( 0.0f, 0.0f );

	if( rs_align & align::hcenter )
	{
		w_vec2 extents = font->get_string_extents( text ) * rs_scale;
		alignment_pos_adjustment.x -= extents.x / 2.0f;
	}

	if( rs_align & align::right )
	{
		w_vec2 extents = font->get_string_extents( text ) * rs_scale;
		alignment_pos_adjustment.x -= extents.x;
	}

	if( rs_align & align::vcenter )
	{
		alignment_pos_adjustment.y -= (font->font_def->max_height * rs_scale ) / 2.0f;
	}

	// ----------------------------------------------------------------------------

	w_font_char* fch;

	MATRIX
		->push()
		->translate( { alignment_pos_adjustment.x, alignment_pos_adjustment.y } );

	float xpos = dst.x;
	float ypos = dst.y;

	for( const char iter : text )
	{
		fch = &( font->font_def->char_map[ static_cast<int>( iter ) ] );

		// small optimization to skip drawing completely blank characters
		if( fch->w > 0 )
		{
			draw(
				fch->subtex.get(),
				w_rect( xpos, ypos + ( fch->yoffset * rs_scale ) )
			);

			xpos += fch->xoffset * rs_scale;
		}

		xpos += fch->xadvance * rs_scale;
	}

	MATRIX->pop();

	return this;
}

/*
	call at the start of each frame to set up and clear the screen
*/
void w_render::begin_frame( float frame_interpolate_pct )
{
	current_texture = nullptr;

	this->frame_interpolate_pct = frame_interpolate_pct;

	// reset all render buffers

	for( const auto& [xxx, asset] : engine->asset_cache->cache )
	{
		asset->clear_render_buffer();
	}
}

/*
	call at end of frame to finalize frame and render all buffers
*/
void w_render::end_frame()
{
	draw_stats();

	// the last draw needs to be flushed
	maybe_draw_master_buffer( nullptr );

	// accum stats
	stats.frame_times_ms.accum( engine->time->delta_ms );
	stats.frame_count.inc();

	// when the frame ends, there should be
	// a single matrix left on the stack (the identity matrix we created
	// at the renderer start up). If there are any other number, then
	// there is an uneven push/pop combo somewhere in the code.

	assert( OPENGL->modelview_stack.size() == 1 );

	// verify that state stacks are back where they started. if not,
	// it means there's a push/pop mismatch somewhere in the code.
	assert( rs_color_stack.size() == 1 );
	assert( rs_alpha_stack.length() == 1 );
	assert( rs_scale_stack.length() == 1 );
	assert( rs_align_stack.length() == 1 );

	OPENGL->clear_texture_bind();
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

	it does this at the world origin, so make sure the camera
	transform is reset before calling this
*/

constexpr int stats_draw_reserve = 10;
w_render* w_render::draw_stats()
{
#if !defined(_FINALRELEASE)
	maybe_draw_master_buffer( nullptr );

	RENDER->begin()->push_depth( zdepth_stats );

	if( show_stats )
	{
		std::vector<std::string> stat_lines;
		stat_lines.reserve( stats_draw_reserve );

		stat_lines.emplace_back( fmt::format( "RENDER : {} FPS ({:.1f} ms) / UPDATE : {} FPS",
											  stats.frame_count.value,
											  stats.frame_times_ms.value,
											  static_cast<int>( w_time::FTS_desired_frames_per_second ) ) );
		stat_lines.emplace_back( fmt::format( "RB: {}, V: {}, I: {}",
											  f_commas( stats.render_buffers.value ),
											  f_commas( stats.render_vertices.value ),
											  f_commas( stats.render_indices.value ) )
		);
		stat_lines.emplace_back( fmt::format( "Layers : {}", engine->layer_mgr->layer_stack.size() ) );
		stat_lines.emplace_back( fmt::format( "Entities : {}", f_commas( stats.num_entities.value ) ) );
		stat_lines.emplace_back( fmt::format( "Time Dilation: {:.2f}", engine->time->dilation ) );
		stat_lines.emplace_back( fmt::format( "Mouse VPos: {:.0f}, {:.0f}", engine->input->mouse_vwindow_pos.x, engine->input->mouse_vwindow_pos.y ) );

		if( stats.stat_custom_string.length() )
		{
			stat_lines.emplace_back( stats.stat_custom_string );
			stats.stat_custom_string = "";
		}

		assert( stat_lines.size() < stats_draw_reserve );
		int font_max_height = engine->pixel_font->font_def->max_height;

		RENDER->begin()
			->push_rgba( w_color( .25f, .25f, .25f, 0.75f ) )
			->draw_filled_rectangle( w_rect( 0.0f, 0.0f, v_window_w, static_cast<float>( font_max_height * stat_lines.size() ) ) )
			->pop_alpha()
			->push_depth_nudge()
			->push_rgb( w_color::white )
			->push_align( align::hcenter );

		float ypos = 0;
		for( const auto& iter : stat_lines )
		{
			RENDER->draw_string( engine->pixel_font, iter, w_rect( v_window_hw, ypos ) );
			ypos += font_max_height;
		}

		RENDER->end();
	}
	else
	{
		RENDER->begin()
			->push_align( align::right )
			->draw_string(
				engine->pixel_font,
				//fmt::format( "{} FPS", f_commas( 1000.f / stats.frame_times_ms.value ) ),
				fmt::format( "{} FPS ({:.2f} ms)", stats.frame_count.value, stats.frame_times_ms.value ),
				w_rect( v_window_w, 0 ) )
			->end();
	}
#endif

	RENDER->end();

	return this;
}

w_render* w_render::draw_filled_rectangle( const w_rect& dst )
{
	maybe_draw_master_buffer( engine->white_solid->tex );

	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();

	w_render_vert v0(
		w_vec2( dst.x, dst.y ),
		w_uv( 0, 0 ),
		rs_color
	);
	w_render_vert v1(
		w_vec2( dst.x + dst.w, dst.y ),
		w_uv( 1, 0 ),
		rs_color
	);
	w_render_vert v2(
		w_vec2( dst.x + dst.w, dst.y + dst.h ),
		w_uv( 1, 1 ),
		rs_color
	);
	w_render_vert v3(
		w_vec2( dst.x, dst.y + dst.h ),
		w_uv( 0, 1 ),
		rs_color
	);

	master_render_buffer->add_quad( v0, v1, v2, v3 );

	return this;
}

// draws an empty rectangle

w_render* w_render::draw_rectangle( const w_rect& dst )
{
	maybe_draw_master_buffer( engine->white_solid->tex );

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
	maybe_draw_master_buffer( engine->white_wire->tex );

	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();

	w_render_vert v0( w_vec2::zero, w_uv( 0, 0 ), rs_color );
	w_render_vert v1( w_vec2::zero, w_uv( 0, 0 ), rs_color );

	for( int x = 0; x < circle_sample_points_max; ++x )
	{
		v0.x = origin.x + ( circle_sample_points[ x ].x * radius );
		v0.y = origin.y + ( circle_sample_points[ x ].y * radius );

		v1.x = origin.x + ( circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
		v1.y = origin.y + ( circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

		master_render_buffer->add_line( v0, v1 );
	}

	return this;
}

// draws a line

w_render* w_render::draw_line( const w_vec2& start, const w_vec2& end )
{
	maybe_draw_master_buffer( engine->white_wire->tex );

	w_color rs_color = rs_color_stack.back();
	rs_color.a = rs_alpha_stack.back();

	w_render_vert v0( start, w_uv( 0, 0 ), rs_color );
	w_render_vert v1( end, w_uv( 0, 0 ), rs_color );

	master_render_buffer->add_line( v0, v1 );

	return this;
}

w_render* w_render::draw_point( const w_vec2& pos )
{
	maybe_draw_master_buffer( engine->white_wire->tex );

	draw_rectangle( w_rect( pos.x - 1, pos.y - 1, 2, 2 ) );

	return this;
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

	// nudge the rendering matrix down the height of the top row of subtextures. this
	// allows us to think of the top/left of this window as the actual graphical top/left.

	float xpos = dst.x;
	float ypos = dst.y;

	float inner_w = dst.w - p_00->sz.w - p_20->sz.w;
	float inner_h = dst.h - p_00->sz.h - p_02->sz.h;

	// top row

	draw( p_00, w_rect( xpos, ypos, p_00->sz.w, p_00->sz.h ) );

	xpos += p_00->sz.w;
	draw( p_10, w_rect( xpos, ypos, inner_w, p_10->sz.h ) );

	xpos += inner_w;
	draw( p_20, w_rect( xpos, ypos, p_20->sz.w, p_20->sz.h ) );

	// middle row

	xpos = dst.x;
	ypos += p_00->sz.h;
	draw( p_01, w_rect( xpos, ypos, p_01->sz.w, inner_h ) );

	xpos += p_01->sz.w;
	draw( p_11, w_rect( xpos, ypos, inner_w, inner_h ) );

	xpos += inner_w;
	draw( p_21, w_rect( xpos, ypos, p_21->sz.w, inner_h ) );

	// bottom row

	xpos = dst.x;
	ypos += inner_h;
	draw( p_02, w_rect( xpos, ypos, p_02->sz.w, p_02->sz.h ) );

	xpos += p_02->sz.w;
	draw( p_12, w_rect( xpos, ypos, inner_w, p_12->sz.h ) );

	xpos += inner_w;
	draw( p_22, w_rect( xpos, ypos, p_22->sz.w, p_22->sz.h ) );

	return this;
}

// PROJECTION MATRIX - getting stuff into screen space from camera space

void w_render::init_projection_matrix() const
{
	glm::mat4 projection = glm::mat4( 1.0f );
	projection = glm::ortho<float>(
		0, v_window_w, v_window_h, 0,
		-20000.0f, 20000.0f );

	glUniformMatrix4fv( glGetUniformLocation( engine->shader->id, "P" ), 1, GL_FALSE, glm::value_ptr( projection ) );
}

// VIEW MATRIX - getting stuff into camera space from worldspace
//
// if there is an active camera, use it's transform.

void w_render::init_view_matrix() const
{
	RENDER->draw_master_buffer();


	glm::mat4 view = glm::mat4( 1.0f );
	if( current_camera )
	{
		view = glm::translate( view, glm::vec3(
			-( current_camera->pos.x - v_window_hw ) / 2.0f,
			-( current_camera->pos.y - v_window_hh ) / 2.0f,
			0.0f ) );
	}
	glUniformMatrix4fv( glGetUniformLocation( engine->shader->id, "V" ), 1, GL_FALSE, glm::value_ptr( view ) );
}

// VIEW MATRIX - getting stuff into camera space from worldspace
//
// This sets the view matrix as identity for things like UI and mouse cursors.

void w_render::init_view_matrix_identity() const
{
	RENDER->draw_master_buffer();

	glm::mat4 view = glm::mat4( 1.0f );
	glUniformMatrix4fv( glGetUniformLocation( engine->shader->id, "V" ), 1, GL_FALSE, glm::value_ptr( view ) );
}

// call this function to figure out a new value based on the frame interpolation percentage.

float w_render::calc_interpolated_per_sec_value( float current_value, float step_per_second ) const
{
	return current_value + ( ( step_per_second * w_time::FTS_step_value_s ) * RENDER->frame_interpolate_pct );
}
