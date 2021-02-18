#pragma once

struct a_anim_texture : a_src_texture
{
	declare_find_func( a_anim_texture )

	std::vector<const a_texture*> frames;
	w_tween frame_tween;

	e_tween_type tween_type = tween_type::loop;
	int frames_per_second = 0;

	a_anim_texture() = delete;
	a_anim_texture( const e_tween_type tween_type, const int frames_per_second );
	virtual ~a_anim_texture() override;

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;
	void add_frame( const a_texture* texture );
	void randomize();

	[[nodiscard]] const a_texture* get_texture( const float anim_offset );
};
