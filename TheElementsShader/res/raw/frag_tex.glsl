uniform sampler2D uTex;
varying vec2 vTexCoord;

void main()
{
     // Just pass the tex color through
     gl_FragColor = texture2D(uTex, vTexCoord);
}