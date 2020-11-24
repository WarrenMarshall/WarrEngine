#pragma once

struct layer_edit_list_uidp : w_imgui_data_provider
{
	virtual int get_subtexture_idx( w_imgui_control* control ) override;
};

#define uptr( arg ) std::unique_ptr<arg>

struct layer_edit_list : w_layer
{
	std::unique_ptr<layer_edit_list_uidp> uidp = nullptr;

	virtual void push() override;
	//virtual void becoming_top_layer() override;
	//virtual void getting_covered() override;
	virtual void draw() override;
};
