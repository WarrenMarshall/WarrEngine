
#include "master_pch.h"
#include "master_header.h"

namespace war
{

render_state* render::state = nullptr;

// the current palette that all rendering references
palette_asset render::palette = {};

void render::init()
{
	init_generate_circle_sample_points();

	// renderer batches
	dynamic_batches.init();

	// MODEL MATRIX (getting stuff into worldspace from model space)
	//
	// push an identity matrix onto the stack. this will always live as the
	// first element in the stack and should be the only matrix remaining when
	// the frame finishes rendering.

	g_engine->render_api.model_matrix_push_identity();

	// initialize render state stacks

	//render_states.reserve( max_render_states );
	clear_render_state_stack();
}

void render::init_set_up_default_palette()
{
	// create a default palette in case the app doesn't specify one

	render::palette = {};
	render::palette.colors.push_back( color::black );
	render::palette.colors.push_back( color::dark_grey );
	render::palette.colors.push_back( color::grey );
	render::palette.colors.push_back( color::light_grey );
	render::palette.colors.push_back( color::white );
}

// generate the sample points for drawing a circle. these verts sit on a
// unit circle and are scaled to match the radius requested for each circle
// we draw.

void render::init_generate_circle_sample_points()
{
	circle_sample_points.clear();
	circle_sample_points.reserve( circle_sample_points_max );

	float angle = 0;
	float angle_step = 360.f / (float)( circle_sample_points_max );

	for( auto x = 0 ; x < circle_sample_points_max ; ++x )
	{
		circle_sample_points.push_back( vec2::dir_from_angle( angle ) );
		angle += angle_step;
	}
}

// clear the render state stack and add the default state back in

void render::clear_render_state_stack()
{
	render_states.clear();

	render_state rs;
	rs.batch_render_target = &dynamic_batches;
	render_states.push_back( rs );
	render::state = &render_states.back();
}

void render::draw_mesh( mesh_asset* mesh )
{
	{
		scoped_opengl;

		for( auto& [ texture, triangle_list ] : mesh->texture_to_triangles )
		{
			auto tex = texture->get_frame( render::state->anim_offset );

			for( auto& tri : triangle_list )
			{
				render_vertex v0(
					vec3( tri.verts[0].x, tri.verts[ 0 ].y, tri.verts[ 0 ].z ),
					vec2( tri.verts[0].u * render::state->uv_tiling.u, tri.verts[0].v * render::state->uv_tiling.v ),
					render::state->color,
					render::state->glow
				);
				render_vertex v1(
					vec3( tri.verts[ 1 ].x, tri.verts[ 1 ].y, tri.verts[ 1 ].z ),
					vec2( tri.verts[ 1 ].u * render::state->uv_tiling.u, tri.verts[ 1 ].v * render::state->uv_tiling.v ),
					render::state->color,
					render::state->glow
				);
				render_vertex v2(
					vec3( tri.verts[ 2 ].x, tri.verts[ 2 ].y, tri.verts[ 2 ].z ),
					vec2( tri.verts[ 2 ].u * render::state->uv_tiling.u, tri.verts[ 2 ].v * render::state->uv_tiling.v ),
					render::state->color,
					render::state->glow
				);

				render::state->batch_render_target->add_triangle( tex, &v0, &v1, &v2 );
			}
		}

	#ifndef _FINAL_RELEASE
		if( g_engine->renderer.debug.is_drawing_debug_info() )
		{
			// wireframe overlay

			render::state->color = color::grey;
			render::state->z += zdepth_nudge;

			for( auto& [texture, triangle_list] : mesh->texture_to_triangles )
			{
				for( auto& tri : triangle_list )
				{
					render_vertex v0( vec3( tri.verts[ 0 ].x, tri.verts[ 0 ].y, tri.verts[ 0 ].z ), vec2( 0.f, 0.f ), render::state->color, render::state->glow );
					render_vertex v1( vec3( tri.verts[ 1 ].x, tri.verts[ 1 ].y, tri.verts[ 1 ].z ), vec2( 0.f, 0.f ), render::state->color, render::state->glow );
					render_vertex v2( vec3( tri.verts[ 2 ].x, tri.verts[ 2 ].y, tri.verts[ 2 ].z ), vec2( 0.f, 0.f ), render::state->color, render::state->glow );

					render::state->batch_render_target->add_line( g_engine->tex_white, &v0, &v1 );
					render::state->batch_render_target->add_line( g_engine->tex_white, &v1, &v2 );
					render::state->batch_render_target->add_line( g_engine->tex_white, &v2, &v0 );

				}
			}
		}
	#endif
	}
}

// draws a texture as a sprite onto the screen.
//
// this offsets along left and up by half the texture size, which centers the
// quad being drawn at 0,0,0.

void render::draw_sprite( texture_asset* texture, const vec2& dst )
{
	auto frame = texture->get_frame( render::state->anim_offset );

	float hw = frame->rc.w / 2.f;
	float hh = frame->rc.h / 2.f;

	render_vertex v0(
		vec2( -hw, -hh ),
		vec2( frame->uv00.u, frame->uv11.v ),
		render::state->color, render::state->glow );

	render_vertex v1(
		vec2( hw, -hh ),
		vec2( frame->uv11.u * render::state->uv_tiling.u, frame->uv11.v ),
		render::state->color, render::state->glow );

	render_vertex v2(
		vec2( hw, hh ),
		vec2( frame->uv11.u * render::state->uv_tiling.u, frame->uv00.v * render::state->uv_tiling.v ),
		render::state->color, render::state->glow );

	render_vertex v3(
		vec2( -hw, hh ),
		vec2( frame->uv00.u, frame->uv00.v * render::state->uv_tiling.v ),
		render::state->color, render::state->glow );

	{
		scoped_opengl;
		g_engine->render_api.top_matrix->apply_transform( { dst.x, dst.y }, render::state->angle, render::state->scale );

		render::state->batch_render_target->add_quad( frame, &v0, &v1, &v2, &v3 );
	}
}

// draws a textured quad

void render::draw_quad( texture_asset* texture, const vec2& dst )
{
	texture_asset* frame = texture->get_frame( render::state->anim_offset );

	render::draw_quad( texture, rect( dst.x, dst.y, frame->rc.w, frame->rc.h ) );
}

void render::draw_quad( texture_asset* texture, const rect& dst )
{
	auto frame = texture->get_frame( render::state->anim_offset );

	float w = dst.w ? dst.w : frame->rc.w;
	float h = dst.h ? dst.h : frame->rc.h;

	render_vertex v0(
		vec2( 0.f, 0.f ),
		vec2( frame->uv00.u, frame->uv11.v * render::state->uv_tiling.v ),
		render::state->color, render::state->glow );

	render_vertex v1(
		vec2( w, 0.0 ),
		vec2( frame->uv11.u * render::state->uv_tiling.u, frame->uv11.v * render::state->uv_tiling.v ),
		render::state->color, render::state->glow );

	render_vertex v2(
		vec2( w, h ),
		vec2( frame->uv11.u * render::state->uv_tiling.u, frame->uv00.v ),
		render::state->color, render::state->glow );

	render_vertex v3(
		vec2( 0.f, h ),
		vec2( frame->uv00.u, frame->uv00.v ),
		render::state->color, render::state->glow );

	{
		scoped_opengl;
		g_engine->render_api.top_matrix->apply_transform( { dst.x, dst.y }, render::state->angle, render::state->scale );

		render::state->batch_render_target->add_quad( frame, &v0, &v1, &v2, &v3 );
	}
}

void render::draw_tiled( texture_asset* texture, const rect& dst )
{
	scoped_render_state;

	texture_asset* frame = texture->get_frame( render::state->anim_offset );

	render::state->uv_tiling = vec2::compute_uv_tiling( frame, dst );

	render::draw_quad( frame, dst );
}

// draws a string from a bitmap font, char by char, as textured quads
//
// returns: the x,y position immediately following the last character that was
// drawn.

vec2 render::draw_string( const std::string& text, const vec2& pos )
{
	auto extents = render::state->font->get_string_extents( text );

	vec2 alignment_pos_adjustment( 0.f, 0.f );

	if( render::state->align & align::hcenter )
	{
		alignment_pos_adjustment.x -= ( extents.x * render::state->scale.x ) / 2.f;
	}

	if( render::state->align & align::right )
	{
		alignment_pos_adjustment.x -= extents.x * render::state->scale.x;
	}

	if( render::state->align & align::vcenter )
	{
		alignment_pos_adjustment.y += ( render::state->font->get_max_height() * render::state->scale.y ) / 2.f;
	}

	// ----------------------------------------------------------------------------

	vec2 draw_pos = { pos.x + alignment_pos_adjustment.x, pos.y - alignment_pos_adjustment.y };

	for( const char iter : text )
	{
		font_def_asset::glyph* fch = &( render::state->font->font_def->char_map[ (int)( iter ) ] );

		// small optimization to skip drawing completely blank characters
		if( fch->w > 0 )
		{
			render::draw_quad(
				&fch->glyph_texture,
				rect(
				draw_pos.x + ( fch->xoffset * render::state->scale.x ),
				draw_pos.y + ( fch->yoffset * render::state->scale.y )
			)
			);
		}

		draw_pos.x += ( fch->xadvance * render::state->scale.x );
	}

	return draw_pos;
}

// returns: the x,y position immediately following the last character that was
// drawn on the last line.

vec2 render::draw_string( const std::vector<std::string>& text, const vec2& pos )
{
	vec2 wk_pos = pos;

	vec2 draw_pos;

	for( auto& iter : text )
	{
		draw_pos = render::draw_string( iter, wk_pos );

		wk_pos.y += render::state->font->get_max_height();
	}

	return draw_pos;
}

vec2 render::draw_string( const std::vector<std::string>& text, const rect& rc )
{
	vec2 wk_pos = { rc.x, rc.y };

	if( render::state->align & align::right )
	{
		wk_pos.x = rc.x + rc.w;
	}
	if( render::state->align & align::hcenter )
	{
		wk_pos.x = rc.x + ( rc.w / 2.f );
	}
	if( render::state->align & align::vcenter )
	{
		wk_pos.y = rc.y + ( render::state->font->get_max_height() / 2.f );
	}

	vec2 draw_pos;

	for( auto& iter : text )
	{
		draw_pos = render::draw_string( iter, wk_pos );

		wk_pos.y += render::state->font->get_max_height();

		if( wk_pos.y >= rc.y + rc.h )
		{
			break;
		}
	}

	return draw_pos;
}

std::vector<std::string> render::wrap_string_to_width( std::string_view text, float width )
{
	std::vector<std::string> wrapped_lines;
	auto space_char_extents = render::state->font->get_string_extents( " " );

	tokenizer tok;
	tok.init( text, " " );


	// ----------------------------------------------------------------------------
	// each token that contains newline characters must be broken down into
	// individual tokens, preserving the newlines as standalone tokens. this
	// will allow the wrapping code below to work with computed newlines as well
	// as manually placed ones.

	std::vector<std::string_view> new_tokens;

	for( auto& token : tok.tokens )
	{
		bool has_newlines = token.find_first_of( '\n' ) != std::string::npos;

		if( has_newlines )
		{
			// break the line up by newline chars
			size_t idx, start = 0;
			for( idx = 0 ; idx < token.size() ; ++idx )
			{
				char ch = token[ idx ];

				if( ch == '\n' )
				{
					if( start == idx )
					{
						new_tokens.push_back( token.substr( start, 1 ) );
						start++;
					}
					else
					{
						new_tokens.push_back( token.substr( start, idx - start ) );
						new_tokens.push_back( token.substr( idx, 1 ) );
						start = idx + 1;
					}

				}
			}

			// catch the last word in the token
			if( start != idx )
			{
				new_tokens.push_back( token.substr( start, idx - start ) );
			}
		}
		else
		{
			new_tokens.push_back( token );
		}
	}

	// ----------------------------------------------------------------------------

	float line_width = 0.f;
	std::string wk_line;

	for( auto& iter : new_tokens )
	{
		bool is_newline = ( iter == "\n" );

		auto word_extents = render::state->font->get_string_extents( iter );

		if( is_newline or line_width + space_char_extents.x + word_extents.x > width )
		{
			wrapped_lines.push_back( wk_line );

			if( is_newline )
			{
				wk_line.clear();
				line_width = 0;
			}
			else
			{
				wk_line = iter;
				line_width = word_extents.x;
			}
		}
		else
		{
			if( !wk_line.empty() )
			{
				wk_line += " ";
			}
			wk_line += iter;

			line_width += space_char_extents.x + word_extents.x;
		}
	}

	// add the final line into the list

	wrapped_lines.push_back( wk_line );

	return wrapped_lines;
}

// called at the start of each frame to set up and clear the screen

void render::begin_frame()
{
	// reset glviewport
	glViewport( 0, 0, (int)viewport_w, (int)viewport_h );

	// clear color attachments
	g_engine->frame_buffer->clear_color_attachments();
	g_engine->blur_frame_buffer->clear_color_attachments();
	g_engine->composite_frame_buffer->clear_color_attachments();
}

// called at end of each frame to finalize and render all buffers

void render::end_frame()
{
	// stats - use the UI view matrix since stats are drawn as if they are UI

	g_engine->render_api.set_view_matrix_identity_ui();

	g_engine->stats.frame_times_ms += g_engine->time.delta_ms;
	g_engine->stats.frame_count++;

	g_engine->stats.draw();

	// flush the final batches

	dynamic_batches.flush_and_reset_internal( draw_call::opaque );
	dynamic_batches.flush_and_reset_internal( draw_call::transparent );

	// there should be a single model matrix left on the stack (the identity
	// matrix we created at renderer start up). If there is any other number,
	// then there is an uneven push/pop combo somewhere in the code.

	assert( g_engine->render_api.model_matrix_stack.size() == 1 );

	clear_render_state_stack();

#ifndef _FINAL_RELEASE
	g_engine->renderer.debug.single_frame_log = false;
	g_engine->renderer.debug.entity_info_log = false;
#endif
}

// draws the X and Y axis in the viewport at the world origin
//
// positive directions are brighter than the negative ones

void render::draw_world_axis()
{
	{
		scoped_render_state;

		render::state->z = zdepth_topmost;

		render::state->color = make_color( color::red, 0.5f );
		render::draw_line( vec2::zero, vec2( 5000, 0 ) );

		render::state->color = make_color( color::dark_red, 0.5f );
		render::draw_line( vec2::zero, vec2( -5000, 0 ) );

		render::state->color = make_color( color::green, 0.5f );
		render::draw_line( vec2::zero, vec2( 0, 5000 ) );

		render::state->color = make_color( color::dark_green, 0.5f );
		render::draw_line( vec2::zero, vec2( 0, -5000 ) );
	}
}

// draws an empty rectangle

void render::draw_rect( const rect& dst )
{
	render::draw_line_loop( dst );
}

void render::draw_rounded_rect( const rect& dst, float radius )
{
	// make sure there is enough room for the rounded corners
	assert( dst.w >= radius * 2.f or dst.h >= radius * 2.f );

	render::draw_circle( { dst.x + radius, dst.y + radius }, radius, corner::top_left );
	render::draw_circle( { dst.x + dst.w - radius, dst.y + radius }, radius, corner::top_right );
	render::draw_circle( { dst.x + dst.w - radius, dst.y + dst.h - radius }, radius, corner::bottom_right );
	render::draw_circle( { dst.x + radius, dst.y + dst.h - radius }, radius, corner::bottom_left );

	render::draw_line( { dst.x + radius, dst.y }, { dst.x + dst.w - radius, dst.y } );
	render::draw_line( { dst.x + dst.w, dst.y + radius }, { dst.x + dst.w, dst.y + dst.h - radius } );
	render::draw_line( { dst.x + dst.w - radius, dst.y + dst.h }, { dst.x + radius, dst.y + dst.h } );
	render::draw_line( { dst.x, dst.y + dst.h - radius }, { dst.x, dst.y + radius } );
}

void render::draw_rounded_filled_rect( const rect& dst, float radius )
{
	// make sure there is enough room for the rounded corners
	assert( dst.w >= radius * 2.f or dst.h >= radius * 2.f );

	render::draw_filled_circle( { dst.x + radius, dst.y + radius }, radius, corner::top_left );
	render::draw_filled_circle( { dst.x + dst.w - radius, dst.y + radius }, radius, corner::top_right );
	render::draw_filled_circle( { dst.x + dst.w - radius, dst.y + dst.h - radius }, radius, corner::bottom_right );
	render::draw_filled_circle( { dst.x + radius, dst.y + dst.h - radius }, radius, corner::bottom_left );

	float dradius = radius * 2.f;
	render::draw_filled_rect( { dst.x + radius, dst.y, dst.w - dradius, radius } );
	render::draw_filled_rect( { dst.x, dst.y + radius, dst.w, dst.h - dradius } );
	render::draw_filled_rect( { dst.x + radius, dst.y + dst.h - radius, dst.w - dradius, radius } );
}

void render::draw_filled_rect( const rect& dst )
{
	render_vertex v0(
		vec2( dst.x, dst.y ),
		vec2( 0, 0 ),
		render::state->color,
		render::state->glow
	);
	render_vertex v1(
		vec2( dst.x, dst.y + dst.h ),
		vec2( 0, 1 ),
		render::state->color,
		render::state->glow
	);
	render_vertex v2(
		vec2( dst.x + dst.w, dst.y + dst.h ),
		vec2( 1, 1 ),
		render::state->color,
		render::state->glow
	);
	render_vertex v3(
		vec2( dst.x + dst.w, dst.y ),
		vec2( 1, 0 ),
		render::state->color,
		render::state->glow
	);

	render::state->batch_render_target->add_quad( g_engine->tex_white, &v0, &v1, &v2, &v3 );
}

void render::draw_triangle( const vec2& v0, const vec2& v1, const vec2& v2 )
{
	render::draw_line_loop( { v0, v1, v2 } );
}

void render::draw_tile_map( tile_set_asset* tile_set, tile_map_asset* tile_map, const vec2& pos )
{
	for( auto& layer : tile_map->layers )
	{
		if( !layer.is_visible )
		{
			continue;
		}

		for( auto& chunk : layer.chunks )
		{
			for( auto y = 0 ; y < chunk.tilemap_bounds.h ; ++y )
			{
				for( auto x = 0 ; x < chunk.tilemap_bounds.w ; ++x )
				{
					auto tile = &( chunk.tiles[ ( y * (int)chunk.tilemap_bounds.w ) + x ] );

					if( tile->idx == tile_map_asset::tile::empty )
					{
						continue;
					}

					bool flip_h = tile->flags & tile_flags::flipped_horizontally;
					bool flip_v = tile->flags & tile_flags::flipped_vertically;
					bool flip_d = tile->flags & tile_flags::flipped_diagonally;

					render::state->scale = vec2( 1.f, 1.f );
					render::state->angle = 0.f;

					if( flip_d )
					{
						if( flip_h and !flip_v )
						{
							render::state->angle = 90.f;
						}
						else if( !flip_h and !flip_v )
						{
							render::state->angle = 180.f;
						}
						else if( !flip_h and flip_v )
						{
							render::state->angle = 270.f;
						}
					}
					else
					{
						if( flip_h and flip_v )
						{
							render::state->angle = 180.f;
						}
						else if( flip_h and !flip_v )
						{
							render::state->scale.x = -1.f;
						}
						else if( !flip_h and flip_v )
						{
							render::state->scale.y = -1.f;
						}
					}

					vec2 tile_pos =
					{
						pos.x + ( ( x + chunk.tilemap_bounds.x ) * tile_map->tile_sz ) + ( tile_map->tile_sz / 2.f ),
						pos.y + ( ( y + chunk.tilemap_bounds.y ) * tile_map->tile_sz ) + ( tile_map->tile_sz / 2.f )
					};

					texture_asset* tile_set_texture = &tile_set->tile_definitions[ tile->idx ].texture;
					render::draw_sprite( tile_set_texture, tile_pos );
				}
			}
		}
	}
}

// utility function for drawing a fun marker. not meant for games, really.

void render::draw_crosshair( vec2 pos )
{
	scoped_render_state;

	const float sz = 7.f;
	const float hsz = sz / 2.f;
	const float spoke_sz = sz / 3.f;

	auto l_draw_spoke = [&] ( vec2 offset )
	{
		render::draw_line( pos + offset, pos + ( offset * spoke_sz ) );
	};

	render::state->color = make_color( color::grey, 0.25f );
	l_draw_spoke( { hsz, 0.f } );
	l_draw_spoke( { -hsz, 0.f } );
	l_draw_spoke( { 0.f, hsz } );
	l_draw_spoke( { 0.f, -hsz } );

}

void render::draw_filled_triangle( const vec2& v0, const vec2& v1, const vec2& v2 )
{
	render_vertex rv0( v0 );
	render_vertex rv1( v1 );
	render_vertex rv2( v2 );

	render::state->batch_render_target->add_triangle( g_engine->tex_white, &rv0, &rv1, &rv2 );
}

auto render::get_circle_start_end_indices( e_corner corner )
{
	int start, end;
	int quarter_circle = circle_sample_points_max / 4;

	// default to drawing the full circle
	start = 0;
	end = circle_sample_points_max;

	switch( corner )
	{
		case corner::top_left:
		{
			start = quarter_circle * 3;
			end = quarter_circle * 4;
			break;
		}

		case corner::bottom_left:
		{
			start = quarter_circle * 2;
			end = quarter_circle * 3;
			break;
		}

		case corner::bottom_right:
		{
			start = quarter_circle;
			end = quarter_circle * 2;
			break;
		}

		case corner::top_right:
		{
			start = 0;
			end = quarter_circle;
			break;
		}
	}

	return std::make_tuple( start, end );
}

// draws a circle with line segments

void render::draw_circle( const vec2& origin, float radius, e_corner corner )
{
	render_vertex v0( vec2::zero, vec2( 0, 0 ), render::state->color, render::state->glow );
	render_vertex v1( vec2::zero, vec2( 0, 0 ), render::state->color, render::state->glow );

	{
		scoped_render_state;

		auto [circle_start, circle_end] = get_circle_start_end_indices( corner );

		for( auto x = circle_start; x < circle_end; ++x )
		{
			v0.x = origin.x + ( g_engine->renderer.circle_sample_points[ x ].x * radius );
			v0.y = origin.y + ( g_engine->renderer.circle_sample_points[ x ].y * radius );

			v1.x = origin.x + ( g_engine->renderer.circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
			v1.y = origin.y + ( g_engine->renderer.circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

			render::state->batch_render_target->add_line( g_engine->tex_white, &v0, &v1 );
		}
	}
}

void render::draw_filled_circle( const vec2& origin, float radius, e_corner corner )
{
	render_vertex v0( origin, vec2( 0, 0 ), render::state->color, render::state->glow );
	render_vertex v1( v0 );
	render_vertex v2( v0 );

	{
		scoped_render_state;

		auto [circle_start, circle_end] = get_circle_start_end_indices( corner );

		for( auto x = circle_start; x < circle_end; ++x )
		{
			v1.x = origin.x + ( g_engine->renderer.circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
			v1.y = origin.y + ( g_engine->renderer.circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

			v2.x = origin.x + ( g_engine->renderer.circle_sample_points[ x ].x * radius );
			v2.y = origin.y + ( g_engine->renderer.circle_sample_points[ x ].y * radius );

			render::state->batch_render_target->add_triangle( g_engine->tex_white, &v0, &v1, &v2 );
		}
	}
}

// draws a single line segment

void render::draw_line( const vec2& start, const vec2& end )
{
	render_vertex v0( start, vec2( 0, 0 ), render::state->color, render::state->glow );
	render_vertex v1( end, vec2( 0, 0 ), render::state->color, render::state->glow );

	render::state->batch_render_target->add_line(
		g_engine->tex_white,
		&v0,
		&v1
	);
}

// draws a series of connected lines

void render::draw_lines( const std::vector<vec2>& list_of_verts )
{
	auto sz = list_of_verts.size();
	assert( sz > 2 );	// call draw_line instead

	for( auto x = 0 ; x < sz ; ++x )
	{
		draw_line( list_of_verts[ x ], list_of_verts[ ( x + 1 ) % sz ] );
	}
}

// same as "draw_lines", except the first and last vertices are connected at the end

void render::draw_line_loop( const rect& rc )
{
	auto left = rc.x;
	auto top = rc.y;
	auto right = rc.x + rc.w;
	auto bottom = rc.y + rc.h;

	render::draw_line_loop(
		{
			{ left, top },
			{ right, top },
			{ right, bottom },
			{ left, bottom }
		}
	);
}

void render::draw_line_loop( const std::vector<vec2>& list_of_verts )
{
	auto sz = list_of_verts.size();
	assert( sz > 2 );	// call draw_lines instead

	draw_lines( list_of_verts );
	draw_line( list_of_verts[ 0 ], list_of_verts[ sz - 1 ] );
}

void render::draw_point( const vec2& pos )
{
	render_vertex v0( pos, vec2( 0, 0 ), render::state->color, render::state->glow );

	{
		scoped_render_state;

		render::state->batch_render_target->add_point( g_engine->tex_white, &v0 );
	}
}

void render::draw_sliced( const slice_def_asset* slice_def, const rect& dst )
{
	texture_asset* p_00 = slice_def->patches[ slicedef_patch::top_left ];
	texture_asset* p_10 = slice_def->patches[ slicedef_patch::top_middle ];
	texture_asset* p_20 = slice_def->patches[ slicedef_patch::top_right ];
	texture_asset* p_01 = slice_def->patches[ slicedef_patch::middle_left ];
	texture_asset* p_11 = slice_def->patches[ slicedef_patch::middle_middle ];
	texture_asset* p_21 = slice_def->patches[ slicedef_patch::middle_right ];
	texture_asset* p_02 = slice_def->patches[ slicedef_patch::bottom_left ];
	texture_asset* p_12 = slice_def->patches[ slicedef_patch::bottom_middle ];
	texture_asset* p_22 = slice_def->patches[ slicedef_patch::bottom_right ];

	float xpos = dst.x;
	float ypos = dst.y;

	float inner_w = dst.w - p_00->rc.w - p_20->rc.w;
	float inner_h = dst.h - p_00->rc.h - p_02->rc.h;

	// top row

	draw_quad( p_00, rect( xpos, ypos, p_00->rc.w, p_00->rc.h ) );

	xpos += p_00->rc.w;
	draw_quad( p_10, rect( xpos, ypos, inner_w, p_10->rc.h ) );

	xpos += inner_w;
	draw_quad( p_20, rect( xpos, ypos, p_20->rc.w, p_20->rc.h ) );

	// middle row

	xpos = dst.x;
	ypos += p_00->rc.h;
	draw_quad( p_01, rect( xpos, ypos, p_01->rc.w, inner_h ) );

	xpos += p_01->rc.w;
	draw_quad( p_11, rect( xpos, ypos, inner_w, inner_h ) );

	xpos += inner_w;
	draw_quad( p_21, rect( xpos, ypos, p_21->rc.w, inner_h ) );

	// bottom row

	xpos = dst.x;
	ypos += inner_h;
	draw_quad( p_02, rect( xpos, ypos, p_02->rc.w, p_02->rc.h ) );

	xpos += p_02->rc.w;
	draw_quad( p_12, rect( xpos, ypos, inner_w, p_12->rc.h ) );

	xpos += inner_w;
	draw_quad( p_22, rect( xpos, ypos, p_22->rc.w, p_22->rc.h ) );
}

// call this function to figure out a new value based on the frame interpolation percentage.

float render::calc_interpolated_per_sec_value( float current_value, float step_per_second ) const
{
	return current_value + ( fixed_time_step::per_second( step_per_second ) * g_engine->renderer.frame_interpolate_pct );
}

// samples the "pick" frame buffer at click_pos and returns the pick_id found
// there.

int render::sample_pick_id_at( vec2 viewport_click_pos )
{
	g_engine->frame_buffer->bind();
	glReadBuffer( GL_COLOR_ATTACHMENT0 + 2 );

	// texture is flipped vertically from screen space
	viewport_click_pos.y = viewport_h - viewport_click_pos.y;

	// read single pixel back from texture to see what was at viewport_click_pos
	float pixel[ 4 ];
	glReadPixels( (int)viewport_click_pos.x, (int)viewport_click_pos.y, 1, 1, GL_RGBA, GL_FLOAT, &pixel );

	return (int)( pixel[ 0 ] );
}

}
