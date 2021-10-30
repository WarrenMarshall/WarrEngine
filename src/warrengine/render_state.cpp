
#include "master_pch.h"

namespace war
{

Render_State::Render_State()
	: font( g_engine->pixel_font )
{
}

void Render_State::set_from_opt( Render_State_Optional& rso )
{
	align = rso.align.value_or( align );
	pct = rso.pct.value_or( pct );
	angle = rso.angle.value_or( angle );
	color = rso.color.value_or( color );
	glow = rso.glow.value_or( glow );
	pick_id = rso.pick_id.value_or( pick_id );
	scale = rso.scale.value_or( scale );
	uv_tiling = rso.uv_tiling.value_or( uv_tiling );
	font = rso.font.value_or( font );
}

}
