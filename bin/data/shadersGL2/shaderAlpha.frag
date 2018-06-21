#version 150

uniform float opacityImg;
uniform sampler2DRect maskTex;
uniform sampler2DRect tex0;

uniform int mode;

in vec2 texCoordVarying;
out vec4 fragColor;

void main()
{
	vec2 pos = gl_FragCoord.xy;
	vec3 src = texture(tex0, pos).rgb;
	float alpha = texture(tex0, pos).a;
	float mask = texture(maskTex, pos).r;

	if (mode == 0) mask = 1.0;

    // first three values are the rgb, fourth is the alpha
    fragColor = vec4(src, mask * opacityImg * alpha);
}