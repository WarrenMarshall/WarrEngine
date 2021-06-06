
#include "master_pch.h"
#include "master_header.h"

namespace war
{

render_state::render_state()
	: font( g_engine->pixel_font )
{
}

void render_state::set_from_opt( render_state_optional& rso )
{
	align = rso.align.value_or( align );
	angle = rso.angle.value_or( angle );
	color = rso.color.value_or( color );
	glow = rso.glow.value_or( glow );
	pick_id = rso.pick_id.value_or( pick_id );
	scale = rso.scale.value_or( scale );
	uv_tiling = rso.uv_tiling.value_or( uv_tiling );
	font = rso.font.value_or( font );
}

}
