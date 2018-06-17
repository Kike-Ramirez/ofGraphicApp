#version 140
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex0;
uniform sampler2DRect imageMask;
uniform float opacityImg;
uniform int modo;

in vec2 texCoordVarying;
out vec4 color;

void main()
{
    if (modo == 1) {
        vec4 texel0 = texture2DRect(tex0, texCoordVarying);
        vec4 texel1 = texture2DRect(imageMask, texCoordVarying);
        color = vec4(texel0.rgb, (texel1.r+texel1.g+texel1.b) / 3 * texel0.a * opacityImg);
    }
    
    else {

        vec4 texel0 = texture2DRect(tex0, texCoordVarying);
        color = vec4(texel0.rgb, texel0.a * opacityImg);
        
    }
}
