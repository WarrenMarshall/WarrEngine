#pragma once

struct w_game
{
	std::vector<std::unique_ptr<w_entity>> entities;

	w_game() = default;
	virtual ~w_game() = default;

	virtual void init() {}
	virtual void new_game();
	virtual void update();
	virtual void draw_entities();

	template<typename T>
	T* spawn_entity( w_vec2 pos = w_vec2::zero, float angle = 0.0f, float scale = 1.0f )
	{
		entities.emplace_back( std::make_unique<T>() );

		T* new_entity = static_cast<T*>( entities.back().get() );
		new_entity->set_transform( pos, angle, scale );
		new_entity->post_spawn();

		return new_entity;
	}
};
