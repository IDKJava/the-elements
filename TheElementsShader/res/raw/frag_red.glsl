uniform sampler2D uTex;
varying vec2 vTexCoord;

void main()
{
    vec4 texSample = texture2D(uTex, vTexCoord);
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}