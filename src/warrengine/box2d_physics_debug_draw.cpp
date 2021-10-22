
#include "master_pch.h"
#include "master_header.h"

namespace war::box2d_physics
{
constexpr auto alpha_scale = 0.5f;

// Draw a closed polygon provided in CCW order.
void Box2D_Debug_Draw::DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
{
	{
		scoped_render_state;

		Render::state->color = war::Color( color.r, color.g, color.b, color.a * alpha_scale );

		for( auto v = 0 ; v < vertexCount ; ++v )
		{
			Vec2 v0 = Vec2( vertices[ v ] ).from_box2d();
			Vec2 v1 = Vec2( vertices[ ( v + 1 ) % vertexCount ] ).from_box2d();

			Render::draw_line( v0, v1 );
		}
	}
}

// Draw a solid closed polygon provided in CCW order.
void Box2D_Debug_Draw::DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
{
	{
		scoped_render_state;

		Render::state->color = war::Color( color.r, color.g, color.b, color.a * alpha_scale );

		assert( vertexCount > 2 );

		Vec2 v0 = Vec2( vertices[ 0 ] ).from_box2d();
		Vec2 v1 = Vec2( vertices[ 1 ] ).from_box2d();
		Vec2 v2;

		for( auto v = 2 ; v < vertexCount ; ++v )
		{
			v2 = Vec2( vertices[ v ] ).from_box2d();
			Render::draw_filled_triangle( v0, v1, v2 );
			v1 = v2;
		}
	}
}

// Draw a circle.
void Box2D_Debug_Draw::DrawCircle( const b2Vec2& center, f32 radius, const b2Color& color )
{
	Vec2 position = Vec2( center ).from_box2d();

	{
		scoped_opengl;
		g_engine->opengl_mgr.top_matrix->translate( { position.x, position.y } );

		{
			scoped_render_state;

			Render::state->color = war::Color( color.r, color.g, color.b, color.a * alpha_scale );
			Render::draw_circle( { 0.f, 0.f }, from_box2d( radius ) );
		}
	}
}

// Draw a solid circle.
void Box2D_Debug_Draw::DrawSolidCircle( const b2Vec2& center, f32 radius, const b2Vec2& axis, const b2Color& color )
{
	Vec2 position = Vec2( center ).from_box2d();

	{
		scoped_opengl;

		g_engine->opengl_mgr.top_matrix->translate( { position.x, position.y } );
		{
			scoped_render_state;

			Render::state->color = war::Color( color.r, color.g, color.b, 0.25f );
			Render::draw_filled_circle( { 0.f, 0.f }, from_box2d( radius ) );
		}
	}
}

// Draw a line segment.
void Box2D_Debug_Draw::DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
{
	Vec2 start = Vec2( p1 ).from_box2d();
	Vec2 end = Vec2( p2 ).from_box2d();

	{
		scoped_render_state;

		Render::state->color = war::Color( color.r, color.g, color.b, color.a * alpha_scale );
		Render::draw_line( start, end );
	}
}

// Draw a transform. Choose your own length scale.
// @param xf a transform.
void Box2D_Debug_Draw::DrawTransform( const b2Transform& xf )
{
	constexpr f32 debug_line_length = 10.f;

	Vec2 v = Vec2( xf.p ).from_box2d();

	Matrix mtx;
	mtx.rotate( glm::degrees( xf.q.GetAngle() ) );

	Vec2 x_axis = v + mtx.transform_vec2( Vec2( debug_line_length, 0.f ) );
	Vec2 y_axis = v + mtx.transform_vec2( Vec2( 0.f, debug_line_length ) );

	{
		scoped_render_state;

		Render::state->color = Color( 192, 0, 0 );

		Render::draw_line( v, x_axis );

		Render::state->color = Color( 0, 192, 0 );
		Render::draw_line( v, y_axis );
	}
}

// Draw a point.
void Box2D_Debug_Draw::DrawPoint( const b2Vec2& p, f32 size, const b2Color& color )
{
	Vec2 v = Vec2( p ).from_box2d();

	{
		scoped_render_state;

		Render::state->color = war::Color( color.r, color.g, color.b, color.a * alpha_scale );
		Render::draw_point( v );
	}
}

}
