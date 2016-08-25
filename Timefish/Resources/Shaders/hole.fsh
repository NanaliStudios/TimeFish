#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 centerPos;
uniform float holeRadius;
uniform vec4 inLineColor;
uniform vec4 outLineColor;
uniform float holeAlpha;

void main() {
    vec4 outside = vec4(0,0,0, 0);
    vec4 hole = vec4(0,0,0, holeAlpha);

    vec2 pos = v_texCoord - centerPos;
    float dist = pos.x * pos.x  + pos.y * pos.y;
    float diff = dist - holeRadius;

    vec4 org = vec4(0,0,0, 0);
    if (diff > 0.001) { // empty
        org = vec4(outside);
    }
    else if ( diff >= -0.01) { // circle out-line
        org = vec4(outLineColor.rgb, 1);
    }
    else if ( diff >= -0.018) { // circle in-line
        org = vec4(inLineColor.rgb, 1);
    }
    else { // inside of circle
        org = vec4(hole);
    }
    
    gl_FragColor = vec4(org);
}
