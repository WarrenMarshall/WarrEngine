
namespace war
{

// ----------------------------------------------------------------------------
// scoped variable helpers

#define scoped_var(type, line_num) type scoped_var_##line_num = {}
#define scoped_decl(type, line_num) scoped_var(type, line_num)

#define scoped_var_arg1(type, arg1, line_num) type scoped_var_##line_num ( arg1 )
#define scoped_decl_arg1(type, arg1, line_num) scoped_var_arg1(type, arg1, line_num)

// ----------------------------------------------------------------------------

struct Scoped_Render_State final
{
	Scoped_Render_State();
	~Scoped_Render_State();
};

#define scoped_render_state	scoped_decl( Scoped_Render_State, __LINE__)

// ----------------------------------------------------------------------------

struct Scoped_OpenGL final
{
	Scoped_OpenGL();
	~Scoped_OpenGL();
};

#define scoped_opengl	scoped_decl( Scoped_OpenGL, __LINE__)

// ----------------------------------------------------------------------------

struct Scoped_OpenGL_Identity final
{
	Scoped_OpenGL_Identity();
	~Scoped_OpenGL_Identity();
};

#define scoped_opengl_identity	scoped_decl( Scoped_OpenGL_Identity, __LINE__)

// ----------------------------------------------------------------------------

struct Scoped_UI_Layout final
{
	Scoped_UI_Layout( Rect rc );
	~Scoped_UI_Layout();
};

#define scoped_ui_layout( layout_rect )	scoped_decl_arg1( Scoped_UI_Layout, layout_rect, __LINE__)

}
