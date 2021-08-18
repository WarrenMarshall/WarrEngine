
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
	// default the color to hot pink. this is what you get if this
	// routine can't figure out what to do.
	vec4 color = vec4( 1.0f, 0.0f, 0.5f, 1.0f );

	switch( tex_id )
	{
		case  0:	color = texture(u_textures[ 0], tex_coord);	break;
		case  1:	color = texture(u_textures[ 1], tex_coord);	break;
		case  2:	color = texture(u_textures[ 2], tex_coord);	break;
		case  3:	color = texture(u_textures[ 3], tex_coord);	break;
		case  4:	color = texture(u_textures[ 4], tex_coord);	break;
		case  5:	color = texture(u_textures[ 5], tex_coord);	break;
		case  6:	color = texture(u_textures[ 6], tex_coord);	break;
		case  7:	color = texture(u_textures[ 7], tex_coord);	break;
		case  8:	color = texture(u_textures[ 8], tex_coord);	break;
		case  9:	color = texture(u_textures[ 9], tex_coord);	break;
		case 10:	color = texture(u_textures[10], tex_coord);	break;
		case 11:	color = texture(u_textures[11], tex_coord);	break;
		case 12:	color = texture(u_textures[12], tex_coord);	break;
		case 13:	color = texture(u_textures[13], tex_coord);	break;
		case 14:	color = texture(u_textures[14], tex_coord);	break;
		case 15:	color = texture(u_textures[15], tex_coord);	break;
		case 16:	color = texture(u_textures[16], tex_coord);	break;
		case 17:	color = texture(u_textures[17], tex_coord);	break;
		case 18:	color = texture(u_textures[18], tex_coord);	break;
		case 19:	color = texture(u_textures[19], tex_coord);	break;
		case 20:	color = texture(u_textures[20], tex_coord);	break;
		case 21:	color = texture(u_textures[21], tex_coord);	break;
		case 22:	color = texture(u_textures[22], tex_coord);	break;
		case 23:	color = texture(u_textures[23], tex_coord);	break;
		case 24:	color = texture(u_textures[24], tex_coord);	break;
		case 25:	color = texture(u_textures[25], tex_coord);	break;
		case 26:	color = texture(u_textures[26], tex_coord);	break;
		case 27:	color = texture(u_textures[27], tex_coord);	break;
		case 28:	color = texture(u_textures[28], tex_coord);	break;
		case 29:	color = texture(u_textures[29], tex_coord);	break;
		case 30:	color = texture(u_textures[30], tex_coord);	break;
		case 31:	color = texture(u_textures[31], tex_coord);	break;
	}

	return color;
}
