varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec3 colorRate;
uniform float u_BlendValue;

void main(void)
{
    vec4 col = texture2D(CC_Texture0, v_texCoord);
    
    vec3 gray = vec3(dot(col.rgb, colorRate));

    gl_FragColor.xyz =  mix(gray.rgb, col.rgb, u_BlendValue);
    gl_FragColor.w = col.w;
}

