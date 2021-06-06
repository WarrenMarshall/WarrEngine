
#include "app_header.h"

using namespace war;

scene_entity_picking::scene_entity_picking()
{
	draws_completely_solid = true;
}

void scene_entity_picking::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	{
		auto e = add_entity<entity>();
		e->make_pickable();
		e->transform_set_pos( { 100.f, 75.f } );
		e->transform_set_scale( 2.f );
		{
			auto ec = e->add_component<primitive_shape_component>();
			ec->add_shape( primitive_shape::filled_rect, rect( -16, -16, 32, 32 ) );
			ec->rs_opt.color = make_color( pal::darker );
		}
	}

	{
		auto e = add_entity<entity>();
		e->tag = H( "player_mario" );
		e->make_pickable();
		e->transform_set_pos( { 64.f, 64.f } );
		e->transform_set_scale( 3.f );
		{
			auto ec = e->add_component<sprite_component>();
			ec->init( "anim_player_run" );
			ec->finalize();
		}
	}

	{
		auto e = add_entity<entity>();
		e->make_pickable();
		e->transform_set( { 150.f, 200.f }, 15.f, 2.f );
		{
			auto ec = e->add_component<primitive_shape_component>();
			ec->add_shape( primitive_shape::filled_rect, rect( -32, -16, 64, 32 ) );
			ec->rs_opt.color = make_color( pal::brighter );
		}
	}
}

void scene_entity_picking::draw()
{
	draw_tiled_background();
	scene::draw();
	render::draw_world_axis();
}

void scene_entity_picking::draw_ui()
{
	scene::draw_ui();
	draw_title( "Entity Picking" );
}

void scene_entity_picking::post_update()
{
	scene::post_update();

	{
		auto e = find_entity( H( "player_mario" ) );
		auto entity_tform = e->get_transform();
		auto cam_transform = get_transform();

		vec2 diff = (entity_tform->pos - (cam_transform->pos * -1.f)) * 0.25f;

		cam_transform->set_pos( cam_transform->pos - diff );
	}

}

bool scene_entity_picking::on_input_pressed( const input_event* evt )
{
	if( evt->input_id == input_id::mouse_button_left )
	{
		vec2 click_pos = coord_system::window_to_viewport_pos( g_engine->input.mouse_window_pos );
		auto pick_id = render::sample_pick_id_at( click_pos );

		deselect_all();
		select_by_pick_id( pick_id );

		for( auto& e : entities )
		{
			e->rs_opt.glow = ( e->is_selected * 1.5f );
		}

		return true;
	}

	return false;
}

bool scene_entity_picking::on_input_motion( const input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		if( g_engine->input.get_button_state( input_id::mouse_button_left ) == button_state::held )
		{
			vec2 delta = coord_system::window_to_world_vec( evt->delta );

			auto selected_entities = get_selected();

			for( auto& e : selected_entities )
			{
				e->transform_delta_pos( delta );
			}

			return true;
		}
	}

	return false;
}
