attribute vec4 position;
uniform mat4 uMVPMatrix;

void main()
{
    gl_Position = uMVPMatrix * position;
}