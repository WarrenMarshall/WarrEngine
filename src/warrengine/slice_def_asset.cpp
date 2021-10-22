
#include "master_pch.h"
#include "master_header.h"

namespace war
{

f32 Slice_Def_Asset::get_left_slice_sz() const
{
	return patches[ (i32)e_slice_def_patch::top_left ]->rc.w;
}

f32 Slice_Def_Asset::get_right_slice_sz() const
{
	return patches[ (i32)e_slice_def_patch::top_right ]->rc.w;
}

f32 Slice_Def_Asset::get_top_slice_sz() const
{
	return patches[ (i32)e_slice_def_patch::top_left ]->rc.h;
}

f32 Slice_Def_Asset::get_bottom_slice_sz() const
{
	return patches[ (i32)e_slice_def_patch::bottom_left ]->rc.h;
}

}
