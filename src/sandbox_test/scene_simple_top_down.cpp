
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

constexpr auto player_collision_radius = 8.f;

// ----------------------------------------------------------------------------

constexpr float max_raycast_length = 350.f;
constexpr auto radius = 12.f;

// ----------------------------------------------------------------------------

Scene_Simple_Top_Down::Scene_Simple_Top_Down()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

f_decl_tile_map_spawn_entity( topdown_spawn_entity )
{
	auto gameplay_scene = (Scene_Simple_Top_Down*)scene;

	switch( tile->idx )
	{
		case 4:
		{
			auto tmc = gameplay_scene->world->get_component<Tile_Map_Component>();

			auto e = scene->add_entity<Entity>();
			e->set_pos( Vec2( tile->x_idx * tmc->tile_map->tile_sz, tile->y_idx * tmc->tile_map->tile_sz ) );
			e->add_delta_pos( Vec2( tmc->tile_map->tile_sz / 2.f, tmc->tile_map->tile_sz / 2.f ) );
			e->simple.friction = 0.5f;

			{
				auto ec = e->add_component<Simple_Collision_Body>();
				ec->tag = H( "player_body" );
				ec->set_as_circle( player_collision_radius );

				ec->set_collision_flags( gameplay_scene->coll_flags.player, gameplay_scene->coll_flags.geo );
			}
			{
				auto ec = e->add_component<Sprite_Component>();
				ec->init( "top_down_walk" );
				ec->get_transform()->add_pos( { 0.f, -4.f } );
			}

			gameplay_scene->player = e;

			tile->idx = Tile_Map_Asset::Tile::empty;
		}
		break;
	}
}

void Scene_Simple_Top_Down::pushed()
{
	get_transform()->set_scale( 2.f );

	Scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	// HIT MARKER

	{
		auto e = add_entity<Entity>();
		e->tag = H( "hit_marker" );
		e->set_pos( { 0.f, 0.f } );
		e->rs_opt.z_bias = zdepth_debug_bias;
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->rs_opt.color = make_color( Color::yellow );
		}

		hit_marker = e;

	}

	// GEO

	{
		world = add_entity<Entity>( "world" );
		world->simple.type = e_sc_type::stationary;

		{
			auto ec = world->add_component<Tile_Map_Component>();
			ec->set_collision_flags( coll_flags.geo, 0 );
			ec->init( "ts_top_down", "tm_top_down" );
			ec->spawn_entities( this, topdown_spawn_entity );
		}
	}
}

void Scene_Simple_Top_Down::draw()
{
	Scene::draw();

	scoped_render_state;

	Render::state->z -= 10.f;
	Render::state->color = make_color( Color::red, 0.15f );

	auto start = player->get_pos();
	auto dir = Vec2::dir_from_angle( player->get_angle() );
	auto end = start + ( dir * ray_cast_length_hit );
	Render::draw_line( start, end );
}

void Scene_Simple_Top_Down::update()
{
	Scene::update();
	follow_cam( player->get_transform() );

	reset_collision_trace_results();

	auto start = player->get_pos();
	auto dir = Vec2::dir_from_angle( player->get_angle() );
	auto end = start + ( dir * 1024.f );

	simple_collision::Raycast_Closest callback;
	sc_world->ray_cast( &callback, player, start, end );

	if( callback.hit_something )
	{
		ray_cast_length_hit = ( callback.result.pos - player->get_pos() ).get_size();
	}

}

void Scene_Simple_Top_Down::reset_collision_trace_results()
{
	hit_marker->get_component<Primitive_Shape_Component>()->shapes.clear();

	for( auto& iter : world->get_components<Simple_Collision_Body>() )
	{
		iter->rs_opt.color = make_color( Color::dark_teal );
	}
}

bool Scene_Simple_Top_Down::on_input_pressed( const Input_Event* evt )
{
	// #topdown - add a keypress toggle to switch into a mode where the player
	// rotates based on the right_stick direction but the world stays upright
	return false;
}

bool Scene_Simple_Top_Down::on_input_motion( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::gamepad_left_stick:
		{
			// take the input delta and apply the scene matrix to it. this
			// allows proper walking angles regardless of scene rotation.

			Vec2 dir = evt->delta;
			auto mtx = player->get_transform()->to_matrix_vec();
			dir = mtx.transform_vec2( dir );

			player->add_force( { dir, 50.5f } );
			return true;
		}
		break;

		case e_input_id::gamepad_right_stick:
		{
			// rotate the player and the scene in opposite directions, which
			// leaves the player facing north

			get_transform()->add_angle( -evt->delta.x * 1.5f );
			player->add_delta_angle( evt->delta.x * 1.5f );

			return true;
		}
		break;
	}

	return false;
}

