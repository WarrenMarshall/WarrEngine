
namespace war
{

// ----------------------------------------------------------------------------
// scoped variable helpers

#define scoped_var(type, line_num) type scoped_var_##line_num = {}
#define scoped_decl(type, line_num) scoped_var(type, line_num)

#define scoped_var_arg1(type, arg1, line_num) type scoped_var_##line_num ( arg1 )
#define scoped_decl_arg1(type, arg1, line_num) scoped_var_arg1(type, arg1, line_num)

// ----------------------------------------------------------------------------

struct s_render_state
{
	s_render_state();
	~s_render_state();
};

#define scoped_render_state	scoped_decl( s_render_state, __LINE__)

// ----------------------------------------------------------------------------

struct s_opengl
{
	s_opengl();
	~s_opengl();
};

#define scoped_opengl	scoped_decl( s_opengl, __LINE__)

// ----------------------------------------------------------------------------

struct s_opengl_identity
{
	s_opengl_identity();
	~s_opengl_identity();
};

#define scoped_opengl_identity	scoped_decl( s_opengl_identity, __LINE__)

// ----------------------------------------------------------------------------

struct s_ui_layout
{
	s_ui_layout( rect rc );
	~s_ui_layout();
};

#define scoped_ui_layout( layout_rect )	scoped_decl_arg1( s_ui_layout, layout_rect, __LINE__)

}
