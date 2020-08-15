#pragma once

struct GE_API i_speaker
{
	std::vector<i_listener*> listeners;

	void add_listener( i_listener* listener );
	void remove_listener( i_listener* listener );
	void send_event_to_listeners( e_event_id event, void* object );
};
