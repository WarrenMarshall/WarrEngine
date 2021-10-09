
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Render_State* Render::state = nullptr;

// the current palette being used
Palette_Asset Render::palette = {};

void Render::init()
{
	init_generate_circle_sample_points();

	// renderer batches
	dynamic_batches.init();

	// MODEL MATRIX (getting stuff into worldspace from model space)
	//
	// push an identity matrix onto the stack. this will always live as the
	// first element in the stack and should be the only matrix remaining when
	// the frame finishes rendering.

	g_engine->opengl_mgr.model_matrix_push_identity();

	// initialize render state stacks

	//render_states.reserve( max_render_states );
	clear_render_state_stack();
}

void Render::init_set_up_default_palette()
{
	// create a default palette in case the app doesn't specify one

	Render::palette = {};
	Render::palette.colors.push_back( Color::black );
	Render::palette.colors.push_back( Color::dark_grey );
	Render::palette.colors.push_back( Color::grey );
	Render::palette.colors.push_back( Color::light_grey );
	Render::palette.colors.push_back( Color::white );
}

// generate the sample points for drawing a circle. these verts sit on a
// unit circle and are scaled to match the radius requested for each circle
// we draw.

void Render::init_generate_circle_sample_points()
{
	circle_sample_points.clear();
	circle_sample_points.reserve( circle_sample_points_max );

	float_t angle = 0;
	float_t angle_step = 360.f / (float_t)( circle_sample_points_max );

	for( auto x = 0 ; x < circle_sample_points_max ; ++x )
	{
		circle_sample_points.push_back( Vec2::dir_from_angle( angle ) );
		angle += angle_step;
	}
}

// clear the render state stack and add the default state back in

void Render::clear_render_state_stack()
{
	render_states.clear();

	Render_State rs;
	rs.batch_render_target = &dynamic_batches;
	render_states.push_back( rs );
	Render::state = &render_states.back();
}

void Render::draw_mesh( const Mesh_Asset* mesh )
{
	{
		scoped_opengl;

		for( auto& [texture, triangle_list] : mesh->texture_to_triangles )
		{
			auto tex = texture->get_frame( Render::state->anim_offset );

			for( auto& tri : triangle_list )
			{
				Render_Vertex v0(
					Vec3( tri.verts[ 0 ].x, tri.verts[ 0 ].y, tri.verts[ 0 ].z ),
					Vec2( tri.verts[ 0 ].u * Render::state->uv_tiling.u, tri.verts[ 0 ].v * Render::state->uv_tiling.v ),
					Render::state->color,
					Render::state->glow
				);
				Render_Vertex v1(
					Vec3( tri.verts[ 1 ].x, tri.verts[ 1 ].y, tri.verts[ 1 ].z ),
					Vec2( tri.verts[ 1 ].u * Render::state->uv_tiling.u, tri.verts[ 1 ].v * Render::state->uv_tiling.v ),
					Render::state->color,
					Render::state->glow
				);
				Render_Vertex v2(
					Vec3( tri.verts[ 2 ].x, tri.verts[ 2 ].y, tri.verts[ 2 ].z ),
					Vec2( tri.verts[ 2 ].u * Render::state->uv_tiling.u, tri.verts[ 2 ].v * Render::state->uv_tiling.v ),
					Render::state->color,
					Render::state->glow
				);

				Render::state->batch_render_target->add_triangle( tex, &v0, &v1, &v2 );
			}
		}

	#ifndef _FINAL_RELEASE
		if( g_engine->render.debug.is_drawing_debug_info() )
		{
			// wireframe overlay

			Render::state->color = Color::grey;
			Render::state->z += zdepth_nudge;

			for( auto& [texture, triangle_list] : mesh->texture_to_triangles )
			{
				for( auto& tri : triangle_list )
				{
					Render_Vertex v0( Vec3( tri.verts[ 0 ].x, tri.verts[ 0 ].y, tri.verts[ 0 ].z ), Vec2( 0.f, 0.f ), Render::state->color, Render::state->glow );
					Render_Vertex v1( Vec3( tri.verts[ 1 ].x, tri.verts[ 1 ].y, tri.verts[ 1 ].z ), Vec2( 0.f, 0.f ), Render::state->color, Render::state->glow );
					Render_Vertex v2( Vec3( tri.verts[ 2 ].x, tri.verts[ 2 ].y, tri.verts[ 2 ].z ), Vec2( 0.f, 0.f ), Render::state->color, Render::state->glow );

					Render::state->batch_render_target->add_line( g_engine->tex_white, &v0, &v1 );
					Render::state->batch_render_target->add_line( g_engine->tex_white, &v1, &v2 );
					Render::state->batch_render_target->add_line( g_engine->tex_white, &v2, &v0 );

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

void Render::draw_sprite( const Texture_Asset* texture, const Vec2& dst )
{
	const auto frame = const_cast<Texture_Asset*>( texture )->get_frame( Render::state->anim_offset );

	float_t hw = frame->rc.w / 2.f;
	float_t hh = frame->rc.h / 2.f;

	Render_Vertex v0(
		Vec2( -hw, -hh ),
		Vec2( frame->uv00.u, frame->uv11.v ),
		Render::state->color, Render::state->glow );

	Render_Vertex v1(
		Vec2( hw, -hh ),
		Vec2( frame->uv11.u * Render::state->uv_tiling.u, frame->uv11.v ),
		Render::state->color, Render::state->glow );

	Render_Vertex v2(
		Vec2( hw, hh ),
		Vec2( frame->uv11.u * Render::state->uv_tiling.u, frame->uv00.v * Render::state->uv_tiling.v ),
		Render::state->color, Render::state->glow );

	Render_Vertex v3(
		Vec2( -hw, hh ),
		Vec2( frame->uv00.u, frame->uv00.v * Render::state->uv_tiling.v ),
		Render::state->color, Render::state->glow );

	{
		scoped_opengl;
		g_engine->opengl_mgr.top_matrix->apply_transform( { dst.x, dst.y }, Render::state->angle, Render::state->scale );

		Render::state->batch_render_target->add_quad( frame, &v0, &v1, &v2, &v3 );
	}
}

// draws a textured quad

void Render::draw_quad( const Texture_Asset* texture, const Vec2& dst )
{
	const Texture_Asset* frame = const_cast<Texture_Asset*>( texture )->get_frame( Render::state->anim_offset );

	Render::draw_quad( texture, Rect( dst.x, dst.y, frame->rc.w, frame->rc.h ) );
}

void Render::draw_quad( const Texture_Asset* texture, const Rect& dst )
{
	const auto frame = const_cast<Texture_Asset*>( texture )->get_frame( Render::state->anim_offset );

	float_t w = dst.w ? dst.w : frame->rc.w;
	float_t h = dst.h ? dst.h : frame->rc.h;

	Render_Vertex v0(
		Vec2( 0.f, 0.f ),
		Vec2( frame->uv00.u, frame->uv11.v * Render::state->uv_tiling.v ),
		Render::state->color, Render::state->glow );

	Render_Vertex v1(
		Vec2( w, 0.0 ),
		Vec2( frame->uv11.u * Render::state->uv_tiling.u, frame->uv11.v * Render::state->uv_tiling.v ),
		Render::state->color, Render::state->glow );

	Render_Vertex v2(
		Vec2( w, h ),
		Vec2( frame->uv11.u * Render::state->uv_tiling.u, frame->uv00.v ),
		Render::state->color, Render::state->glow );

	Render_Vertex v3(
		Vec2( 0.f, h ),
		Vec2( frame->uv00.u, frame->uv00.v ),
		Render::state->color, Render::state->glow );

	{
		scoped_opengl;
		g_engine->opengl_mgr.top_matrix->apply_transform( { dst.x, dst.y }, Render::state->angle, Render::state->scale );

		Render::state->batch_render_target->add_quad( frame, &v0, &v1, &v2, &v3 );
	}
}

void Render::draw_tiled( const Texture_Asset* texture, const Rect& dst )
{
	scoped_render_state;

	const Texture_Asset* frame = const_cast<Texture_Asset*>( texture )->get_frame( Render::state->anim_offset );

	Render::state->uv_tiling = Vec2::compute_uv_tiling( frame, dst );

	Render::draw_quad( frame, dst );
}

// draws a string from a bitmap font, char by char, as textured quads
//
// returns: the x,y position immediately following the last character that was
// drawn.

Vec2 Render::draw_string( const std::string& text, const Vec2& pos )
{
	auto extents = Render::state->font->get_string_extents( text );

	Vec2 alignment_pos_adjustment( 0.f, 0.f );

	if( (int32_t)(Render::state->align & e_align::hcenter) )
	{
		alignment_pos_adjustment.x -= ( extents.x * Render::state->scale.x ) / 2.f;
	}

	if( (int32_t)(Render::state->align & e_align::right) )
	{
		alignment_pos_adjustment.x -= extents.x * Render::state->scale.x;
	}

	if( (int32_t)(Render::state->align & e_align::vcenter) )
	{
		alignment_pos_adjustment.y += ( Render::state->font->get_max_height() * Render::state->scale.y ) / 2.f;
	}

	// ----------------------------------------------------------------------------

	Vec2 draw_pos = { pos.x + alignment_pos_adjustment.x, pos.y - alignment_pos_adjustment.y };

	for( const char iter : text )
	{
		Font_Def_Asset::Glyph* fch = &( Render::state->font->font_def->char_map[ (int32_t)( iter ) ] );

		Render::draw_quad(
			&fch->glyph_texture,
			Rect(
				draw_pos.x + ( fch->xoffset * Render::state->scale.x ),
				draw_pos.y + ( fch->yoffset * Render::state->scale.y )
			)
		);

		draw_pos.x += ( fch->xadvance * Render::state->scale.x );
	}

	return draw_pos;
}

// "rc" can be a proper rectangular region or it can just be an x,y coordinate
// with no w,h specified. the w,h are only used for certain alignments and
// knowing when to stop drawing strings inside of a box. if w.h are zero,
// those options just don't do anything.

Vec2 Render::draw_string( const std::vector<std::string>& text, const Rect& rc )
{
	Vec2 wk_pos = { rc.x, rc.y };

	if( (int32_t)( Render::state->align & e_align::right) )
	{
		wk_pos.x = rc.x + rc.w;
	}
	if( (int32_t)( Render::state->align & e_align::hcenter) )
	{
		wk_pos.x = rc.x + ( rc.w / 2.f );
	}
	if( (int32_t)( Render::state->align & e_align::vcenter) )
	{
		wk_pos.y = rc.y - ( ( Render::state->font->get_max_height() * text.size() ) / 2.f );
	}

	Vec2 draw_pos;

	for( auto& iter : text )
	{
		draw_pos = Render::draw_string( iter, wk_pos );
		wk_pos.y += Render::state->font->get_max_height();

		if( rc.h > 0.f and wk_pos.y >= rc.y + rc.h )
		{
			break;
		}
	}

	return draw_pos;
}

std::vector<std::string> Render::wrap_string_to_width( std::string_view text, float_t width )
{
	std::vector<std::string> wrapped_lines;
	auto space_char_extents = Render::state->font->get_string_extents( " " );

	Tokenizer tok;
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

	float_t line_width = 0.f;
	std::string wk_line;

	for( auto& iter : new_tokens )
	{
		bool is_newline = ( iter == "\n" );

		auto word_extents = Render::state->font->get_string_extents( iter );

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

void Render::begin_frame()
{
	// reset glviewport
	glViewport( 0, 0, (int32_t)viewport_w, (int32_t)viewport_h );

	// clear color attachments
	g_engine->frame_buffer.clear_color_attachments();
	g_engine->blur_frame_buffer.clear_color_attachments();
	g_engine->composite_frame_buffer.clear_color_attachments();
}

// called at end of each frame to finalize and render all buffers

void Render::end_frame()
{
	// stats - use the UI view matrix since stats are drawn as if they are UI

	g_engine->opengl_mgr.set_view_matrix_identity_ui();

	g_engine->stats.frame_times_ms += g_engine->clock.delta_ms;
	g_engine->stats.frame_count++;

	g_engine->stats.draw();

	// flush the final batches

	Render::flush_buffers();

	// there should be a single model matrix left on the stack (the identity
	// matrix we created at renderer start up). If there is any other number,
	// then there is an uneven push/pop combo somewhere in the code.

	assert( g_engine->opengl_mgr.model_matrix_stack.size() == 1 );

	clear_render_state_stack();

#ifndef _FINAL_RELEASE
	g_engine->render.debug.single_frame_log = false;
	g_engine->render.debug.entity_info_log = false;
#endif
}

// draws the X and Y axis in the viewport at the world origin
//
// positive directions are brighter than the negative ones

void Render::draw_world_axis()
{
	{
		scoped_render_state;

		Render::state->z = zdepth_topmost;

		Render::state->color = make_color( Color::red, 0.5f );
		Render::draw_line( Vec2::zero, Vec2( 5000, 0 ) );

		Render::state->color = make_color( Color::dark_red, 0.5f );
		Render::draw_line( Vec2::zero, Vec2( -5000, 0 ) );

		Render::state->color = make_color( Color::green, 0.5f );
		Render::draw_line( Vec2::zero, Vec2( 0, 5000 ) );

		Render::state->color = make_color( Color::dark_green, 0.5f );
		Render::draw_line( Vec2::zero, Vec2( 0, -5000 ) );
	}
}

// draws an empty rectangle

void Render::draw_rect( const Rect& dst )
{
	Render::draw_line_loop( dst );
}

void Render::draw_rounded_rect( const Rect& dst, float_t radius )
{
	// make sure there is enough room for the rounded corners
	assert( dst.w >= radius * 2.f or dst.h >= radius * 2.f );

	Render::draw_circle( { dst.x + radius, dst.y + radius }, radius, e_corner::top_left );
	Render::draw_circle( { dst.x + dst.w - radius, dst.y + radius }, radius, e_corner::top_right );
	Render::draw_circle( { dst.x + dst.w - radius, dst.y + dst.h - radius }, radius, e_corner::bottom_right );
	Render::draw_circle( { dst.x + radius, dst.y + dst.h - radius }, radius, e_corner::bottom_left );

	Render::draw_line( { dst.x + radius, dst.y }, { dst.x + dst.w - radius, dst.y } );
	Render::draw_line( { dst.x + dst.w, dst.y + radius }, { dst.x + dst.w, dst.y + dst.h - radius } );
	Render::draw_line( { dst.x + dst.w - radius, dst.y + dst.h }, { dst.x + radius, dst.y + dst.h } );
	Render::draw_line( { dst.x, dst.y + dst.h - radius }, { dst.x, dst.y + radius } );
}

void Render::draw_rounded_filled_rect( const Rect& dst, float_t radius )
{
	// make sure there is enough room for the rounded corners
	assert( dst.w >= radius * 2.f or dst.h >= radius * 2.f );

	Render::draw_filled_circle( { dst.x + radius, dst.y + radius }, radius, e_corner::top_left );
	Render::draw_filled_circle( { dst.x + dst.w - radius, dst.y + radius }, radius, e_corner::top_right );
	Render::draw_filled_circle( { dst.x + dst.w - radius, dst.y + dst.h - radius }, radius, e_corner::bottom_right );
	Render::draw_filled_circle( { dst.x + radius, dst.y + dst.h - radius }, radius, e_corner::bottom_left );

	float_t dradius = radius * 2.f;
	Render::draw_filled_rect( { dst.x + radius, dst.y, dst.w - dradius, radius } );
	Render::draw_filled_rect( { dst.x, dst.y + radius, dst.w, dst.h - dradius } );
	Render::draw_filled_rect( { dst.x + radius, dst.y + dst.h - radius, dst.w - dradius, radius } );
}

void Render::draw_filled_rect( const Rect& dst )
{
	Render_Vertex v0(
		Vec2( dst.x, dst.y ),
		Vec2( 0, 0 ),
		Render::state->color,
		Render::state->glow
	);
	Render_Vertex v1(
		Vec2( dst.x, dst.y + dst.h ),
		Vec2( 0, 1 ),
		Render::state->color,
		Render::state->glow
	);
	Render_Vertex v2(
		Vec2( dst.x + dst.w, dst.y + dst.h ),
		Vec2( 1, 1 ),
		Render::state->color,
		Render::state->glow
	);
	Render_Vertex v3(
		Vec2( dst.x + dst.w, dst.y ),
		Vec2( 1, 0 ),
		Render::state->color,
		Render::state->glow
	);

	Render::state->batch_render_target->add_quad( g_engine->tex_white, &v0, &v1, &v2, &v3 );
}

void Render::draw_triangle( const Vec2& v0, const Vec2& v1, const Vec2& v2 )
{
	Render::draw_line_loop( { v0, v1, v2 } );
}

void Render::draw_tile_map( const Tile_Set_Asset* tile_set, const Tile_Map_Asset* tile_map, const Vec2& pos )
{
	for( auto& layer : tile_map->layers )
	{
		if( !layer.flags.is_visible or !layer.flags.should_draw )
		{
			continue;
		}

		for( auto& chunk : layer.chunks )
		{
			for( auto y = 0 ; y < chunk.tilemap_bounds.h ; ++y )
			{
				for( auto x = 0 ; x < chunk.tilemap_bounds.w ; ++x )
				{
					auto tile = &( chunk.tiles[ ( y * (int32_t)chunk.tilemap_bounds.w ) + x ] );

					if( tile->idx == Tile_Map_Asset::Tile::empty )
					{
						continue;
					}

					bool flip_h = (int32_t)(tile->flags & e_tile_flags::flipped_horizontally);
					bool flip_v = (int32_t)(tile->flags & e_tile_flags::flipped_vertically);
					bool flip_d = (int32_t)(tile->flags & e_tile_flags::flipped_diagonally);

					Render::state->scale = Vec2( 1.f, 1.f );
					Render::state->angle = 0.f;

					if( flip_d )
					{
						if( flip_h and !flip_v )
						{
							Render::state->angle = 90.f;
						}
						else if( !flip_h and !flip_v )
						{
							Render::state->angle = 180.f;
						}
						else if( !flip_h and flip_v )
						{
							Render::state->angle = 270.f;
						}
					}
					else
					{
						if( flip_h and flip_v )
						{
							Render::state->angle = 180.f;
						}
						else if( flip_h and !flip_v )
						{
							Render::state->scale.x = -1.f;
						}
						else if( !flip_h and flip_v )
						{
							Render::state->scale.y = -1.f;
						}
					}

					Vec2 tile_pos =
					{
						pos.x + ( ( x + chunk.tilemap_bounds.x ) * tile_map->tile_sz ) + ( tile_map->tile_sz / 2.f ),
						pos.y + ( ( y + chunk.tilemap_bounds.y ) * tile_map->tile_sz ) + ( tile_map->tile_sz / 2.f )
					};

					const Texture_Asset* tile_set_texture = &tile_set->tile_definitions[ tile->idx ].texture;
					Render::draw_sprite( tile_set_texture, tile_pos );
				}
			}
		}
	}
}

// utility function for drawing a fun marker. not meant for games, really.

void Render::draw_crosshair( Vec2 pos )
{
	scoped_render_state;

	const float_t sz = 7.f;
	const float_t hsz = sz / 2.f;
	const float_t spoke_sz = sz / 3.f;

	auto l_draw_spoke = [&] ( Vec2 offset )
	{
		Render::draw_line( pos + offset, pos + ( offset * spoke_sz ) );
	};

	Render::state->color = make_color( Color::grey, 0.25f );
	l_draw_spoke( { hsz, 0.f } );
	l_draw_spoke( { -hsz, 0.f } );
	l_draw_spoke( { 0.f, hsz } );
	l_draw_spoke( { 0.f, -hsz } );

}

void Render::draw_filled_triangle( const Vec2& v0, const Vec2& v1, const Vec2& v2 )
{
	Render_Vertex rv0( v0 );
	Render_Vertex rv1( v1 );
	Render_Vertex rv2( v2 );

	Render::state->batch_render_target->add_triangle( g_engine->tex_white, &rv0, &rv1, &rv2 );
}

auto Render::get_circle_start_end_indices( e_corner corner )
{
	int32_t start, end;
	int32_t quarter_circle = circle_sample_points_max / 4;

	// default to drawing the full circle
	start = 0;
	end = circle_sample_points_max;

	switch( corner )
	{
		case e_corner::top_left:
		{
			start = quarter_circle * 3;
			end = quarter_circle * 4;
			break;
		}

		case e_corner::bottom_left:
		{
			start = quarter_circle * 2;
			end = quarter_circle * 3;
			break;
		}

		case e_corner::bottom_right:
		{
			start = quarter_circle;
			end = quarter_circle * 2;
			break;
		}

		case e_corner::top_right:
		{
			start = 0;
			end = quarter_circle;
			break;
		}
	}

	return std::make_tuple( start, end );
}

// draws a circle with line segments

void Render::draw_circle( const Vec2& origin, float_t radius, e_corner corner )
{
	Render_Vertex v0( Vec2::zero, Vec2( 0, 0 ), Render::state->color, Render::state->glow );
	Render_Vertex v1( Vec2::zero, Vec2( 0, 0 ), Render::state->color, Render::state->glow );

	{
		scoped_render_state;

		auto [circle_start, circle_end] = get_circle_start_end_indices( corner );

		for( auto x = circle_start; x < circle_end; ++x )
		{
			v0.x = origin.x + ( g_engine->render.circle_sample_points[ x ].x * radius );
			v0.y = origin.y + ( g_engine->render.circle_sample_points[ x ].y * radius );

			v1.x = origin.x + ( g_engine->render.circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
			v1.y = origin.y + ( g_engine->render.circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

			Render::state->batch_render_target->add_line( g_engine->tex_white, &v0, &v1 );
		}
	}
}

void Render::draw_filled_circle( const Vec2& origin, float_t radius, e_corner corner )
{
	Render_Vertex v0( origin, Vec2( 0, 0 ), Render::state->color, Render::state->glow );
	Render_Vertex v1( v0 );
	Render_Vertex v2( v0 );

	{
		scoped_render_state;

		auto [circle_start, circle_end] = get_circle_start_end_indices( corner );

		for( auto x = circle_start; x < circle_end; ++x )
		{
			v1.x = origin.x + ( g_engine->render.circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].x * radius );
			v1.y = origin.y + ( g_engine->render.circle_sample_points[ ( x + 1 ) % circle_sample_points_max ].y * radius );

			v2.x = origin.x + ( g_engine->render.circle_sample_points[ x ].x * radius );
			v2.y = origin.y + ( g_engine->render.circle_sample_points[ x ].y * radius );

			Render::state->batch_render_target->add_triangle( g_engine->tex_white, &v0, &v1, &v2 );
		}
	}
}

// draws a single line segment

void Render::draw_line( const Vec2& start, const Vec2& end )
{
	Render_Vertex v0( start, Vec2( 0, 0 ), Render::state->color, Render::state->glow );
	Render_Vertex v1( end, Vec2( 0, 0 ), Render::state->color, Render::state->glow );

	Render::state->batch_render_target->add_line(
		g_engine->tex_white,
		&v0,
		&v1
	);
}

// draws a series of connected lines

void Render::draw_lines( const std::vector<Vec2>& list_of_verts )
{
	auto sz = list_of_verts.size();
	assert( sz > 2 );	// call draw_line instead

	for( auto x = 0 ; x < sz ; ++x )
	{
		draw_line( list_of_verts[ x ], list_of_verts[ ( x + 1 ) % sz ] );
	}
}

// same as "draw_lines", except the first and last vertices are connected at the end

void Render::draw_line_loop( const Rect& rc )
{
	auto left = rc.x;
	auto top = rc.y;
	auto right = rc.x + rc.w;
	auto bottom = rc.y + rc.h;

	Render::draw_line_loop(
		{
			{ left, top },
		{ right, top },
		{ right, bottom },
		{ left, bottom }
		}
	);
}

void Render::draw_line_loop( const std::vector<Vec2>& list_of_verts )
{
	auto sz = list_of_verts.size();
	assert( sz > 2 );	// call draw_lines instead

	draw_lines( list_of_verts );
	draw_line( list_of_verts[ 0 ], list_of_verts[ sz - 1 ] );
}

void Render::draw_point( const Vec2& pos )
{
	Render_Vertex v0( pos, Vec2( 0, 0 ), Render::state->color, Render::state->glow );

	{
		scoped_render_state;

		Render::state->batch_render_target->add_point( g_engine->tex_white, &v0 );
	}
}

void Render::draw_sliced( const Slice_Def_Asset* slice_def, const Rect& dst )
{
	Texture_Asset* p_00 = slice_def->patches[ (int32_t)e_slice_def_patch::top_left ];
	Texture_Asset* p_10 = slice_def->patches[ (int32_t)e_slice_def_patch::top_middle ];
	Texture_Asset* p_20 = slice_def->patches[ (int32_t)e_slice_def_patch::top_right ];
	Texture_Asset* p_01 = slice_def->patches[ (int32_t)e_slice_def_patch::middle_left ];
	Texture_Asset* p_11 = slice_def->patches[ (int32_t)e_slice_def_patch::middle_middle ];
	Texture_Asset* p_21 = slice_def->patches[ (int32_t)e_slice_def_patch::middle_right ];
	Texture_Asset* p_02 = slice_def->patches[ (int32_t)e_slice_def_patch::bottom_left ];
	Texture_Asset* p_12 = slice_def->patches[ (int32_t)e_slice_def_patch::bottom_middle ];
	Texture_Asset* p_22 = slice_def->patches[ (int32_t)e_slice_def_patch::bottom_right ];

	float_t xpos = dst.x;
	float_t ypos = dst.y;

	float_t inner_w = dst.w - p_00->rc.w - p_20->rc.w;
	float_t inner_h = dst.h - p_00->rc.h - p_02->rc.h;

	// top row

	draw_quad( p_00, Rect( xpos, ypos, p_00->rc.w, p_00->rc.h ) );

	xpos += p_00->rc.w;
	draw_quad( p_10, Rect( xpos, ypos, inner_w, p_10->rc.h ) );

	xpos += inner_w;
	draw_quad( p_20, Rect( xpos, ypos, p_20->rc.w, p_20->rc.h ) );

	// middle row

	xpos = dst.x;
	ypos += p_00->rc.h;
	draw_quad( p_01, Rect( xpos, ypos, p_01->rc.w, inner_h ) );

	xpos += p_01->rc.w;
	draw_quad( p_11, Rect( xpos, ypos, inner_w, inner_h ) );

	xpos += inner_w;
	draw_quad( p_21, Rect( xpos, ypos, p_21->rc.w, inner_h ) );

	// bottom row

	xpos = dst.x;
	ypos += inner_h;
	draw_quad( p_02, Rect( xpos, ypos, p_02->rc.w, p_02->rc.h ) );

	xpos += p_02->rc.w;
	draw_quad( p_12, Rect( xpos, ypos, inner_w, p_12->rc.h ) );

	xpos += inner_w;
	draw_quad( p_22, Rect( xpos, ypos, p_22->rc.w, p_22->rc.h ) );
}

// call this function to figure out a new value based on the frame interpolation percentage.

float_t Render::calc_interpolated_per_sec_value( float_t current_value, float_t step_per_second ) const
{
	return current_value + ( fixed_time_step::per_second( step_per_second ) * g_engine->render.frame_interpolate_pct );
}

// samples the "pick" frame buffer at click_pos and returns the pick_id found
// there.

int32_t Render::sample_pick_id_at( Vec2 viewport_click_pos )
{
	g_engine->frame_buffer.bind();
	glReadBuffer( GL_COLOR_ATTACHMENT0 + 2 );

	// texture is flipped vertically from screen space
	viewport_click_pos.y = viewport_h - viewport_click_pos.y;

	// read single pixel back from texture to see what was at viewport_click_pos
	float_t pixel[ 4 ];
	glReadPixels( (int32_t)viewport_click_pos.x, (int32_t)viewport_click_pos.y, 1, 1, GL_RGBA, GL_FLOAT, &pixel );

	return (int32_t)( pixel[ 0 ] );
}

void Render::flush_buffers()
{
	g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::opaque );
	g_engine->render.dynamic_batches.flush_and_reset_internal( e_draw_call::transparent );
}

}
