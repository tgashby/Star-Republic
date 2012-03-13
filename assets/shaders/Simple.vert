attribute vec4 Position;
attribute vec2 TextureCoord;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform mat4 TextureMatrix;

varying vec2 TextureCoordOut;

void main(void)
{
   TextureCoordOut = vec2(TextureMatrix * vec4(TextureCoord, 1, 1));
   gl_Position = Projection * Modelview * Position;
}