#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect imageMask;
uniform float opacityImg;
uniform int modo;

varying vec2 texCoordVarying;

void main()
{
    if (modo == 1) {
        vec4 texel0 = texture2DRect(tex0, texCoordVarying);
        vec4 texel1 = texture2DRect(imageMask, texCoordVarying);
        gl_FragColor = vec4(texel0.rgb, texel1.r * texel0.a * opacityImg);
    }
    
    else {

        vec4 texel0 = texture2DRect(tex0, texCoordVarying);
        gl_FragColor = vec4(texel0.rgb, texel0.a * opacityImg);
        
    }
}
