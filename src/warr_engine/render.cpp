
#include "master_pch.h"
#include "master_header.h"

void w_render_state::set_from_opt( w_render_state_opt& rso )
{
	align = rso.align.value_or( align );
	angle = rso.angle.value_or( angle );
	color = rso.color.value_or( color );
	glow = rso.glow.value_or( glow );
	pick_id = rso.angle.value_or( pick_id );
	scale = rso.scale.value_or( scale );
	snap_to_pixel = rso.snap_to_pixel.value_or( snap_to_pixel );
	uv_tiling = rso.uv_tiling.value_or( uv_tiling );
}

// ----------------------------------------------------------------------------

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
	clear_render_state_stack();

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

w_render* w_render::push()
{
	// duplicates the render_state at the top of the stack
	auto rs = render_state;
	render_states.emplace_back( std::move( rs ) );
	top_render_state = &( render_states.back() );

	assert( render_states.size() < max_render_states );

	return this;
}

w_render* w_render::pop()
{
	assert( render_states.size() > 1 );

	render_states.pop_back();
	top_render_state = &( render_states.back() );

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

w_color w_render::pal_color_from_idx( size_t idx )
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

void w_render::clear_render_state_stack()
{
	render_states.clear();

	w_render_state rs;
	render_states.emplace_back( std::move( rs ) );
	top_render_state = &( render_states.back() );
}

void w_render::draw_mesh( a_mesh* mesh )
{
	{
		scoped_opengl_push_pop;

		OPENGL->top()
			->rotate( render_state.angle )
			->scale( render_state.scale.x, render_state.scale.y );

		a_mesh_vertex* amv = {};

		for( int mv = 0 ; mv < mesh->mesh_verts.size() ; mv += 3 )
		{
			amv = &( mesh->mesh_verts[ mv + 0 ] );
			w_render_vertex v0( amv->pos, w_vec2( amv->uv.u * render_state.uv_tiling.u, amv->uv.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );

			amv = &( mesh->mesh_verts[ mv + 1 ] );
			w_render_vertex v1( amv->pos, w_vec2( amv->uv.u * render_state.uv_tiling.u, amv->uv.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );

			amv = &( mesh->mesh_verts[ mv + 2 ] );
			w_render_vertex v2( amv->pos, w_vec2( amv->uv.u * render_state.uv_tiling.u, amv->uv.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );

			engine->render->batch_triangles->add_primitive( mesh->tex, v0, v1, v2 );
		}

	#if 1
		render_state.color = w_color::black;
		render_state.z += zdepth_nudge;

		for( int mv = 0 ; mv < mesh->mesh_verts.size() ; mv += 3 )
		{
			amv = &( mesh->mesh_verts[ mv + 0 ] );
			w_render_vertex v0( amv->pos, w_vec2( amv->uv.u * render_state.uv_tiling.u, amv->uv.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );

			amv = &( mesh->mesh_verts[ mv + 1 ] );
			w_render_vertex v1( amv->pos, w_vec2( amv->uv.u * render_state.uv_tiling.u, amv->uv.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );

			amv = &( mesh->mesh_verts[ mv + 2 ] );
			w_render_vertex v2( amv->pos, w_vec2( amv->uv.u * render_state.uv_tiling.u, amv->uv.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );

			engine->render->batch_lines->add_primitive( mesh->tex, v0, v1 );
			engine->render->batch_lines->add_primitive( mesh->tex, v1, v2 );
			engine->render->batch_lines->add_primitive( mesh->tex, v2, v0 );
		}
	#endif
	}
}

// draws a texture as a sprite onto the screen.
//
// this offsets along left and up by half the texture size, which centers the
// quad being drawn at 0,0,0.

void w_render::draw_sprite( const a_texture* texture, const w_vec2& dst )
{
	float w = texture->rc.w;
	float h = texture->rc.h;

	w *= render_state.scale.x;
	h *= render_state.scale.y;

	float hw = w / 2.0f;
	float hh = h / 2.0f;

	w_render_vertex v0( w_vec2( -hw, hh ), w_vec2( texture->uv00.u * render_state.uv_tiling.u, texture->uv00.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );
	w_render_vertex v1( w_vec2( hw, hh ), w_vec2( texture->uv11.u * render_state.uv_tiling.u, texture->uv00.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );
	w_render_vertex v2( w_vec2( hw, -hh ), w_vec2( texture->uv11.u * render_state.uv_tiling.u, texture->uv11.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );
	w_render_vertex v3( w_vec2( -hw, -hh ), w_vec2( texture->uv00.u * render_state.uv_tiling.u, texture->uv11.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );

	{
		scoped_opengl_push_pop;
		OPENGL->top()
			->translate( { dst.x, dst.y } )
			->rotate( render_state.angle );

		engine->render->batch_quads->add_primitive( texture, v0, v1, v2, v3 );
	}
}

// draws a textured quad

void w_render::draw( const a_texture* texture, const w_rect& dst )
{
	float w = dst.w ? dst.w : texture->rc.w;
	float h = dst.h ? dst.h : texture->rc.h;

	w *= render_state.scale.x;
	h *= render_state.scale.y;

	w_render_vertex v0( w_vec2( 0.0f, h ), w_vec2( texture->uv00.u * render_state.uv_tiling.u, texture->uv00.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );
	w_render_vertex v1( w_vec2( w, h ), w_vec2( texture->uv11.u * render_state.uv_tiling.u, texture->uv00.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );
	w_render_vertex v2( w_vec2( w, 0.0f ), w_vec2( texture->uv11.u * render_state.uv_tiling.u, texture->uv11.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );
	w_render_vertex v3( w_vec2( 0.0f, 0.0f ), w_vec2( texture->uv00.u * render_state.uv_tiling.u, texture->uv11.v * render_state.uv_tiling.v ), render_state.color, render_state.glow );

	{
		scoped_opengl_push_pop;

		OPENGL->top()->translate( { dst.x, dst.y } );

		engine->render->batch_quads->add_primitive( texture, v0, v1, v2, v3 );
	}
}

void w_render::draw_tiled( const a_texture* texture, const w_rect& dst )
{
	{
		scoped_render_push_pop;

		render_state.uv_tiling = w_vec2::compute_uv_tiling( texture, dst );

		w_render::draw( texture, dst );
	}
}

// draws a string from a bitmap font, char by char, as textured quads

void w_render::draw_string( const std::string_view text, const w_pos& pos )
{
	// not specifying a font means you want to use the default font
	w_render::draw_string( engine->pixel_font, text, pos );
}

void w_render::draw_string( const a_font* font, const std::string_view text, const w_pos& pos )
{
	w_vec2 alignment_pos_adjustment( 0.0f, 0.0f );

	if( render_state.align & align::hcenter )
	{
		w_vec2 extents = font->get_string_extents( text ) * render_state.scale.x;
		alignment_pos_adjustment.x -= extents.x / 2.0f;
	}

	if( render_state.align & align::right )
	{
		w_vec2 extents = font->get_string_extents( text ) * render_state.scale.x;
		alignment_pos_adjustment.x -= extents.x;
	}

	if( render_state.align & align::vcenter )
	{
		alignment_pos_adjustment.y -= ( font->font_def->max_height * render_state.scale.y ) / 2.0f;
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
			w_render::draw(
				fch->glyph_texture.get(),
				w_rect( xpos + ( fch->xoffset * render_state.scale.x ), ypos + ( fch->yoffset * render_state.scale.y ) )
			);
		}

		xpos += ( fch->xadvance * render_state.scale.x);
	}
}

// called at the start of each frame to set up and clear the screen

void w_render::begin_frame()
{
	// ----------------------------------------------------------------------------
	// set up the viewport for a new frame
	// ----------------------------------------------------------------------------

	glViewport( 0, 0, (int)v_window_w, (int)v_window_h );

	// clear for depth only
	glClear( GL_DEPTH_BUFFER_BIT );

	// clear specific color attachments
	engine->frame_buffer->color_attachments[ 0 ].clear();	// main
	engine->frame_buffer->color_attachments[ 1 ].clear();	// glow
}

// called at end of each frame to finalize and render all buffers

void w_render::end_frame()
{
	OPENGL->init_view_matrix_identity_ui();

	// stats

	engine->stats->frame_times_ms += engine->time->delta_ms;
	engine->stats->frame_count++;

	w_render::draw_stats();

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

	clear_render_state_stack();
}

// draws the X and Y axis in the viewport at the world origin
//
// positive directions are brighter than the negative ones

void w_render::draw_world_axis()
{
	render_state.color = { 1.0f, 0.0f, 0.0f };
	w_render::draw_line( w_vec2::zero, w_vec2( 5000, 0 ) );

	render_state.color = { 0.5f, 0.0f, 0.0f };
	w_render::draw_line( w_vec2::zero, w_vec2( -5000, 0 ) );

	render_state.color = { 0.0f, 1.0f, 0.0f };
	w_render::draw_line( w_vec2::zero, w_vec2( 0, 5000 ) );

	render_state.color = { 0.0f, 0.5f, 0.0f };
	w_render::draw_line( w_vec2::zero, w_vec2( 0, -5000 ) );
}

// draws useful stats at the top of the screen

static std::vector<std::string> draw_stat_line_buffer;

void w_render::draw_stats()
{
#if !defined(_FINALRELEASE)
	{
		scoped_render_push_pop;

		draw_stat_line_buffer.clear();
		render_state.z = zdepth_stats;

		if( engine->render->show_stats )
		{
			draw_stat_line_buffer.clear();
			draw_stat_line_buffer.emplace_back( fmt::format( "RENDER : {} FPS ({:.1f} ms) / FTS: {} FPS",
				f_commas( engine->stats->frame_count.value ),
				engine->stats->frame_times_ms.value,
				fixed_time_step::frames_per_second ) );
			draw_stat_line_buffer.emplace_back( fmt::format( "DC:{} / Q:{} / T:{} / L:{} / P:{}",
				f_commas( engine->stats->draw_calls.value ),
				f_commas( engine->stats->quads.value ),
				f_commas( engine->stats->triangles.value ),
				f_commas( engine->stats->lines.value ),
				f_commas( engine->stats->points.value ) )
			);
			draw_stat_line_buffer.emplace_back( fmt::format( "Layers : {}", engine->layer_mgr->layer_stack.size() ) );
			draw_stat_line_buffer.emplace_back( fmt::format( "Entities : {}", f_commas( engine->stats->entities.value ) ) );
			draw_stat_line_buffer.emplace_back( fmt::format( "Time Dilation: {:.2f}", engine->time->dilation ) );
			draw_stat_line_buffer.emplace_back( fmt::format( "Mouse: W:{:.0f}, {:.0f} / V:{:.0f}, {:.0f} / U:{:.0f}, {:.0f}",
				engine->input->mouse_window_pos.x, engine->input->mouse_window_pos.y,
				engine->input->mouse_vwindow_pos.x, engine->input->mouse_vwindow_pos.y,
				engine->input->mouse_uiwindow_pos.x, engine->input->mouse_uiwindow_pos.y )
			);

			if( engine->stats->stat_custom_string.length() )
			{
				draw_stat_line_buffer.emplace_back( engine->stats->stat_custom_string );
				engine->stats->stat_custom_string = "";
			}

			{
				scoped_render_push_pop;

				render_state.color = w_color::pal( 0 );
				render_state.color.a = 0.75f;
				w_render::draw_filled_rectangle(
					w_rect( 0.0f, 0.0f,
					ui_window_w, (float)( engine->pixel_font->font_def->max_height * draw_stat_line_buffer.size() ) )
				);
			}

			{
				scoped_render_push_pop;

				render_state.z += zdepth_nudge;

				render_state = {
					.align = align::hcenter,
					.color = w_color::white
				};

				auto ypos = 0.0f;
				for( const auto& iter : draw_stat_line_buffer )
				{
					w_render::draw_string( iter, { ui_window_hw, ypos } );
					ypos += engine->pixel_font->font_def->max_height;
				}
			}
		}
		else
		{
			{
				scoped_render_push_pop;

				render_state.align = align::right;
				w_render::draw_string(
					fmt::format( "{} FPS ({:.2f} ms)", f_commas( engine->stats->frame_count.value ), engine->stats->frame_times_ms.value ),
					{ ui_window_w, 0.0f } );
			}
		}
	}
#endif
}

void w_render::draw_filled_rectangle( const w_rect& dst )
{
	w_render_vertex v0(
		w_vec2( dst.x, dst.y ),
		w_uv( 0, 0 ),
		render_state.color,
		render_state.glow
	);
	w_render_vertex v1(
		w_vec2( dst.x, dst.y + dst.h ),
		w_uv( 0, 1 ),
		render_state.color,
		render_state.glow
	);
	w_render_vertex v2(
		w_vec2( dst.x + dst.w, dst.y + dst.h ),
		w_uv( 1, 1 ),
		render_state.color,
		render_state.glow
	);
	w_render_vertex v3(
		w_vec2( dst.x + dst.w, dst.y ),
		w_uv( 1, 0 ),
		render_state.color,
		render_state.glow
	);

	engine->render->batch_quads->add_primitive( engine->tex_white, v0, v1, v2, v3 );
}

void w_render::draw_triangle( const w_vec2& v0, const w_vec2& v1, const w_vec2& v2 )
{
	w_render::draw_line( v0, v1 );
	w_render::draw_line( v1, v2 );
	w_render::draw_line( v2, v0 );
}

void w_render::draw_filled_triangle( const w_vec2& v0, const w_vec2& v1, const w_vec2& v2 )
{
	w_render_vertex rv0(
		w_vec2( v0.x, v0.y ),
		w_uv( 0, 0 ),
		render_state.color,
		render_state.glow
	);
	w_render_vertex rv1(
		w_vec2( v1.x, v1.y ),
		w_uv( 1, 0 ),
		render_state.color,
		render_state.glow
	);
	w_render_vertex rv2(
		w_vec2( v2.x, v2.y ),
		w_uv( 1, 1 ),
		render_state.color,
		render_state.glow
	);

	engine->render->batch_triangles->add_primitive( engine->tex_white, rv0, rv1, rv2 );
}

// draws an empty rectangle

void w_render::draw_rectangle( const w_rect& dst )
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

	w_render::draw_line( w_vec2( box.min.x, box.min.y ), w_vec2( box.max.x, box.min.y ) );
	w_render::draw_line( w_vec2( box.max.x, box.min.y ), w_vec2( box.max.x, box.max.y ) );
	w_render::draw_line( w_vec2( box.max.x, box.max.y ), w_vec2( box.min.x, box.max.y ) );
	w_render::draw_line( w_vec2( box.min.x, box.max.y ), w_vec2( box.min.x, box.min.y ) );
}

// draws a circle with line segments

void w_render::draw_circle( const w_vec2& origin, float radius )
{
	w_render_vertex v0( w_vec2::zero, w_uv( 0, 0 ), render_state.color, render_state.glow );
	w_render_vertex v1( w_vec2::zero, w_uv( 0, 0 ), render_state.color, render_state.glow );

	{
		scoped_render_push_pop;

		render_state.snap_to_pixel = false;

		for( auto x = 0; x < circle_sample_points_max; ++x )
		{
			v0.x = origin.x + ( engine->render->circle_sample_points[ x ].x * radius );
			v0.y = origin.y + ( engine->render->circle_sample_points[ x ].y * radius );

			v1.x = origin.x + ( engine->render->circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
			v1.y = origin.y + ( engine->render->circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

			engine->render->batch_lines->add_primitive( engine->tex_white, v0, v1 );
		}
	}
}

void w_render::draw_filled_circle( const w_vec2& origin, float radius )
{
	w_render_vertex v0( origin, w_uv( 0, 0 ), render_state.color, render_state.glow );
	w_render_vertex v1 = v0;
	w_render_vertex v2 = v0;

	{
		scoped_render_push_pop;

		render_state.snap_to_pixel = false;

		for( auto x = 0; x < circle_sample_points_max; ++x )
		{
			v1.x = origin.x + ( engine->render->circle_sample_points[ x ].x * radius );
			v1.y = origin.y + ( engine->render->circle_sample_points[ x ].y * radius );

			v2.x = origin.x + ( engine->render->circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
			v2.y = origin.y + ( engine->render->circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

			engine->render->batch_triangles->add_primitive( engine->tex_white, v0, v1, v2 );
		}
	}
}

void w_render::draw_line( const w_vec2& start, const w_vec2& end )
{
	w_render_vertex v0( start, w_uv( 0, 0 ), render_state.color, render_state.glow );
	w_render_vertex v1( end, w_uv( 0, 0 ), render_state.color, render_state.glow );

	render_state.snap_to_pixel = false;

	engine->render->batch_lines->add_primitive(
		engine->tex_white,
		v0,
		v1
	);
}

void w_render::draw_point( const w_vec2& pos )
{
	w_render_vertex v0( pos, w_uv( 0, 0 ), render_state.color, render_state.glow );

	{
		scoped_render_push_pop;

		render_state.snap_to_pixel = false;

		engine->render->batch_points->add_primitive(
			engine->tex_white,
			v0
		);
	}
}

void w_render::draw_sliced( const a_9slice_def* slice_def, const w_rect& dst )
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
}

// call this function to figure out a new value based on the frame interpolation percentage.

float w_render::calc_interpolated_per_sec_value( float current_value, float step_per_second ) const
{
	return current_value + ( ( step_per_second * fixed_time_step::per_second_scaler ) * RENDER->frame_interpolate_pct );
}

// samples the "pick" frame buffer at click_pos and returns the pick_id found
// there.

int w_render::sample_pick_id_at( w_vec2 click_pos )
{
	engine->frame_buffer->bind();
	glReadBuffer( GL_COLOR_ATTACHMENT0 + 2 );

	// texture is flipped vertically from screen space
	click_pos.y = v_window_h - click_pos.y;

	// read single pixel back from texture to see what was at click_pos
	float pixel[ 4 ];
	glReadPixels( (int)click_pos.x, (int)click_pos.y, 1, 1, GL_RGBA, GL_FLOAT, &pixel );

	return static_cast<int>( pixel[ 0 ] );
}
