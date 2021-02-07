
#include "master_pch.h"
#include "master_header.h"

// Draw a closed polygon provided in CCW order.
void w_physics_debug_draw::DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
{
	RENDER_BLOCK
	(
		RS->color = w_color( color.r, color.g, color.b, color.a );

		for( auto v = 0 ; v < vertexCount ; ++v )
		{
			w_vec2 v0 = w_vec2( vertices[ v ] ).from_b2d();
			w_vec2 v1 = w_vec2( vertices[ ( v + 1 ) % vertexCount ] ).from_b2d();

			RENDER->draw_line( v0, v1 );
		}
	)
}

// Draw a solid closed polygon provided in CCW order.
void w_physics_debug_draw::DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
{
	RENDER_BLOCK
	(
		RS->color = w_color( color.r, color.g, color.b, 0.5f );

		assert( vertexCount > 2 );

		w_vec2 v0 = w_vec2( vertices[ 0 ] ).from_b2d();
		w_vec2 v1 = w_vec2( vertices[ 1 ] ).from_b2d();
		w_vec2 v2;

		for( auto v = 2 ; v < vertexCount ; ++v )
		{
			v2 = w_vec2( vertices[ v ] ).from_b2d();
			RENDER->draw_filled_triangle( v0, v1, v2 );
			v1 = v2;
		}
	)
}

// Draw a circle.
void w_physics_debug_draw::DrawCircle( const b2Vec2& center, float radius, const b2Color& color )
{
	w_vec2 position = w_vec2( center ).from_b2d();

	OPENGL
		->push()
		->translate( { position.x, position.y } );

	RENDER_BLOCK
	(
		RS->color = w_color( color.r, color.g, color.b, color.a );
		RENDER->draw_circle( { 0.0f, 0.0f }, from_b2d( radius ) );
	)

	OPENGL->pop();
}

// Draw a solid circle.
void w_physics_debug_draw::DrawSolidCircle( const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color )
{
	w_vec2 position = w_vec2( center ).from_b2d();

	OPENGL
		->push()
		->translate( { position.x, position.y } );

	RENDER_BLOCK
	(
		RS->color = w_color( color.r, color.g, color.b, 0.5f );
		RENDER->draw_filled_circle( { 0.0f, 0.0f }, from_b2d( radius ) );
	)

	OPENGL->pop();
}

// Draw a line segment.
void w_physics_debug_draw::DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
{
	w_vec2 start = w_vec2( p1 ).from_b2d();
	w_vec2 end = w_vec2( p2 ).from_b2d();

	RENDER_BLOCK
	(
		RS->color = w_color( color.r, color.g, color.b, color.a );
		RENDER->draw_line( start, end );
	)
}

// Draw a transform. Choose your own length scale.
// @param xf a transform.
void w_physics_debug_draw::DrawTransform( const b2Transform& xf )
{
	constexpr float debug_line_length = 10.f;

	w_vec2 v = w_vec2( xf.p ).from_b2d();

	w_matrix mtx;
	mtx.rotate( glm::degrees( xf.q.GetAngle() ) );

	w_vec2 x_axis = v + mtx.transform_vec2( w_vec2( debug_line_length, 0.0f ) );
	w_vec2 y_axis = v + mtx.transform_vec2( w_vec2( 0.0f, debug_line_length ) );

	RENDER_BLOCK
	(
		RS->snap_to_pixel = false;
		RS->color = w_color( 192, 0, 0 );
		RENDER->draw_line( v, x_axis );
		RS->color = w_color( 0, 192, 0 );
		RENDER->draw_line( v, y_axis );
	)
}

// Draw a point.
void w_physics_debug_draw::DrawPoint( const b2Vec2& p, float size, const b2Color& color )
{
	w_vec2 v = w_vec2( p ).from_b2d();

	RENDER_BLOCK
	(
		RS->color = w_color( color.r, color.g, color.b, color.a );
		RENDER->draw_point( v );
	)
}
