
#include "master_pch.h"
#include "master_header.h"

namespace war
{
constexpr float alpha_scale = 0.5f;

// Draw a closed polygon provided in CCW order.
void physics_debug_draw::DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
{
	{
		scoped_render_state;

		render::state->color = war::color( color.r, color.g, color.b, color.a * alpha_scale );

		for( auto v = 0 ; v < vertexCount ; ++v )
		{
			vec2 v0 = vec2( vertices[ v ] ).from_box2d();
			vec2 v1 = vec2( vertices[ ( v + 1 ) % vertexCount ] ).from_box2d();

			render::draw_line( v0, v1 );
		}
	}
}

// Draw a solid closed polygon provided in CCW order.
void physics_debug_draw::DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
{
	{
		scoped_render_state;

		render::state->color = war::color( color.r, color.g, color.b, color.a * alpha_scale );

		assert( vertexCount > 2 );

		vec2 v0 = vec2( vertices[ 0 ] ).from_box2d();
		vec2 v1 = vec2( vertices[ 1 ] ).from_box2d();
		vec2 v2;

		for( auto v = 2 ; v < vertexCount ; ++v )
		{
			v2 = vec2( vertices[ v ] ).from_box2d();
			render::draw_filled_triangle( v0, v1, v2 );
			v1 = v2;
		}
	}
}

// Draw a circle.
void physics_debug_draw::DrawCircle( const b2Vec2& center, float radius, const b2Color& color )
{
	vec2 position = vec2( center ).from_box2d();

	{
		scoped_opengl;
		g_engine->render_api.top_matrix->translate( { position.x, position.y } );

		{
			scoped_render_state;

			render::state->color = war::color( color.r, color.g, color.b, color.a * alpha_scale );
			render::draw_circle( { 0.f, 0.f }, from_box2d( radius ) );
		}
	}
}

// Draw a solid circle.
void physics_debug_draw::DrawSolidCircle( const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color )
{
	vec2 position = vec2( center ).from_box2d();

	{
		scoped_opengl;

		g_engine->render_api.top_matrix->translate( { position.x, position.y } );
		{
			scoped_render_state;

			render::state->color = war::color( color.r, color.g, color.b, 0.25f );
			render::draw_filled_circle( { 0.f, 0.f }, from_box2d( radius ) );
		}
	}
}

// Draw a line segment.
void physics_debug_draw::DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
{
	vec2 start = vec2( p1 ).from_box2d();
	vec2 end = vec2( p2 ).from_box2d();

	{
		scoped_render_state;

		render::state->color = war::color( color.r, color.g, color.b, color.a * alpha_scale );
		render::draw_line( start, end );
	}
}

// Draw a transform. Choose your own length scale.
// @param xf a transform.
void physics_debug_draw::DrawTransform( const b2Transform& xf )
{
	constexpr float debug_line_length = 10.f;

	vec2 v = vec2( xf.p ).from_box2d();

	matrix mtx;
	mtx.rotate( glm::degrees( xf.q.GetAngle() ) );

	vec2 x_axis = v + mtx.transform_vec2( vec2( debug_line_length, 0.f ) );
	vec2 y_axis = v + mtx.transform_vec2( vec2( 0.f, debug_line_length ) );

	{
		scoped_render_state;

		render::state->color = color( 192, 0, 0 );

		render::draw_line( v, x_axis );

		render::state->color = color( 0, 192, 0 );
		render::draw_line( v, y_axis );
	}
}

// Draw a point.
void physics_debug_draw::DrawPoint( const b2Vec2& p, float size, const b2Color& color )
{
	vec2 v = vec2( p ).from_box2d();

	{
		scoped_render_state;

		render::state->color = war::color( color.r, color.g, color.b, color.a * alpha_scale );
		render::draw_point( v );
	}
}

}
