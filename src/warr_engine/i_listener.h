#pragma once

struct i_listener
{
	virtual ~i_listener() = default;

	virtual void on_listener_event_received( e_event_id event, void* object ) = 0;
};
