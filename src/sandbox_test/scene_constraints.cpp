
#include "app_header.h"

using namespace war;

Scene_Constraints::Scene_Constraints()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Constraints::pushed()
{
	Scene::pushed();

	// stars
	{
		auto e = add_entity<Entity>();
		e->tag = H( "stars" );
		e->set_pos( { -150.f, -100.f } );

		{
			auto ec = e->add_component<Emitter_Component>();
			ec->init( "em_stars" );
		}
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::circle, 30.f );
			ec->rs_opt.color = make_color( e_pal::brightest );
		}
	}

	// jumping coins
	{
		auto e = add_entity<Entity>();
		e->set_pos( { 0.f, viewport_hh } );

		{
			auto ec = e->add_component<Emitter_Component>();
			ec->init( "em_coin_fountain" );
		}
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::filled_rect, Rect( -viewport_hw, -2.f, viewport_w, 4.f ) );
			ec->rs_opt.color = make_color( e_pal::brightest, 0.25f );
		}
	}

	// torch emitter attached to mouse cursor
	{
		auto e = add_entity<Entity>();
		e->set_pos( { 50.f, -100.f } );
		e->tag = H( "mouse_torch" );

		{
			auto ec = e->add_component<Emitter_Component>();
			ec->init( "em_torch" );
		}
		{
			auto ec = e->add_component<Emitter_Component>();
			ec->init( "em_torch_embers" );
		}
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::filled_circle, 10.f );
			ec->rs_opt.color = make_color( e_pal::brightest );
			ec->rs_opt.color->a = 0.25f;
		}
	}

	g_engine->window.set_mouse_mode( e_mouse_mode::custom );
}

void Scene_Constraints::draw()
{
	draw_tiled_background();
	Scene::draw();
	Render::draw_world_axis();
}

void Scene_Constraints::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Constraints" );

	// compute where the torch is in UI space and draw a label there

	auto viewport_pos = Coord_System::world_to_viewport_pos( find_entity( H( "mouse_torch" ) )->get_pos() );
	viewport_pos += { 24.f, 0.f };
	auto ui_pos = Coord_System::viewport_to_ui_pos( viewport_pos );

	Render::state->align = e_align::left | e_align::vcenter;
	Render::state->color = make_color( e_pal::brighter );
	Render::draw_string( "My Light In The Dark", ui_pos );
}

bool Scene_Constraints::on_input_motion( const Input_Event* evt )
{
	if( evt->input_id == e_input_id::mouse )
	{
		if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_left ) )
		{
			auto wpos = Coord_System::window_to_world_pos( evt->mouse_pos );
			find_entity( H( "mouse_torch" ) )->set_pos( wpos );
		}

		if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_right ) )
		{
			auto wpos = Coord_System::window_to_world_pos( evt->mouse_pos );
			find_entity( H( "stars" ) )->set_pos( wpos );
		}
	}

	return false;
}
