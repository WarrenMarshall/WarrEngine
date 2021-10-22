
namespace war
{

struct Transform final
{
	Vec2 pos = Vec2::zero;
	f32 angle = 0.f;			// rotation (around z axis)
	f32 scale = 1.f;

	// direct transforms

	Transform* set( Vec2 pos, f32 angle, f32 scale );
	Transform* set_pos( Vec2 pos );
	Transform* set_angle( f32 angle );
	Transform* set_scale( f32 scale );

	// delta transforms

	Transform* add_pos( Vec2 delta );
	Transform* add_angle( f32 delta );
	Transform* add_scale( f32 delta );
	Transform* multiply_scale( f32 delta );

	// builds a matrix from the current transform values

	Matrix to_matrix() const;
	Matrix to_matrix_vec() const;
};

}
