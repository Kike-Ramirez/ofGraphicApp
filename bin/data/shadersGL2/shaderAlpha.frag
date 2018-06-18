#version 150

uniform sampler2DRect tex0;
uniform float opacityImg;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec4 texel0 = texture(tex0, texCoordVarying);
    // first three values are the rgb, fourth is the alpha
    outputColor = vec4(texel0.rgb, texel0.a * opacityImg);
}