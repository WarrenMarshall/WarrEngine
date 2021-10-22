
#include "app_header.h"

using namespace war;

Scene_Entity_Picking::Scene_Entity_Picking()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Entity_Picking::pushed()
{
	Scene::pushed();

	{
		auto e = add_entity<Entity>();
		e->set_pickable();
		e->set_pos( { 100.f, 75.f } );
		e->set_scale( 2.f );
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::filled_rect, Rect( -16, -16, 32, 32 ) );
			ec->rs_opt.color = make_color( e_pal::darker );
		}
	}

	{
		auto e = add_entity<Entity>();
		e->tag = H( "player" );
		e->set_pickable();
		e->set_pos( { 64.f, 64.f } );
		e->set_scale( 3.f );
		{
			auto ec = e->add_component<Sprite_Component>();
			ec->init( "anim_player_run" );
		}
	}

	{
		auto e = add_entity<Entity>();
		e->set_pickable();
		e->set_pos_angle_scale( { 150.f, 200.f }, 15.f, 2.f );
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::filled_rect, Rect( -32, -16, 64, 32 ) );
			ec->rs_opt.color = make_color( e_pal::lighter );
		}
	}
}

void Scene_Entity_Picking::draw()
{
	draw_tiled_background();
	Scene::draw();
	Render::draw_world_axis();
}

void Scene_Entity_Picking::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Entity Picking" );
}

void Scene_Entity_Picking::post_update()
{
	Scene::post_update();

	{
		auto e = find_entity( H( "player" ) );
		auto entity_tform = e->get_transform();
		auto cam_transform = get_transform();

		Vec2 diff = (entity_tform->pos - (cam_transform->pos * -1.f)) * 0.25f;

		cam_transform->set_pos( cam_transform->pos - diff );
	}

}

bool Scene_Entity_Picking::on_input_pressed( const Input_Event* evt )
{
	if( evt->input_id == e_input_id::mouse_button_left )
	{
		Vec2 click_pos = Coord_System::window_to_viewport_pos( g_engine->input_mgr.mouse_window_pos );
		auto pick_id = Render::sample_pick_id_at( click_pos );

		deselect_all();
		select_by_pick_id( pick_id );

		// #selection - this glow stuff is a temp hack. we need a nice way to outline entities or something cool like that.
		for( auto& e : entities )
		{
			e->rs_opt.glow = 0.0f;
		}

		for( auto& e : selected_entities )
		{
			e->rs_opt.glow = 1.5f;
		}

		return true;
	}

	return false;
}

bool Scene_Entity_Picking::on_input_motion( const Input_Event* evt )
{
	if( evt->input_id == e_input_id::mouse )
	{
		if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_left ) )
		{
			Vec2 delta = Coord_System::window_to_world_vec( evt->delta );

			auto selected_entities = get_selected();

			for( auto& e : selected_entities )
			{
				e->add_delta_pos( delta );
			}

			return true;
		}
	}

	return false;
}
