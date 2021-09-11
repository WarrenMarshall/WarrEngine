#include "master_pch.h"
#include "master_header.h"

namespace war
{

Vertex_Array_Object::Vertex_Array_Object( Primitive_Batch* batch, e_render_prim_t render_prim )
	: render_prim( render_prim ), batch( batch )
{
	init( batch, render_prim );
}

void Vertex_Array_Object::init( Primitive_Batch* batch, e_render_prim_t render_prim )
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

			vb = Vertex_Buffer( this, 4 );
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

			vb = Vertex_Buffer( this, 3 );
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

			vb = Vertex_Buffer( this, 2 );
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

			vb = Vertex_Buffer( this, 1 );
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
	vb.bind();
	ib->bind();
}

void Vertex_Array_Object::unbind()
{
	glBindVertexArray( 0 );
	vb.unbind();
	ib->unbind();
}

void Vertex_Array_Object::update_stats()
{
#ifndef _FINAL_RELEASE

	if( vb.vertices.empty() )
	{
		return;
	}

	// update stats and clean up
	g_engine->stats.draw_calls++;

	if( render_prim == e_render_prim::quad )
	{
		g_engine->stats.quads += vb.vertices.num_objects_in_pool() / 4.f;
	}
	else if( render_prim == e_render_prim::triangle )
	{
		g_engine->stats.triangles += vb.vertices.num_objects_in_pool() / 3.f;
	}
	else if( render_prim == e_render_prim::line )
	{
		g_engine->stats.lines += vb.vertices.num_objects_in_pool() / 2.f;
	}
	else if( render_prim == e_render_prim::point )
	{
		g_engine->stats.points += vb.vertices.num_objects_in_pool() / 1.f;
	}

	// frame debugger
	{
		if( g_engine->render.debug.is_single_frame_logging() )
		{
			const char* prim_type_desc [] = { "quad", "triangle", "line", "point" };
			auto prim_count = vb.vertices.num_objects_in_pool() / (float)vb.verts_per_element;
			log(
				">> draw call >> {} {}{} (v: {}, i: {})",
				f_commas( prim_count ),
				prim_type_desc[ render_prim ],
				( prim_count > 1.f ) ? "s" : "",
				f_commas( (float)( vb.vertices.num_objects_in_pool() ) ),
				f_commas( vb.vertices.num_objects_in_pool() * indices_to_verts_factor )
			);

			for( auto x = 0 ; x < g_engine->opengl_mgr.max_texture_image_units ; ++x )
			{
				if( vb.texture_slots[ x ] )
				{
					log( "  [{}] {}", x, vb.texture_slots[ x ]->get_src_texture()->tag );
				}
			}
			log_div();
		}
	}
#endif
}

void Vertex_Array_Object::flush_and_reset( e_draw_call_t draw_call )
{
	if( !vb.vertices.empty() )
	{
		update_stats();
		flush_and_reset_internal( draw_call );
	}
}

void Vertex_Array_Object::upload_vertices_to_gpu()
{
	if( vb.vertices.empty() )
	{
		return;
	}

	bind();
	vb.upload_vertices_to_gpu();
}

void Vertex_Array_Object::flush_and_reset_internal( e_draw_call_t draw_call )
{
	if( vb.vertices.empty() )
	{
		return;
	}

	upload_vertices_to_gpu();
	draw( draw_call );
	reset();
}

void Vertex_Array_Object::draw( e_draw_call_t draw_call )
{
	if( vb.vertices.empty() )
	{
		return;
	}

	// lines and points change their size/thickness dynamically based on the level of viewport zoom

	auto gl_prim_type = get_gl_prim_type();
	if( gl_prim_type == GL_LINES or gl_prim_type == GL_POINTS )
	{
		float_t line_width = 1.f;
		float_t point_sz = 2.f;

		float_t scale = g_engine->scene_mgr.get_scale();
		line_width *= 1.f + ( scale * g_engine->opengl_mgr.using_camera );
		point_sz *= 1.f + ( scale * g_engine->opengl_mgr.using_camera );

		glLineWidth( line_width );
		glPointSize( point_sz );

	}

	// bind texture units
	vb.bind_texture_units();

	bind();

	// only write to the depth buffer for opaque primitives
	glDepthMask( draw_call == e_draw_call::opaque );

	glDrawElements( get_gl_prim_type(), (int32_t)( vb.vertices.num_objects_in_pool() * indices_to_verts_factor ), GL_UNSIGNED_INT, nullptr );

	unbind();
}

void Vertex_Array_Object::reset()
{
	vb.reset();
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
