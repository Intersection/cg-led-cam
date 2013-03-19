uniform sampler2D tex;

// Brightness of pixels
uniform float bright;
uniform int spacing;

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

vec4 ledize(vec2 coord, float width, float height) {
    float dx = 1.0;
    float dy = 1.0;
    
    //vec2 uv = vec2(dx * ceil(coord.x/dx), dy * ceil(coord.y/dy));
    vec2 uv = coord;
    
    float mx = uv.x / (width * 2.0);
    float my = uv.y / (height * 2.0);
    float s = ceil(uv.x);
    vec2 scale = vec2(s,my);
    uv = vec2(coord.x/width * ceil(mx), coord.y/height * ceil(my));
    return vec4(scale.x, 0, 0, 1);
    float m = scale.x * scale.y;
    //    vec4 color = texture2D(tex, scale * coord);
    vec4 color = texture2D(tex, uv) * m;
}

void main()
{
    vec2 uv = gl_TexCoord[0].st;

	float s = gl_TexCoord[0].s * ledCount;
	float t = gl_TexCoord[0].t * (ledCount * aspect);
	
    vec2 coord = vec2(s, t);
    vec4 color = pixelize(uv, ledCount) * bright;
//    vec4 color = ledize(coord, ledCount, ledCount * aspect) * bright;


	// Convert the square pixels to "round" "LED" style
    float mvx = abs(sin(coord.s * 3.1415));
    float mvy = abs(sin(coord.t * 3.1415));
    for (int i = 0; i < spacing; i++) {
        mvx *= mvx;
        mvy *= mvy;
    }

    color = color * (mvx * mvy);
    
    gl_FragColor = color;
}