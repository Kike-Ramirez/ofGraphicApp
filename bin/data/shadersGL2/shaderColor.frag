#version 120

uniform vec4 uColor;

void main()
{

	vec2 pos = gl_TexCoord[0].st;

    if (uColor == gl_TexCoord[0].rgb) {

    	glFragColor = vec4(0,0,0,1);

    }

    else glFragColor = vec4(1,1,1,1);

}