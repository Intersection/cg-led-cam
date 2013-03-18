uniform sampler2D tex;

uniform vec3 mixColor;

void main( void )
{
	vec3 sample;

	vec2 st = gl_TexCoord[0].st;

	sample = texture2D( tex, st ).rgb;

	gl_FragColor = vec4( sample, 1.0 );
}

