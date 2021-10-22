
namespace war
{

struct Matrix final
{
	Matrix();
	Matrix( glm::mat4 glm_mtx );

	void apply_transform( const Transform& transform );
	void apply_transform( const Vec2& pos, f32 angle, f32 scale );
	void apply_transform( const Vec2& pos, f32 angle, const Vec2& scale );

	void identity();
	void translate( const Vec2& v );
	void scale( f32 v );
	void scale( const Vec2& v );
	void rotate( f32 v );
	void invert();

	[[nodiscard]] Vec2 transform_vec2( const Vec2& v ) const;
	void transform_vec2( Vec2* v ) const;

	Matrix operator*( Matrix& mtx ) const;
	Matrix operator*=( Matrix& mtx );

	glm::mat4 m;
};

}
