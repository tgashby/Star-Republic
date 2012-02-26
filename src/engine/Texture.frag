static const char* TextrueFragmentShader = STRINGIFY(

varying vec2 TextureCoordOut;

uniform sampler2D Sampler;

void main(void)
{
   gl_FragColor = texture2D(Sampler, TextureCoordOut);
}
);