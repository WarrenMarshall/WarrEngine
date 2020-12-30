#pragma once

struct a_raw_image_data : i_asset
{
	declare_find_func( a_raw_image_data )

	unsigned int gl_id = 0;
	float w = 0.0f;
	float h = 0.0f;
	a_subtexture* subtex = nullptr;

	virtual ~a_raw_image_data() override;

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;

	[[nodiscard]] virtual a_subtexture* get_subtexture( float offset = 0.0f );
};
