
namespace war
{

struct Transform
{
	Vec2 pos = Vec2::zero;
	float_t angle = 0.f;			// rotation (around z axis)
	float_t scale = 1.f;

	// direct transforms

	Transform* set( Vec2 pos, float_t angle, float_t scale );
	Transform* set_pos( Vec2 pos );
	Transform* set_angle( float_t angle );
	Transform* set_scale( float_t scale );

	// delta transforms

	Transform* add_pos( Vec2 delta );
	Transform* add_angle( float_t delta );
	Transform* add_scale( float_t delta );
	Transform* multiply_scale( float_t delta );

	// builds a matrix from the current transform values

	Matrix to_matrix() const;
	Matrix to_matrix_vec() const;
};

}
