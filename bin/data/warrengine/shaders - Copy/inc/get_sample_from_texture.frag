
// NOTE
//
// shader assumes we will never use more than 32 textures in a draw call. if we
// ever do, there are some magic numbers in here that will need to be updated.
//
// running on a device with less than 32 texture units is fine, the engine code
// will adapt to that automatically.

uniform sampler2D u_textures[32];

vec4 get_sample_from_texture( int tex_id, vec2 tex_coord )
{
	switch( tex_id )
	{
		case  0:	return texture(u_textures[ 0], tex_coord);
		case  1:	return texture(u_textures[ 1], tex_coord);
		case  2:	return texture(u_textures[ 2], tex_coord);
		case  3:	return texture(u_textures[ 3], tex_coord);
		case  4:	return texture(u_textures[ 4], tex_coord);
		case  5:	return texture(u_textures[ 5], tex_coord);
		case  6:	return texture(u_textures[ 6], tex_coord);
		case  7:	return texture(u_textures[ 7], tex_coord);
		case  8:	return texture(u_textures[ 8], tex_coord);
		case  9:	return texture(u_textures[ 9], tex_coord);
		case 10:	return texture(u_textures[10], tex_coord);
		case 11:	return texture(u_textures[11], tex_coord);
		case 12:	return texture(u_textures[12], tex_coord);
		case 13:	return texture(u_textures[13], tex_coord);
		case 14:	return texture(u_textures[14], tex_coord);
		case 15:	return texture(u_textures[15], tex_coord);
		case 16:	return texture(u_textures[16], tex_coord);
		case 17:	return texture(u_textures[17], tex_coord);
		case 18:	return texture(u_textures[18], tex_coord);
		case 19:	return texture(u_textures[19], tex_coord);
		case 20:	return texture(u_textures[20], tex_coord);
		case 21:	return texture(u_textures[21], tex_coord);
		case 22:	return texture(u_textures[22], tex_coord);
		case 23:	return texture(u_textures[23], tex_coord);
		case 24:	return texture(u_textures[24], tex_coord);
		case 25:	return texture(u_textures[25], tex_coord);
		case 26:	return texture(u_textures[26], tex_coord);
		case 27:	return texture(u_textures[27], tex_coord);
		case 28:	return texture(u_textures[28], tex_coord);
		case 29:	return texture(u_textures[29], tex_coord);
		case 30:	return texture(u_textures[30], tex_coord);
		case 31:	return texture(u_textures[31], tex_coord);
	}

	// return purple if the tex_id is one we don't recognize
	return vec4( 1.0f, 0.0f, 1.0f, 1.0f );
}
