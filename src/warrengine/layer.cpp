
#include "master_pch.h"
#include "master_header.h"

w_layer::w_layer()
{
	draws_completely_solid = false;
	blocks_further_input = false;

	is_debug_physics_layer = false;

	// preallocate some space for entity pointers
	entities.reserve( 150 );
}

// ----------------------------------------------------------------------------

void w_layer::select_by_pick_id( int pick_id )
{
	if( !pick_id )
	{
		return;
	}

	for( const auto& e : entities )
	{
		if( e->pick_id == pick_id )
		{
			e->is_selected = true;
		}
	}
}

void w_layer::deselect_all()
{
	for( const auto& e : entities )
	{
		e->is_selected = false;
	}
}

void w_layer::get_selected( std::vector<w_entity*>& selections )
{
	selections.clear();

	for( const auto& e : entities )
	{
		if( e->is_selected )
		{
			selections.emplace_back( e.get() );
		}
	}
}

// ----------------------------------------------------------------------------

void w_layer::update()
{
	// clear out dead entities
	//
	// NOTE : uses standard 'for' loop because it manipulates the vector as it runs.

	for( size_t x = 0; x < entities.size(); ++x )
	{
		w_entity* e = entities[ x ].get();

		if( e->can_be_deleted() )
		{
			entities.erase( entities.begin() + x );
			x--;
		}
	}

	// update entities and components

	for( auto& entity : entities )
	{
		{
			scoped_opengl;

			engine->opengl->top()->add_transform( *( entity->get_transform() ) );

			entity->update();
			entity->update_components();
			entity->update_from_physics();
		}
	}
}

void w_layer::draw()
{
	for( const auto& entity : entities )
	{
		engine->stats->entities++;

		{
			scoped_opengl;

			engine->opengl->top()->add_transform( *entity->get_transform() );

			entity->draw();
		}
	}
}

void w_layer::draw_ui()
{
}

w_entity* w_layer::add_camera()
{
	auto camera = add_entity<e_camera>();
	camera->get_transform()->set_pos( { viewport_hw, viewport_hh } );
	return camera;
}

w_entity* w_layer::get_camera()
{
	return find_entity( H( "main_camera" ) );
}

bool w_layer::is_topmost_layer() const
{
	return ( engine->layer_mgr->get_top() == this );
}

w_entity* w_layer::find_entity( hash tag )
{
	w_entity* entity = nullptr;

	// look for an entity with a matching tag
	auto iter = std::find_if(
		entities.begin(),
		entities.end(),
		[&] ( std::unique_ptr<w_entity>& entity )
		{
			return entity->tag == tag;
		}
	);

	// #todo - this could benefit from an MRU type cache where the most
	// frequently searched entities and their tags are found quickly

	if( iter != entities.end() )
	{
		entity = ( *iter ).get();
	}

	return entity;
}

void w_layer::new_game()
{
	entities.clear();
}

w_imgui_callback* w_layer::get_imgui_callback()
{
	return &( engine->ui->imgui->default_callback );
}

bool w_layer::on_input_motion( const w_input_event* evt )
{
	return false;
}

bool w_layer::on_input_pressed( const w_input_event* evt )
{
	if( engine->layer_mgr->get_top() == this )
	{
		auto callback = get_imgui_callback();

		if( callback->on_input_pressed( evt ) )
		{
			return true;
		}

		engine->ui->tag_focus = hash_none;
	}

	return false;
}

bool w_layer::on_input_held( const w_input_event* evt )
{
	if( engine->layer_mgr->get_top() == this )
	{
		if( get_imgui_callback()->on_input_held( evt ) )
		{
			return true;
		}
	}

	return false;
}

bool w_layer::on_input_released( const w_input_event* evt )
{
	if( engine->layer_mgr->get_top() == this )
	{
		if( get_imgui_callback()->on_input_released( evt ) )
		{
			return true;
		}
	}

	return false;
}

bool w_layer::on_input_key( const w_input_event* evt )
{
	if( engine->layer_mgr->get_top() == this )
	{
		if( get_imgui_callback()->on_input_key( evt ) )
		{
			return true;
		}
	}

	return false;
}
