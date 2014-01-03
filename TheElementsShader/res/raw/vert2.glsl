attribute vec4 position;
attribute vec2 aTexCoord;
uniform mat4 uMVPMatrix;
varying vec2 texCoord;

void main()
{
    gl_Position = uMVPMatrix * position;
    texCoord = aTexCoord;
}