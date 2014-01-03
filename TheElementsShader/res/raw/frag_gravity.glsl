uniform sampler2D uTex;
uniform float uRxscale;
varying vec2 vTexCoord;

// Simple gravity shader
void main()
{
    vec2 up = vTexCoord - vec2(uRxscale, 0.0);
    vec4 texSample = texture2D(uTex, up);
    if (up.x < 0.0)
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
    else
    {
        gl_FragColor = texSample;
    }
}