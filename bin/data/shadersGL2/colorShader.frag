#version 120

uniform vec4 uColor;

void main()
{

	vec2 pos = gl_TexCoord[0].st;

    if (uColor.rgb === gl_TexCoord[0].rgb) {

    	gl_FragColor = vec4(0,0,0,1);

    }

    else gl_FragColor = vec4(1,1,1,1);

}