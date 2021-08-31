
namespace war
{

struct Transform
{
	Vec2 pos = Vec2::zero;
	float angle = 0.f;			// rotation (around z axis)
	float scale = 1.f;

	// direct transforms

	Transform* set( Vec2 pos, float angle, float scale );
	Transform* set_pos( Vec2 pos );
	Transform* set_angle( float angle );
	Transform* set_scale( float scale );

	// delta transforms

	Transform* add_pos( Vec2 delta );
	Transform* add_angle( float delta );
	Transform* add_scale( float delta );
	Transform* multiply_scale( float delta );

	// builds a matrix from the current transform values

	Matrix to_matrix() const;
	Matrix to_matrix_vec() const;
};

}
