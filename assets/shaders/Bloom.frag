varying vec2 TextureCoordOut;

uniform sampler2D Sampler;

void main(void)
{
   vec4 color = texture2D(Sampler, TextureCoordOut);
   gl_FragColor = color;
}
