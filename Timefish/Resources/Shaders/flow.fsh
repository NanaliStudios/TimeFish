#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;

varying vec2 v_texCoord;

uniform sampler2D CC_Texture01;
uniform float flowCountX;
uniform float flowCountY;

void main() {
    float newS = v_texCoord.s + flowCountX;
    float newT = v_texCoord.t + flowCountY;
    if (newS > 1.0) {
        newS = newS - 1.0;
    }
    if (newT > 1.0) {
        newT = newT - 1.0;
    }

    vec2 texCoord=vec2(newS, newT);

    gl_FragColor = v_fragmentColor * texture2D(CC_Texture01, texCoord);
}