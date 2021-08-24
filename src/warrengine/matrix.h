
namespace war
{

struct matrix final
{
	matrix();
	matrix( glm::mat4 glm_mtx );

	void apply_transform( const transform& transform );
	void apply_transform( const vec2& pos, float angle, float scale );
	void apply_transform( const vec2& pos, float angle, const vec2& scale );

	void identity();
	void translate( const vec2& v );
	void scale( float v );
	void scale( const vec2& v );
	void rotate( float v );
	void invert();

	[[nodiscard]] vec2 transform_vec2( const vec2& v ) const;
	void transform_vec2( vec2* v ) const;

	matrix operator*( matrix& mtx ) const;
	matrix operator*=( matrix& mtx );

	glm::mat4 m;
};

}
