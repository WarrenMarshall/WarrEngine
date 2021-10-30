#include "master_pch.h"

namespace war
{

Vertex_Array_Object::Vertex_Array_Object( Primitive_Batch* batch, e_render_prim render_prim )
	: render_prim( render_prim ), batch( batch )
{
	init( batch, render_prim );
}

void Vertex_Array_Object::init( Primitive_Batch* batch, e_render_prim render_prim )
{
	this->batch = batch;
	this->render_prim = render_prim;

	assert( batch );

	glCreateVertexArrays( 1, &gl_id );
	glBindVertexArray( gl_id );

	switch( render_prim )
	{
		case e_render_prim::quad:
		{
			// this computes to 10,000 x 4 vertices, which is 40,000 vertices max,
			// which is 60,000 indices
			//
			// (4 verts / 6 indices = 1 quad)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.5f;

			vb = std::make_unique<Vertex_Buffer>( this, 4 );
			ib = g_engine->opengl_mgr.ib_quads.get();
			break;
		}

		case e_render_prim::triangle:
		{
			// this computes to 10,000 x 3 vertices, which is 30,000 vertices max,
			// which is 60,000 indices
			//
			// (3 verts / 3 indices = 1 triangle)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.f;

			vb = std::make_unique<Vertex_Buffer>( this, 3 );
			ib = g_engine->opengl_mgr.ib_tris.get();
			break;
		}

		case e_render_prim::line:
		{
			// this computes to 10,000 x 2 vertices, which is 20,000 vertices max,
			// which is 40,000 indices
			//
			// (2 verts / 2 indices = 1 line)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.f;

			vb = std::make_unique<Vertex_Buffer>( this, 2 );
			ib = g_engine->opengl_mgr.ib_lines.get();
			break;
		}

		case e_render_prim::point:
		{
			// this computes to 10,000 x 1 vertices, which is 10,000 vertices max,
			// which is 10,000 indices
			//
			// (1 verts / 1 indices = 1 point)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.f;

			vb = std::make_unique<Vertex_Buffer>( this, 1 );
			ib = g_engine->opengl_mgr.ib_points.get();
			break;
		}

		default:
		{
			assert( false );
		}
	}
}

Vertex_Array_Object::~Vertex_Array_Object()
{
	glDeleteVertexArrays( 1, &gl_id );
}

void Vertex_Array_Object::bind()
{
	glBindVertexArray( gl_id );
	vb->bind();
	ib->bind();
}

void Vertex_Array_Object::unbind()
{
	glBindVertexArray( 0 );
	vb->unbind();
	ib->unbind();
}

void Vertex_Array_Object::update_stats()
{
#ifndef _RELEASE

	if( vb->vertices.empty() )
	{
		return;
	}

	// update stats and clean up

	g_engine->stats.inc( g_engine->stats._draw_calls );

	if( render_prim == e_render_prim::quad )
	{
		g_engine->stats.add( g_engine->stats._quads, vb->vertices.count / 4.f );
	}
	else if( render_prim == e_render_prim::triangle )
	{
		g_engine->stats.add( g_engine->stats._triangles, vb->vertices.count / 3.f );
	}
	else if( render_prim == e_render_prim::line )
	{
		g_engine->stats.add( g_engine->stats._lines, vb->vertices.count / 2.f );
	}
	else if( render_prim == e_render_prim::point )
	{
		g_engine->stats.add( g_engine->stats._points, vb->vertices.count / 1.f );
	}

	// frame debugger
	{
		if( g_engine->render.debug.is_single_frame_logging() )
		{
			const char* prim_type_desc [] = { "quad", "triangle", "line", "point" };
			auto prim_count = vb->vertices.count / ( f32 )vb->verts_per_element;
			log(
				">> draw call >> {} {}{} (v: {}, i: {})",
				f_commas( prim_count ),
				prim_type_desc[ ( i32 )render_prim ],
				( prim_count > 1.f ) ? "s" : "",
				f_commas( ( f32 )( vb->vertices.count ) ),
				f_commas( vb->vertices.count * indices_to_verts_factor )
			);

			for( auto x = 0; x < g_engine->opengl_mgr.max_texture_image_units; ++x )
			{
				if( vb->texture_slots[ x ] )
				{
					log( "  [{}] {}", x, vb->texture_slots[ x ]->get_src_texture()->tag );
				}
			}
			log_div();
		}
	}
#endif
}

void Vertex_Array_Object::upload_vertices_to_gpu()
{
	if( vb->vertices.empty() )
	{
		return;
	}

	bind();
	vb->upload_vertices_to_gpu();
}

void Vertex_Array_Object::flush_and_reset( e_draw_call draw_call )
{
	if( vb->vertices.empty() )
	{
		return;
	}

	upload_vertices_to_gpu();
	update_stats();
	draw( draw_call );
	reset();
}

void Vertex_Array_Object::draw( e_draw_call draw_call )
{
	if( vb->vertices.empty() )
	{
		return;
	}

	// lines and points change their size/thickness dynamically based on the level of viewport zoom

	auto gl_prim_type = get_gl_prim_type();
	if( gl_prim_type == GL_LINES or gl_prim_type == GL_POINTS )
	{
		f32 line_width = 1.f;
		f32 point_sz = 2.f;

		f32 scale = g_engine->scene_mgr.get_scale();
		line_width *= 1.f + ( scale * g_engine->opengl_mgr.using_camera );
		point_sz *= 1.f + ( scale * g_engine->opengl_mgr.using_camera );

		glLineWidth( line_width );
		glPointSize( point_sz );

	}

	// bind texture units
	vb->bind_texture_units();

	bind();

	// only write to the depth buffer for opaque primitives
	glDepthMask( ( draw_call == e_draw_call::opaque ) );

	glDrawElements( get_gl_prim_type(), ( i32 )( vb->vertices.count * indices_to_verts_factor ), GL_UNSIGNED_INT, nullptr );

	unbind();
}

void Vertex_Array_Object::reset()
{
	vb->reset();
}

GLenum Vertex_Array_Object::get_gl_prim_type()
{
	return (
		( ( render_prim == e_render_prim::line ) * GL_LINES )
		+ ( ( render_prim == e_render_prim::point ) * GL_POINTS )
		+ ( ( render_prim == e_render_prim::triangle ) * GL_TRIANGLES )
		+ ( ( render_prim == e_render_prim::quad ) * GL_TRIANGLES )
		);
}

}
