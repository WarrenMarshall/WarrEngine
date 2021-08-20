
#pragma once

namespace war
{

struct timeline_nodes
{
	std::vector<std::unique_ptr<timeline_nodes_key_frame>> key_frames;
	life_cycle_mgr life_cycle;
	time_ms start = 0, end = 0;
	float duration = 0.f;

	timeline_nodes();

	void clear();
	void init( time_ms duration );
	void update();

	template<typename T, typename ...Args>
	timeline_nodes_key_frame* add_key_frame( Args &&...args )
	{
		key_frames.push_back( std::make_unique<T>( std::forward<Args>( args )... ) );
		return key_frames.back().get();
	}
};

}
