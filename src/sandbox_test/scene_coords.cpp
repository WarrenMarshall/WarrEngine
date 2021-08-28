
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

scene_coords::scene_coords()
{
	flags.blocks_further_drawing = true;
}

void scene_coords::pushed()
{
	scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	{
		auto e = add_entity<Entity>();
		e->tag = H( "crosshair" );
		e->set_pos( { 0.f, 0.f } );
		e->rs_opt.color = make_color( e_pal::brighter );
		{
			auto ec = e->add_component<Sprite_Component>();
			ec->init( "tex_swirl" );
		}
		{
			auto ec = e->add_component<Sprite_Component>();
			ec->init( "tex_crosshair" );
			ec->rs_opt.color = Color::white;
		}
	}
}

void scene_coords::draw()
{
	draw_tiled_background();
	scene::draw();
	Render::draw_world_axis();
}

void scene_coords::draw_ui()
{
	scene::draw_ui();
	draw_title( "Coordinate Systems" );

	{
		scoped_render_state;

		Render::state->color = make_color( Color::black, 0.5f );
		Render::draw_rounded_filled_rect( Rect( 4.f, 30.f, 75.f, 52.f ), 8.f );
		Render::state->color = make_color( Color::black, 1.f );
		Render::draw_rounded_rect( Rect( 4.f, 30.f, 75.f, 52.f ), 8.f );

		Render::state->color = make_color( Color::white, 0.5f );
		Render::state->scale = 0.5f;

		auto window_pos = g_engine->input_mgr.mouse_window_pos;

		Vec2 render_pos( 12.f, 36.f );
		Render::draw_string( std::format(
			"window : {:.0f} / {:.0f}",
			window_pos.x, window_pos.y ),
			render_pos
		);

		render_pos.y += Render::state->font->get_max_height() * Render::state->scale.y;
		auto viewport_pos = Coord_System::window_to_viewport_pos( window_pos );
		Render::draw_string( std::format(
			"viewport : {:.0f} / {:.0f}",
			viewport_pos.x, viewport_pos.y ),
			render_pos
		);

		render_pos.y += Render::state->font->get_max_height() * Render::state->scale.y;
		auto ui_pos = Coord_System::viewport_to_ui_pos( viewport_pos );
		Render::draw_string( std::format(
			"ui : {:.0f} / {:.0f}",
			ui_pos.x, ui_pos.y ),
			render_pos
		);

		render_pos.y += Render::state->font->get_max_height() * Render::state->scale.y;
		auto world_pos = Coord_System::viewport_to_world_pos( viewport_pos );
		Render::draw_string( std::format(
			"world : {:.0f} / {:.0f}",
			world_pos.x, world_pos.y ),
			render_pos
		);

		auto cam_transform = get_transform();

		render_pos.y += (Render::state->font->get_max_height() * Render::state->scale.y) * 2.f;
		Render::draw_string( std::format(
			"scene_pos : {:.0f} / {:.0f}", cam_transform->pos.x, cam_transform->pos.y ),
			render_pos
		);
		render_pos.y += Render::state->font->get_max_height() * Render::state->scale.y;
		Render::draw_string( std::format(
			"scene_rot : {:.1f}", cam_transform->angle ),
			render_pos
		);
		render_pos.y += Render::state->font->get_max_height() * Render::state->scale.y;
		Render::draw_string( std::format(
			"scene_scl : {:.2f}", cam_transform->scale ),
			render_pos
		);
	}

	// label next to the crosshair
	{
		auto e = find_entity( H( "crosshair" ) );

		auto viewport_pos = Coord_System::world_to_viewport_pos( e->get_pos() );
		auto ui_pos = Coord_System::viewport_to_ui_pos( viewport_pos + Vec2( 32.f, 0.f ) );

		scoped_render_state;
		Render::state->align = e_align::vcenter;
		Render::state->color = make_color( Color( 1.f, 0.5f, 0.f ) );
		Render::state->glow = glow_val;
		g_engine->opengl_mgr.set_uniform_float( "u_pixelate_factor", glow_val );

		Render::draw_string( "<< Your crosshair", ui_pos );
	}
}

bool scene_coords::on_input_motion( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::mouse:
		{
			if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_left ) )
			{
				auto e = find_entity( H( "crosshair" ) );

				if( evt->control_down )
				{
					// rotate crosshair
					glow_val += evt->delta.x / 100.f;
					glow_val = glm::clamp<float>( glow_val, 0.f, 100.f );

				}
				else
				{
					// move crosshair to mouse location
					auto viewport_pos = Coord_System::window_to_viewport_pos( evt->mouse_pos );
					auto world_pos = Coord_System::viewport_to_world_pos( viewport_pos );

					e->set_pos( world_pos );
				}

				return true;
			}
		}
		break;

		case e_input_id::mouse_wheel:
		{
			if( evt->control_down )
			{
				// scale crosshair
				auto e = find_entity( H( "crosshair" ) );
				e->add_delta_scale( evt->delta.y * 0.1f );
				return true;
			}
		}
		break;
	}

	return false;
}
