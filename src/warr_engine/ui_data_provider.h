#pragma once

struct w_imgui_callback
{
	virtual e_imgui_control_state get_state_for_control( w_imgui_control* control );
	virtual void was_left_clicked( w_imgui_control* control );
	virtual void was_right_clicked( w_imgui_control* control );
};
