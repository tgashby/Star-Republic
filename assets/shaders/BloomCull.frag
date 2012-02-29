varying vec2 TextureCoordOut;

uniform sampler2D Sampler;

void main(void)
{
   vec4 color = texture2D(Sampler, TextureCoordOut);
   if (color.w != 0.0) {
      color = vec4(0, 0, 0, 1);
   }
   gl_FragColor = color;
}