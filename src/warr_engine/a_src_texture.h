#pragma once

struct a_src_texture : i_asset
{
	declare_find_func( a_src_texture )

	unsigned int gl_id = 0;
	float w = 0.0f;
	float h = 0.0f;
	a_texture* texture = nullptr;

	virtual ~a_src_texture() override;

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;

	[[nodiscard]] virtual a_texture* get_subtexture( float offset = 0.0f );
};
