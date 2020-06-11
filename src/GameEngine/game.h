#pragma once

struct w_game
{
	std::vector<std::unique_ptr<w_entity>> entities;

	w_game();

	virtual void new_game();
	virtual void update();
	virtual void update_fts();
	virtual void draw_entities();

	template<typename T> T* add_entity()
	{
		entities.emplace_back( std::make_unique<T>() );

		T* new_entity = static_cast<T*>( entities.back().get() );
		return new_entity;
	}
};
