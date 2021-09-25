
#include "app_header.h"

using namespace war;


Scene_Palette_Callback::Scene_Palette_Callback()
{
	palette_data.set_int_value( 0 );
	palette_items.emplace_back( "Default" );
	palette_items.emplace_back( "Ink" );
	palette_items.emplace_back( "Lumpy" );
	palette_items.emplace_back( "Azul 5" );
	palette_items.emplace_back( "Poison" );

	lut_data.set_int_value( 0 );
	lut_items.emplace_back( "Default" );
	lut_items.emplace_back( "Over Exposed" );
	lut_items.emplace_back( "Underwater" );
	lut_items.emplace_back( "Sepia" );
}

UI_Control_Data* Scene_Palette_Callback::get_data( hash tag )
{
	switch( tag )
	{
		case H( "palette_dropdown" ):	return &palette_data;
		case H( "lut_dropdown" ):		return &lut_data;
	}

	return UI_Callback::get_data( tag );
}

size_t Scene_Palette_Callback::get_item_count( hash tag )
{
	switch( tag )
	{
		case H( "palette_dropdown" ):
		{
			return palette_items.size();
		}

		case H( "lut_dropdown" ):
		{
			return lut_items.size();
		}
	}

	return UI_Callback::get_item_count( tag );
}

std::string_view Scene_Palette_Callback::get_item_for_idx( hash tag, int idx )
{
	switch( tag )
	{
		case H( "palette_dropdown" ):
		{
			return palette_items[ idx ];
		}

		case H( "lut_dropdown" ):
		{
			return lut_items[ idx ];
		}
	}

	return UI_Callback::get_item_for_idx( tag, idx );
}

void Scene_Palette_Callback::on_value_changed( hash tag )
{
	switch( tag )
	{
		case H( "palette_dropdown" ):
		{
			switch( palette_data.int_value() )
			{
				case 0: g_engine->render.palette = *( g_engine->find_asset<Palette_Asset>( "pal_default" ) ); break;
				case 1: g_engine->render.palette = *( g_engine->find_asset<Palette_Asset>( "pal_ink" ) ); break;
				case 2: g_engine->render.palette = *( g_engine->find_asset<Palette_Asset>( "pal_lumpy" ) ); break;
				case 3: g_engine->render.palette = *( g_engine->find_asset<Palette_Asset>( "pal_azul5" ) ); break;
				case 4: g_engine->render.palette = *( g_engine->find_asset<Palette_Asset>( "pal_poison" ) ); break;
			}
		}
		break;

		case H( "lut_dropdown" ):
		{
			switch( lut_data.int_value() )
			{
				case 0: g_engine->render.tex_lut = g_engine->find_asset<Texture_Asset>( "tex_lut_default" ); break;
				case 1: g_engine->render.tex_lut = g_engine->find_asset<Texture_Asset>( "tex_lut_over_exposed" ); break;
				case 2: g_engine->render.tex_lut = g_engine->find_asset<Texture_Asset>( "tex_lut_underwater" ); break;
				case 3: g_engine->render.tex_lut = g_engine->find_asset<Texture_Asset>( "tex_lut_sepia" ); break;
			}
		}
		break;
	}

}

// ----------------------------------------------------------------------------

Scene_Palette::Scene_Palette()
{
	ui_callback = &uic;
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Palette::pushed()
{
	Scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::os );
}

void Scene_Palette::draw()
{
	Render::draw_world_axis();
	draw_tiled_background();
	Scene::draw();
}

void Scene_Palette::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Palettes" );

	Rect rc_panel = { 0.f, 0.f, ui_w, ui_h };
	rc_panel.shrink( 32.f );
	g_ui->layout_init( rc_panel );

	{
		scoped_render_state;

		g_ui->label_control()->set_text( "Palettes" )->done();
		g_ui
			->dropdown_control( H( "palette_dropdown" ) )
			->done();

		g_ui->spacer_control()->done();
		g_ui->label_control()->set_text( "Look Up Textures (LUT)" )->done();
		g_ui
			->dropdown_control( H( "lut_dropdown" ) )
			->done();

		g_ui->spacer_control()->done();

		g_ui
			->image_control()
			->set_image( "tex_color_test" )
			->cut_top( g_ui->current_control->image->height() )
			->done();
	}
}
