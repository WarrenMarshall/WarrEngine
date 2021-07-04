#include "master_pch.h"
#include "master_header.h"

namespace war
{

vertex_array_object::vertex_array_object( render_batch* batch, e_render_prim render_prim )
	: render_prim( render_prim ), batch( batch )
{
	init( batch, render_prim );
}

void vertex_array_object::init( render_batch* batch, e_render_prim render_prim )
{
	this->batch = batch;
	this->render_prim = render_prim;

	assert( batch );

	glCreateVertexArrays( 1, &gl_id );
	glBindVertexArray( gl_id );

	switch( render_prim )
	{
		case render_prim::quad:
		{
			// this computes to 10,000 x 4 vertices, which is 40,000 vertices max,
			// which is 60,000 indices
			//
			// (4 verts / 6 indices = 1 quad)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.5f;
			gl_prim_type = GL_TRIANGLES;

			vb = std::make_unique<vertex_buffer>( this, 4 );
			ib = g_engine->render_api.ib_quads.get();
			break;
		}

		case render_prim::triangle:
		{
			// this computes to 10,000 x 3 vertices, which is 30,000 vertices max,
			// which is 60,000 indices
			//
			// (3 verts / 3 indices = 1 triangle)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.f;
			gl_prim_type = GL_TRIANGLES;

			vb = std::make_unique<vertex_buffer>( this, 3 );
			ib = g_engine->render_api.ib_tris.get();
			break;
		}

		case render_prim::line:
		{
			// this computes to 10,000 x 2 vertices, which is 20,000 vertices max,
			// which is 40,000 indices
			//
			// (2 verts / 2 indices = 1 line)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.f;
			gl_prim_type = GL_LINES;

			vb = std::make_unique<vertex_buffer>( this, 2 );
			ib = g_engine->render_api.ib_lines.get();
			break;
		}

		case render_prim::point:
		{
			// this computes to 10,000 x 1 vertices, which is 10,000 vertices max,
			// which is 10,000 indices
			//
			// (1 verts / 1 indices = 1 point)
			//
			// if you want to make the batches larger, the index buffer will need to
			// use a larger data type.

			indices_to_verts_factor = 1.f;
			gl_prim_type = GL_POINTS;

			vb = std::make_unique<vertex_buffer>( this, 1 );
			ib = g_engine->render_api.ib_points.get();
			break;
		}

		default:
		{
			assert( false );
		}
	}
}

vertex_array_object::~vertex_array_object()
{
	glDeleteVertexArrays( 1, &gl_id );
}

void vertex_array_object::bind()
{
	glBindVertexArray( gl_id );
	vb->bind();
	ib->bind();
}

void vertex_array_object::unbind()
{
	glBindVertexArray( 0 );
	vb->unbind();
	ib->unbind();
}

#if 0	// #render_perf
void vertex_array_object::maybe_flush_and_reset()
{
	if( vb->vertices.size() >= render_batch::max_elements_per_draw_call )
	{
		flush_and_reset();
	}

}
#endif

void vertex_array_object::update_stats()
{
#ifndef _FINAL_RELEASE

	if( vb->vertices.empty() )
	{
		return;
	}

	// update stats and clean up
	g_engine->stats.draw_calls++;

	if( render_prim == render_prim::quad )
	{
		g_engine->stats.quads += vb->vertices.size() / 4.f;
	}
	else if( render_prim == render_prim::triangle )
	{
		g_engine->stats.triangles += vb->vertices.size() / 3.f;
	}
	else if( render_prim == render_prim::line )
	{
		g_engine->stats.lines += vb->vertices.size() / 2.f;
	}
	else if( render_prim == render_prim::point )
	{
		g_engine->stats.points += vb->vertices.size() / 1.f;
	}

	// frame debugger
	{
		if( g_engine->renderer.debug.is_single_frame_logging() )
		{
			const char* prim_type_desc [] = { "quad", "triangle", "line", "point" };
			auto prim_count = vb->vertices.size() / (float)vb->verts_per_element;
			log(
				">> draw call >> {} {}{} (v: {}, i: {})",
				f_commas( prim_count ),
				prim_type_desc[ render_prim ],
				( prim_count > 1.f ) ? "s" : "",
				f_commas( (float)( vb->vertices.size() ) ),
				f_commas( vb->vertices.size() * indices_to_verts_factor )
			);

			for( int x = 0 ; x < g_engine->render_api.max_texture_image_units ; ++x )
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

void vertex_array_object::flush_and_reset()
{
	if( !vb->vertices.empty() )
	{
		update_stats();
		flush_and_reset_internal();
	}
}

void vertex_array_object::upload_vertices_to_gpu()
{
	auto index_count = (int)( vb->vertices.size() * indices_to_verts_factor );

	if( index_count )
	{
		bind();
		vb->upload_vertices_to_gpu();
	}
}

void vertex_array_object::flush_and_reset_internal()
{
	auto index_count = (int)( vb->vertices.size() * indices_to_verts_factor );

	if( index_count )
	{
		upload_vertices_to_gpu();
		draw();

		reset();
	}
}

void vertex_array_object::draw()
{
	auto index_count = (int)( vb->vertices.size() * indices_to_verts_factor );
	if( !index_count )
	{
		return;
	}

	// lines and points change their size/thickness dynamically based on the level of viewport zoom

	if( gl_prim_type == GL_LINES or gl_prim_type == GL_POINTS )
	{
		float line_width = 1.0f;
		float point_sz = 2.0f;

		if( g_engine->render_api.using_camera )
		{
			float scale = g_engine->scenes.get_transform()->scale;
			line_width *= scale;
			point_sz *= scale;
		}

		glLineWidth( line_width );
		glPointSize( point_sz );

	}

	// bind texture units
	vb->bind_texture_units();

	bind();

	// draw
	glDrawElements( gl_prim_type, index_count, GL_UNSIGNED_INT, nullptr );

	unbind();
}

void vertex_array_object::reset()
{
	vb->reset();
}

}
