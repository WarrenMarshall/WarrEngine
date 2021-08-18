
#pragma once

namespace war
{

struct timeline_nodes
{
	life_cycle_mgr life_cycle;
	time_ms start, end;
	float duration;

	std::vector<std::unique_ptr<timeline_nodes_key_frame>> key_frames;
	e_timeline_type type = timeline_type::invalid;

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
