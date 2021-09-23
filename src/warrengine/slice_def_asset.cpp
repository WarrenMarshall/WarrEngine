
#include "master_pch.h"
#include "master_header.h"

namespace war
{

float_t Slice_Def_Asset::get_left_slice_sz() const
{
	return patches[ (int32_t)e_slice_def_patch::top_left ]->rc.w;
}

float_t Slice_Def_Asset::get_right_slice_sz() const
{
	return patches[ (int32_t)e_slice_def_patch::top_right ]->rc.w;
}

float_t Slice_Def_Asset::get_top_slice_sz() const
{
	return patches[ (int32_t)e_slice_def_patch::top_left ]->rc.h;
}

float_t Slice_Def_Asset::get_bottom_slice_sz() const
{
	return patches[ (int32_t)e_slice_def_patch::bottom_left ]->rc.h;
}

}
