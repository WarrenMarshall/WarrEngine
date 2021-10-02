
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

Scene_Spatial::Scene_Spatial()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Spatial::spawn_entity( Vec2 pos )
{
	auto e = add_entity<Entity>();
	e->set_pos( pos );
	e->set_scale( Random::getf_range( 0.5f, 0.75f ) );
	e->set_pickable();
	e->rs_opt.color = make_color( e_pal::brighter );
	{
		auto ec = e->add_component<Sprite_Component>();
		ec->init( "tex_skull" );
		ec->rs_opt.color = make_color( e_pal::brightest );
	}
	{
		auto ec = e->add_component<Simple_Collision_Body>();
		if( Random::getb() )
		{
			ec->set_as_circle( 8.f );
		}
		else
		{
			ec->set_as_centered_box( 16.f, 16.f );
		}
		ec->set_collision_flags( coll_flags.skull, coll_flags.skull );
	}
}

void Scene_Spatial::pushed()
{
	Scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::custom );

	Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
	//Rect rc( -200, -200, 400, 400 );
	qt.init( rc );
	qt.max_entities_per_node = 1;
	qt.min_node_area = 64;

	// PLAYER SHAPE
	{
		auto e = add_entity<Entity>();
		e->tag = H( "main_ball" );
		e->set_scale( 1.0f );
		e->flags.include_in_quad_tree = true;

#if 1
		{
			auto ec = e->add_component<Simple_Collision_Body>();

			ec->set_as_circle( 12.f );
			ec->set_collision_flags( coll_flags.player, coll_flags.skull );
		}
#else
		{
			auto ec = e->add_component<Simple_Collision_Body>();

			//ec->set_as_circle( 8.f );
			ec->set_as_centered_box( 16.f, 16.f );
			ec->get_transform()->set_pos( { 32.f, 32.f } );
			ec->set_collision_flags( coll_flags.player, coll_flags.skull );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();

			//ec->set_as_circle( 8.f );
			ec->set_as_centered_box( 16.f, 16.f );
			ec->get_transform()->set_pos( { -32.f, -32.f } );
			ec->set_collision_flags( coll_flags.player, coll_flags.skull );
		}
#endif
		{
			auto ec = e->add_component<Sprite_Component>();
			ec->init( "tex_player" );
		}

		player_shape = e;
	}

	for( auto x = 0 ; x < 0 ; ++x )
	{
		spawn_entity( rc.find_random_pos_within() );
	}
}

void Scene_Spatial::draw()
{
	//draw_tiled_background();
	Scene::draw();
	//Render::draw_world_axis();

	qt.debug_draw();

	// show entities that COULD collide with player shape

	auto potential_entities = qt.get_potential_entity_colliding_set( player_shape );
	//auto potential_entities = qt.get_potential_entity_colliding_set( player_shape->get_pos(), 32.f );
	//auto potential_entities = qt.get_potential_entity_colliding_set( Rect::create_centered( 64.f, 8.f ) + player_shape->get_pos() );

	Render::state->color = make_color( Color::teal, 0.5f );
	for( auto& e : potential_entities )
	{
		Render::draw_line( player_shape->get_transform()->pos, e->get_transform()->pos );
	}

	// debug
	// debug
	// debug
	Render::state->color = Color::light_blue;
	Render::draw_rect( player_shape->simple_collision_ws_aabb );
	// debug
	// debug
	// debug
}

void Scene_Spatial::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Spatial Partitioning" );

	Render::draw_string( std::format( "{} nodes", qt.nodes.size() ), Vec2( 4, 4 ) );
}

bool Scene_Spatial::on_input_pressed( const Input_Event* evt )
{
	// delete entities with right click
	if( evt->input_id == e_input_id::mouse_button_right )
	{
		if( evt->shift_down )
		{
			auto world_pos = Coord_System::window_to_world_pos( evt->mouse_pos );
			spawn_entity( world_pos );

		}
		else
		{
			auto pick_id = Render::sample_pick_id_at( Coord_System::window_to_viewport_pos( evt->mouse_pos ) );
			auto e = find_entity_by_pick_id( pick_id );

			if( e && e != player_shape )
			{
				e->set_life_cycle( e_life_cycle::dying );
			}
		}
	}

	return false;
}

bool Scene_Spatial::on_input_motion( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::mouse:
		{
			if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_left ) )
			{
				if( !evt->shift_down and !evt->control_down )
				{
					auto world_pos = Coord_System::window_to_world_pos( evt->mouse_pos );

					player_shape->set_pos( world_pos );

					return true;
				}
			}
		}
		break;
	}

	return false;
}
