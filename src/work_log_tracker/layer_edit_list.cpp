
#include "app_header.h"

layer_edit_list::layer_edit_list()
{
	draws_completely_solid = true;
}

void layer_edit_list::push()
{
	// warren - draws at 128, wtf?
	auto entity = GAME->spawn_entity<w_entity>( { 64, 0 } );

	ec_emitter* ec;
	ec = entity->add_component<ec_emitter>();
	ec->init( entity, "background_fire_down" );
	ec = entity->add_component<ec_emitter>();
	ec->init( entity, "background_fire_up" );
	ec->set_transform( { 0, 200 }, 0.0f, 1.0f );
}

void layer_edit_list::draw()
{
	RENDER->begin();
	GAME->draw_entities();
	RENDER->end();

	if( !style_button )
	{
		style_button = std::make_unique<w_ui_style_button>();
	}

	float ypos = 6.0f;

	for( auto& iter : GAME->artists )
	{
		RENDER->begin()->push_depth_nudge( 100 );
		if( UI->im_active( iter.name, w_rect( 8, ypos, 130, 16 ), *( style_button.get() ) ) & im_result::left_clicked )
		{
			log_msg( fmt::format( "CLICKED : {}", iter.name ) );
		}
		RENDER->end();

		ypos += 19.0f;
	}
}
