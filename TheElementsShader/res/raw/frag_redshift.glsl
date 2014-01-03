uniform sampler2D uTex;
varying vec2 vTexCoord;

void main()
{
    vec4 texSample = texture2D(uTex, vTexCoord);
    if (texSample.x > 0.8)
    {
        gl_FragColor = texSample - vec4(texSample.x, 0.0, 0.0, 0.0);
    }
    else
    {
        gl_FragColor = texSample + vec4(0.01, 0.0, 0.0, 0.0);
    }
}