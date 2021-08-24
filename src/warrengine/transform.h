
namespace war
{

struct transform final
{
	vec2 pos = vec2::zero;
	float angle = 0.f;			// rotation (around z axis)
	float scale = 1.f;

	// direct transforms

	transform* set( vec2 pos, float angle, float scale );
	transform* set_pos( vec2 pos );
	transform* set_angle( float angle );
	transform* set_scale( float scale );

	// delta transforms

	transform* add_pos( vec2 delta );
	transform* add_angle( float delta );
	transform* add_scale( float delta );
	transform* multiply_scale( float delta );

	// builds a matrix from the current transform values

	matrix to_matrix() const;
	matrix to_matrix_vec() const;
};

}
