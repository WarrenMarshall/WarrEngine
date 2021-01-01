#pragma once

#if 0 // #texture
struct a_anim_texture : a_src_texture
{
	declare_find_func( a_anim_texture )

	std::vector<a_texture*> frames;
	std::unique_ptr<w_tween> frame_tween = nullptr;

	e_tween_type tween_type = tween_type::loop;
	int frames_per_second = 1;

	a_anim_texture() = delete;
	a_anim_texture( e_tween_type tween_type, int frames_per_second );
	virtual ~a_anim_texture() override;

	virtual void clean_up_internals() override;
	virtual bool create_internals() override;
	void add_frame( a_texture* texture );
	void randomize();

	virtual void update() override;
	[[nodiscard]] virtual a_texture* get_texture( float anim_offset ) override;
};
#endif
