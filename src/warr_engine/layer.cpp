
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

			OPENGL->top()->add_transform( *( entity->get_transform() ) );

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

			OPENGL->top()->add_transform( *entity->get_transform() );

			entity->draw();
		}
	}
}

void w_layer::draw_ui()
{
}

w_entity* w_layer::add_camera()
{
	return add_entity<e_camera>();
}

w_entity* w_layer::get_camera()
{
	return find_entity( H( "main_camera" ) );
}

bool w_layer::is_topmost_layer() const
{
	return ( LAYER == this );
}

w_entity* w_layer::find_entity( hash tag )
{
	// look for an entity with a matching tag
	auto iter = std::find_if(
		entities.begin(),
		entities.end(),
		[&] ( std::unique_ptr<w_entity>& entity )
		{
			return entity->tag == tag;
		}
	);

	if( iter != entities.end() )
	{
		// move any found entity to the front of the entity list for faster
		// lookup next time
		std::iter_swap( entities.begin(), iter );

		return ( *iter ).get();
	}

	return nullptr;
}

void w_layer::new_game()
{
	entities.clear();
}

w_imgui_callback* w_layer::get_imgui_callback()
{
	return &( IMGUI->default_callback );
}

bool w_layer::on_input_motion( const w_input_event* evt )
{
	return false;
}

bool w_layer::on_input_pressed( const w_input_event* evt )
{
	if( LAYER == this )
	{
		auto callback = get_imgui_callback();

		if( callback->on_input_pressed( evt ) )
		{
			return true;
		}

		UI->tag_focus = hash_none;
	}

	return false;
}

bool w_layer::on_input_held( const w_input_event* evt )
{
	if( LAYER == this )
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
	if( LAYER == this )
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
	if( LAYER == this )
	{
		if( get_imgui_callback()->on_input_key( evt ) )
		{
			return true;
		}
	}

	return false;
}
