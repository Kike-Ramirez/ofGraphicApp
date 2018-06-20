#version 150

uniform float opacityImg;

in sampler2DRect tex0;
in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{
    vec4 texel0 = texture(tex0, texCoordVarying);
    // first three values are the rgb, fourth is the alpha
    outputColor = vec4(texel0.rgb, 1.0 * opacityImg);
}