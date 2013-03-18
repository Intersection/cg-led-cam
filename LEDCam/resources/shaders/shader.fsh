uniform sampler2D tex;

// Brightness of pixels
uniform float bright;

// 'LED' resolution - number of LEDs on each axis - this is not corrected for aspect ratio.
uniform float ledCount;

uniform float aspect;

vec4 pixelize(vec2 uv, float count) {
    float dx = 1.0/count;
    float dy = 1.0/count;
    vec2 coord = vec2(dx * ceil(uv.x/dx),
                      dy * ceil(uv.y/dy));
    return texture2D(tex, coord);
}

void main()
{
    vec2 uv = gl_TexCoord[0].st;

    vec2 coord = gl_TexCoord[0].st * ledCount;
    vec4 color = pixelize(uv, ledCount) * bright;


	// Convert the square pixels to "round" "LED" style
    float mvx = abs(sin(coord.s * 3.1415)) * 1.0;
    float mvy = abs(sin(coord.t * 3.1415)) * 1.0;

    if (mvx * mvy < 0.99)
        color = color * (mvx * mvy);

    gl_FragColor = color;
}