#pragma once

struct i_listener
{
	virtual void on_listener_event_received( e_event_id event, void* object ) = 0;
};