uniform sampler2D tex;
varying vec2 texCoord;

void main()
{
     vec4 texData = texture2D(tex, texCoord);
     if (gl_FragCoord.x <= 200) // 10 alternating stripes horizontally
     {
         // Set the color to green
         gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
     }
     else
     {
         // Just pass the tex color through
         gl_FragColor = texData;
     }
}