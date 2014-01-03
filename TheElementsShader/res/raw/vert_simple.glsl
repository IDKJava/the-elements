attribute vec4 aPos;
attribute vec2 aTexCoord;
uniform mat4 uMVPMatrix;
varying vec2 vTexCoord;

void main()
{
    gl_Position = uMVPMatrix * aPos;
    vTexCoord = aTexCoord;
}