#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

//in values
uniform vec2 size;
uniform sampler2D screenTexture;

vec4 lightEffect(vec2);

void main(void)
{
    vec4 col = lightEffect(v_texCoord);
    gl_FragColor = vec4(col) * v_fragmentColor;
}

vec4 lightEffect(vec2 pos)
{
    vec4 screenColor = texture2D(screenTexture, pos);
    vec4 lightColor = texture2D(CC_Texture0, pos);
    vec3 afterEffectColor = screenColor.rgb * lightColor.rgb;
    return vec4(afterEffectColor.r, afterEffectColor.g, afterEffectColor.b, 1.0);
}