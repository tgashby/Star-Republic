static const char* TextureVertexShader = STRINGIFY(

attribute vec4 Position;
attribute vec2 TextureCoord;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;

varying vec2 TextureCoordOut;

void main(void)
{
   gl_Position = Projection * Modelview * Position;
   TextureCoordOut = TextureCoord;
}
);