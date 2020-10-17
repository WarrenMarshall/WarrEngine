#pragma once

struct a_texture : i_asset
{
	unsigned int gl_prim_type = GL_TRIANGLES;
	unsigned int id = 0;
	float w = 0.0f;
	float h = 0.0f;

	a_subtexture* subtex = nullptr;

	virtual ~a_texture() override;

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;

	void bind();
	void unbind();

	_NODISCARD virtual a_subtexture* get_subtexture( float offset = 0.0f );
};
